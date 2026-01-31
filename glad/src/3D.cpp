#include <glad/glad.h>
#include "objects/Grid.hpp"
#include "objects/Sphere.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include <iostream>
using namespace std;

struct Camera{
    float yaw = -90.0f;
    float pitch = 20.0f;
    float radius = 100.0f;
    double lastX = 400;
    double lastY = 300;
    bool firstMouse = true;
    bool dragging = false;
    glm::vec3 cameraPos = glm::vec3(
        radius * cos(glm::radians(pitch) * cos(glm::radians(yaw))), 
        radius * sin(glm::radians(pitch)), 
        radius * cos(glm::radians(pitch) * sin(glm::radians(yaw))));
};

GLuint createShaderProgram() {
    const char* vertexShaderSource = R"(
        #version 410 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;

        out vec3 FragPos;
        out vec3 Normal;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        
        void main()
        {
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;

            gl_Position = projection * view * vec4(FragPos, 1.0);
        }
    )";
    const char* fragmentShaderSource = R"(
        #version 410 core
        out vec4 FragColor;

        in vec3 FragPos;
        in vec3 Normal;

        uniform vec3 lightPos;
        uniform vec3 viewPos;
        uniform vec3 objectColor;
        uniform vec3 lightColor;

        void main()
        {
            // Ambient
            float ambientStrength = 0.2;
            vec3 ambient = ambientStrength * lightColor;

            // Diffuse
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;

            vec3 result = (ambient + diffuse) * objectColor;
            FragColor = vec4(result, 1.0);
        }
    )";

    GLint success;
    char infoLog[512];

    //compiling
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //return integer vector
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex Shader Error: " << infoLog << std::endl;
    }
    //---------
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex Shader Error: " << infoLog << std::endl;
    }

    //create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //cleanup unused data
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!cam->dragging) return;
    static float sensitivity = 0.1f;

    if (cam->firstMouse) {
        cam->lastX = xpos;
        cam->lastY = ypos;
        cam->firstMouse = false;
    }

    float xoffset = xpos - cam->lastX;
    float yoffset = cam->lastY - ypos; // reversed

    cam->lastX = xpos;
    cam->lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cam->yaw   -= xoffset;
    cam->pitch -= yoffset;

    // clamp pitch
    cam->pitch = glm::clamp(cam->pitch, -89.0f, 89.0f);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) cam->dragging = true;
        else if (action == GLFW_RELEASE) cam->dragging = false;
    }
}

int main(int argc, char* argv[]) {

    // if(argc != 17) {
    //     cerr << "ecpected 16 arguments" << endl;
    //     return -1;
    // }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    int width, height;


    //create window *************************************************************
    GLFWwindow* window = glfwCreateWindow(900, 900, "3D", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        return -1;
    }
    glfwGetFramebufferSize(window, &width, &height);

    //method directs OpenGL calls on current thread to window when created
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    glfwGetWindowSize(window, &width, &height);
    /******************************************************************************/

    GLuint shaderProgram = createShaderProgram(); //creating shader program
    int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    Camera camera;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 500.0f); //maps and translates 3d coordinates to 2d screen print ; (field of view : vertical angle of camera lense, aspect ratio : keeps objects looking streched, near clipping plane : closest distance visible, far clippiing plane : furthest distance visible)
    // Sphere sphere1(atof(argv[5]), atof(argv[7]), atof(argv[9]), atof(argv[11]), atof(argv[13]), atof(argv[15]), atof(argv[3]), atof(argv[1]));
    // Sphere sphere2(atof(argv[6]), atof(argv[8]), atof(argv[10]), atof(argv[12]), atof(argv[14]), atof(argv[16]), atof(argv[4]), atof(argv[2]));
    Grid grid;
    vector<Sphere> spheres = {
        Sphere (0,0,0,0,0,0,10.0f, 10000.0f),
        Sphere (35,0,0,-1,0,5,3,150),
        Sphere (40,0,0,2,0,0,2,1)
    };


    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float timeNow = glfwGetTime();
        float dt = timeNow - lastTime;
        lastTime = timeNow;
        camera.cameraPos.x = camera.radius * cos(glm::radians(camera.pitch)) * cos(glm::radians(camera.yaw));
        camera.cameraPos.y = camera.radius * sin(glm::radians(camera.pitch));
        camera.cameraPos.z = camera.radius * cos(glm::radians(camera.pitch)) * sin(glm::radians(camera.yaw));

        glm::mat4 view = glm::lookAt(camera.cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        grid.drawGrid(view, projection, colorLoc, shaderProgram, camera.cameraPos);
        for(int i = 0; i < spheres.size(); ++i){
            spheres[i].drawSphere(view, projection, colorLoc, shaderProgram, camera.cameraPos);
        }
        for(int i = 0; i < spheres.size(); ++i){
            for(int j = 0; j < spheres.size(); ++j){
                if(i != j){
                    spheres[i].computeGravity(spheres[j]);
                }
            }
        }
        for(int i = 0; i < spheres.size(); ++i){
            spheres[i].updatePosition(dt);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}