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
    glGenVertexArrays(1, &arrowVAO);
    glGenBuffers(1, &arrowVBO);
}

void Circle::createVertices(){    
    //vertices in NDC  
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    for(int i = 0; i <= resolution; ++i){
        float angle = 2.0f * 3.14159f * i / resolution;
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;
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

void Circle::drawCircle(int colorLoc, int offsetLoc){
    glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f); //function sets fragement color for procedding code
    glUniform2f(offsetLoc, offSetX, offSetY);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
    glBindVertexArray(0);
}

void Circle::drawArrow(int colorLoc, int offsetLoc){
    arrowVertices.clear();
    float scale = 0.1f; // scale for arrow length
    float thickness = 0.01f; // thickness of the arrow
    float vx = velocityX * scale;
    float vy = velocityY * scale;
    
    // Normalize the direction vector
    float len = sqrt(vx * vx + vy * vy);
    if (len > 0) {
        vx /= len;
        vy /= len;
    } else {
        return; // no velocity, no arrow
    }
    
    // Perpendicular vector
    float px = -vy;
    float py = vx;
    
    // Half thickness
    float hx = px * thickness * 0.5f;
    float hy = py * thickness * 0.5f;
    
    // Create quad vertices: start - perp, start + perp, end - perp, end + perp
    // For GL_TRIANGLE_STRIP: 0: start-perp, 1: start+perp, 2: end-perp, 3: end+perp
    arrowVertices.push_back(-hx); // start - perp
    arrowVertices.push_back(-hy);
    arrowVertices.push_back(hx);  // start + perp
    arrowVertices.push_back(hy);
    arrowVertices.push_back(vx * len - hx); // end - perp
    arrowVertices.push_back(vy * len - hy);
    arrowVertices.push_back(vx * len + hx); // end + perp
    arrowVertices.push_back(vy * len + hy);
    
    glBindVertexArray(arrowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, arrowVBO);
    glBufferData(GL_ARRAY_BUFFER, arrowVertices.size() * sizeof(float), arrowVertices.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f); // green for arrow
    glUniform2f(offsetLoc, offSetX, offSetY);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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