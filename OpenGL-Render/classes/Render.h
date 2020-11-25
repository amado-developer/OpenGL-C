#ifndef Render_h
#define Render_h
#include <GL/glew.h>
#include <vector>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
class Render{
    private:
        GLFWwindow* window;
        GLuint VertexArrayID;
        GLuint programID;
        GLuint MatrixID;
        GLuint ViewMatrixID;
        GLuint ModelMatrixID;
        GLuint Texture;
        GLuint TextureID;
        std::vector<glm::vec3> vertices;
	    std::vector<glm::vec2> uvs;
	    std::vector<glm::vec3> normals;
        bool res;
        GLuint vertexbuffer;
        GLuint uvbuffer;
        GLuint normalbuffer;
        GLuint LightID;
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        glm::vec3 position = glm::vec3( 0, 0.9, 3); 
        float horizontalAngle = 3.14f;
        float verticalAngle = 0.0f;
        float initialFoV = 45.0f;
        float speed = 3.0f; 
        float mouseSpeed = 0.005f;

    public:
        GLFWwindow* getWindow();
        void createWindow(int width, int heigth, char* windowName);
        void clearColor(float r, float g, float b);
        void clear();
        void render();
        void drawTriangle();
        void loadShader();
        void loadTexture(char* txt);
        void loadObject(char* obj);
        void loadVBO();
        void setLight();
        void drawModel();
        void computeMatricesFromInputs();
        void destroy();
        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();
};

#endif /* Render_h */
