#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Grid.hpp"

Grid::Grid(){
    generateVertices();
    bindElements();
}

void Grid::generateVertices(){
    for (float i = -gridSize; i <= gridSize; i += gridStep) {
        // Line along X at z = i
        gridVertices.push_back(-gridSize); // x1
        gridVertices.push_back(0.0f);      // y1
        gridVertices.push_back(i);          // z1

        gridVertices.push_back(gridSize);  // x2
        gridVertices.push_back(0.0f);      // y2
        gridVertices.push_back(i);          // z2

        // Line along Z at x = i
        gridVertices.push_back(i);          // x1
        gridVertices.push_back(0.0f);      // y1
        gridVertices.push_back(-gridSize); // z1

        gridVertices.push_back(i);          // x2
        gridVertices.push_back(0.0f);      // y2
        gridVertices.push_back(gridSize);  // z2
    }
}

void Grid::bindElements(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Grid::drawGrid(glm::mat4 view, glm::mat4 projection, int colorLoc, GLuint shaderProgram, glm::vec3 cameraPos){
    glUseProgram(shaderProgram);
    glm::mat4 model = glm::mat4(1.0f); // identity
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.7f, 0.7f, 0.7f); // light gray

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, gridVertices.size() / 3);
    glBindVertexArray(0);
}