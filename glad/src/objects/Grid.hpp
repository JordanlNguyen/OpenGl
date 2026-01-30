#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
class Grid{
    private:
        float gridSize = 500.0f;
        float gridStep = 1.0f;
        std::vector<float> gridVertices;
        GLuint VBO, VAO;

        void generateVertices();
        void bindElements();
    public:
        Grid();
        void drawGrid(glm::mat4 view, glm::mat4 projection, int colorLoc, GLuint shaderProgram, glm::vec3 cameraPos);

};