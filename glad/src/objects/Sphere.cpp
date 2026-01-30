#include <glad/glad.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Sphere.hpp"

Sphere::Sphere(float X, float Y, float Z, float vx, float vy, float vz, float r, float m){
    x = X;
    y = Y;
    z = Z;
    velocityX = vx;
    velocityY = vy;
    velocityZ = vz;
    radius = r;
    mass = m;
    generateVertices();
    generateIndices();
    bindElements();
}
void Sphere::generateVertices(){
    for(int i = 0; i <= stackCount; ++i){
        float stackAngle = glm::pi<float>() / 2 - i * glm::pi<float>() / stackCount; // from pi/2 to -pi/2
        float xy = radius * cos(stackAngle); // radius of current stack circle
        float zz = radius * sin(stackAngle);

        for(int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * 2 * glm::pi<float>() / sectorCount; // 0 to 2pi
            float xx = xy * cos(sectorAngle);
            float yy = xy * sin(sectorAngle);
            vertices.push_back(xx);
            vertices.push_back(yy);
            vertices.push_back(zz);
            //push back normalized positions
            glm::vec3 normal = glm::normalize(glm::vec3(xx, yy, zz));
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
        }
    }
}

void Sphere::generateIndices(){
    for(int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1);
        int k2 = k1 + sectorCount + 1;

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if(i != 0)
                indices.push_back(k1), indices.push_back(k2), indices.push_back(k1 + 1);
            if(i != (stackCount-1))
                indices.push_back(k1 + 1), indices.push_back(k2), indices.push_back(k2 + 1);
        }
    }
}

void Sphere::bindElements(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Sphere::drawSphere(glm::mat4 view, glm::mat4 projection, int colorLoc, GLuint shaderProgram, glm::vec3 cameraPos){
    glm::mat4 model = glm::mat4(1.0f); //create identity matrix
    model = glm::translate(model, glm::vec3(x,y,z));
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    /* inefficientcies */
    /* looking up evertyime */
    /* need to change and optimizee so that these are not being called evertime draw is called*/
    glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"),10.0f, 10.0f, 10.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"),cameraPos[0], cameraPos[1], cameraPos[2]);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"),1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"),0.2f, 0.7f, 0.3f);
    /*************** */
    glUniform3f(colorLoc, 0.2f, 0.7f, 0.3f);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Sphere::computeGravity(Sphere &s2){
    float dx = s2.x - x;
    float dy = s2.y - y;
    float dz = s2.z - z;
    float r2 = pow(dx,2) + pow(dy,2) + pow(dz,2);
    const float epsilon = 0.001f;
    if (r2 < epsilon) return;
    float r = sqrt(r2);

    float F = G * mass * s2.mass / r2;
    fx = F * dx / r;
    fy = F * dy / r;
    fz = F * dz / r;
}

void Sphere::updatePosition(float dt){
    float ax = fx / mass;
    float ay = fy / mass;
    float az = fz / mass;

    velocityX += ax * dt;
    velocityY += ay * dt;
    velocityZ += az * dt;

    x += velocityX * dt;
    y += velocityY * dt;
    z += velocityZ * dt;
}