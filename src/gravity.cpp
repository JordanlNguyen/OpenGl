#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
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

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int width, height;


    //create window *************************************************************
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
    glfwGetWindowSize(window, &width, &height);
    /******************************************************************************/

    Circle circle(0.0f , 0.0f, 0.1f, 120, width, height);
    GLuint shaderProgram = createShaderProgram(); //creating shader program
    int colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    int offsetLoc = glGetUniformLocation(shaderProgram, "uOffset"); //(program ID, offset variable name) : function returns the address of the variable specified within the specified program
    float lastTime = glfwGetTime();
    float gravity = 1.62f;
    //function loops window being opened untill its closed
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram); //order matters here
        glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f); //function sets fragement color for procedding code
        glUniform2f(offsetLoc, circle.offSetX, circle.offSetY);
        circle.drawCircle();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        //calculate velocity first then position
        float timeDisplacement = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        if(circle.offSetY + circle.centerY >= -1){
            circle.velocity += gravity * timeDisplacement;
            circle.offSetY -= circle.velocity * timeDisplacement;
        }
        std::cout << circle.centerY + circle.offSetY << std::endl;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}