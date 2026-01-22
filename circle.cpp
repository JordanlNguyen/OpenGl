#include "circle.h"
#include <cmath>


Circle::Circle(float cX, float cY, float rad, int res, int screenWidth, int screenHeight, float vx, float vy, float m){
    centerX = cX;
    centerY = cY;
    offSetX = cX;
    offSetY = cY;
    radius = rad;
    resolution = res;
    screenWidth = screenWidth;
    screenHeight = screenHeight;
    velocityY = vy;
    velocityX = vx;
    mass = m;
    createVertices();
    setUpBuffer();
}

void Circle::createVertices(){    
    //vertices in NDC  
    vertices.push_back(centerX);
    vertices.push_back(centerY);
    for(int i = 0; i <= resolution; ++i){
        float angle = 2.0f * 3.14159f * i / resolution;
        float x = centerX + cos(angle) * radius;
        float y = centerY + sin(angle) * radius;
        vertices.push_back(x);
        vertices.push_back(y);
    }
}

void Circle::setUpBuffer() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,                  // layout location
        2,                  // vec2
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Circle::drawCircle(){
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
    glBindVertexArray(0);
}

void Circle::computeGravity(Circle &c2){
    //this function is trying to calculate the force vector of this object
    float dx = c2.offSetX - offSetX;
    float dy = c2.offSetY - offSetY;
    float r2 = pow(dx,2) + pow(dy,2);
    float r = sqrt(r2);

    float F = G * mass * c2.mass / r2;
    fx = F * dx / r;
    fy = F * dy / r;
}

void Circle::updatePosition(float dt){
    float ax = fx / mass;
    float ay = fy / mass;

    velocityX += ax * dt;
    velocityY += ay * dt;

    offSetX += velocityX * dt;
    offSetY += velocityY * dt;
}