#include <vector>

class Circle{
    private:
        void createVertices(){    
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
    public:
        float centerX;
        float centerY;
        float radius;
        int resolution;
        int screenWidth;
        int screenHeight;
        std::vector<float> vertices;  
        
        Circle(float cX, float cY, float rad, int res, int screenWidth, int screenHeight){
            centerX = cX;
            centerY = cY;
            radius = rad;
            resolution = res;
            screenWidth = screenWidth;
            screenHeight = screenHeight;
            createVertices();
        }

        
};