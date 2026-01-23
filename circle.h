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
        float mass = 0.9f;
        float fy = 0;
        float fx = 0;
        GLuint VAO;
        GLuint VBO;
        std::vector<float> vertices;  
        const float G = 1.0f;
        GLuint arrowVAO;
        GLuint arrowVBO;
        std::vector<float> arrowVertices;
        
        Circle(float cX, float cY, float rad, int res, int screenWidth, int screenHeight, float vx, float vy, float m);
        void drawCircle(int colorLoc, int offsetLoc);
        void drawArrow(int colorLoc, int offsetLoc);
        void computeGravity(Circle &c2);
        void updatePosition(float dt);
};