#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
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
    int random;

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

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int width, height;

    //create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
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

    Circle circle(0.0f , 0.0f, 0.5f, 120, width, height);
    
    GLuint VAO, VBO; //generate GL objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO); //binding
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData( //allocate GPU memory for data
        GL_ARRAY_BUFFER,
        circle.vertices.size() * sizeof(float),
        circle.vertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer( //describe the raw float data
        0,              // attribute index (location = 0)
        2,              // 2 floats per vertex (x, y)
        GL_FLOAT,       // data type
        GL_FALSE,       // normalize?
        2 * sizeof(float), // stride
        (void*)0        // offset
    );
    glEnableVertexAttribArray(0);
    
    GLuint shaderProgram = createShaderProgram(); //creating shader program

    float lastTime = glfwGetTime();
    float offsetX = 0.0f;
    float offsetY = 0.0f;

    //function loops window being opened untill its closed
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        /**********************************************************************/
        glUseProgram(shaderProgram); //order matters here
        int colorLoc = glGetUniformLocation(shaderProgram, "uColor");
        glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f); // red
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, circle.vertices.size() / 2);

        int offsetLoc = glGetUniformLocation(shaderProgram, "uOffset"); //(program ID, offset variable name) : function returns the address of the variable specified within the specified program
        offsetX -= 0.5f;
        glUniform2f(offsetLoc, offsetX, offsetY); //sending values to GPU for use
        std::cout << offsetLoc << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // change circle center to move down
        // float currentTime = glfwGetTime();
        // float delTime = currentTime - lastTime;
        // lastTime = currentTime;
        /**********************************************************************/
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}