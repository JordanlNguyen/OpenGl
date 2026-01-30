#include <vector>
#include <glm/glm.hpp>
class Sphere{
    private:
        int sectorCount = 36; //longitude
        int stackCount = 18; //latitude
        float radius;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        GLuint VAO, VBO, EBO;
        float x;
        float y;
        float z;
        float velocityX;
        float velocityY;
        float velocityZ;
        float fx = 0;
        float fy = 0;
        float fz = 0;
        float mass;
        const float G = 1.0f;

        void generateVertices();
        void generateIndices();
        void bindElements();
    public:
        Sphere(float X, float Y, float Z, float vx, float vy, float vz, float r, float m);
        void drawSphere(glm::mat4 view, glm::mat4 projection, int colorLoc, GLuint shaderProgram, glm::vec3 cameraPos);
        void computeGravity(Sphere &s2);
        void updatePosition(float dt);    
};      