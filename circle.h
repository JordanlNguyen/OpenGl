#include <vector>
#include <glad/glad.h>

class Circle{
    private:
        void createVertices();
        void setUpBuffer();
    public:
        float centerX;
        float centerY;
        float radius;
        int resolution;
        int screenWidth;
        int screenHeight;
        float offSetX;
        float offSetY;
        float velocityX;
        float velocityY;
        float mass = 0.1f;
        float fy = 0;
        float fx = 0;
        GLuint VAO;
        GLuint VBO;
        std::vector<float> vertices;  
        const float G = 1.0f;
        
        Circle(float cX, float cY, float rad, int res, int screenWidth, int screenHeight, float vx, float vy, float m);
        void drawCircle();
        void computeGravity(Circle &c2);
        void updatePosition(float dt);
};