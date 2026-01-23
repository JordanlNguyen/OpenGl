using namespace std;
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include <iostream>
#include "../grid.h"
#include "../circle.h"

GLuint createShaderProgram() {
    const char* vertexShaderSource = R"(
        #version 410 core
        layout (location = 0) in vec2 aPos;
        uniform vec2 uOffset; //offset will be added with original center
        void main() {
            gl_Position = vec4(aPos + uOffset, 0.0, 1.0); //aPos == (x,y) // aPos is a vecotr of 2 (x,y)
        }
    )";
    const char* fragmentShaderSource = R"(
        #version 410 core
        out vec4 FragColor;
        uniform vec3 uColor; //color input
        void main() {
            FragColor = vec4(uColor, 1.0);
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
bool detectCollision(Circle &c1, Circle &c2){
    //calculate distance of 2 bodies
    float dx = c1.offSetX - c2.offSetX;
    float dy = c1.offSetY - c2.offSetY;
    float distance = sqrt(pow(dx,2) + pow(dy,2));
    if(distance <= c1.radius + c2.radius) return true;
    return false;
}
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    int width, height;


    //create window *************************************************************
    GLFWwindow* window = glfwCreateWindow(900, 900, "OpenGL", nullptr, nullptr);
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

    Circle circle1(-0.0f , -0.0f, 0.09f, 120, width, height, 0.0f, 0.0f, 1.0f);
    Circle circle2(0.9f , 0.0f, 0.04f, 120, width, height, -0.0f, -0.9f, 0.0123f);
    GLuint shaderProgram = createShaderProgram(); //creating shader program
    int colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    int offsetLoc = glGetUniformLocation(shaderProgram, "uOffset"); //(program ID, offset variable name) : function returns the address of the variable specified within the specified program
    float lastTime = glfwGetTime();
    float gravity = 1.62f;
    //function loops window being opened untill its closed
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        //draw
        glUseProgram(shaderProgram); //order matters here
        circle1.drawCircle(colorLoc, offsetLoc);
        circle2.drawCircle(colorLoc, offsetLoc);
        circle1.drawArrow(colorLoc, offsetLoc);
        circle2.drawArrow(colorLoc, offsetLoc);

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        
        //calculate
        if(!detectCollision(circle1, circle2)){
            float currentTime = glfwGetTime();
            float dt = currentTime - lastTime;
            lastTime = currentTime;
            circle1.computeGravity(circle2);
            circle2.computeGravity(circle1);
            circle1.updatePosition(dt);
            circle2.updatePosition(dt);        
            cout << "circl1 offsetX " << circle1.offSetX << " | " << "circle1 offSetY " << circle1.offSetY <<" | distance : " << sqrt(pow(circle1.offSetX - circle2.offSetX,2) + pow(circle2.offSetY - circle1.offSetY,2))<< endl;
            cout << "circl1 offsetY " << circle2.offSetX << " | " << "circle2 offSetY " << circle2.offSetY << endl;  
        }

      

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}