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
        GLuint ModelView3x3MatrixID;
        GLuint DiffuseTextureID;
        GLuint NormalTextureID;
        GLuint SpecularTextureID;
        GLuint Texture;
        GLuint DiffuseTexture;
        GLuint NormalTexture;
        GLuint SpecularTexture;
        GLuint TextureID;
        std::vector<glm::vec3> vertices;
	    std::vector<glm::vec2> uvs;
	    std::vector<glm::vec3> normals;
        std::vector<glm::vec3> tangents;
        std::vector<glm::vec3> bitangents;
        bool res;
        GLuint vertexbuffer;
        GLuint uvbuffer;
        GLuint normalbuffer;
        GLuint tangentbuffer;
        GLuint bitangentbuffer;
        GLuint elementbuffer;
        GLuint LightID;
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        glm::vec3 position = glm::vec3( 0, 0.9, 3); 
        float horizontalAngle = 3.14f;
        float verticalAngle = 0.0f;
        float initialFoV = 45.0f;
        float speed = 3.0f; 
        float mouseSpeed = 0.005f;
        std::vector<unsigned short> indices;
        std::vector<glm::vec3> indexed_vertices;
        std::vector<glm::vec2> indexed_uvs;
        std::vector<glm::vec3> indexed_normals;
        std::vector<glm::vec3> indexed_tangents;
        std::vector<glm::vec3> indexed_bitangents;

    public:
        GLFWwindow* getWindow();
        void createWindow(int width, int heigth, char* windowName);
        void clearColor(float r, float g, float b);
        void clear();
        void render();
        void drawTriangle();
        void loadShader(char* shader);
        void loadTexture(char* txt);
        void loadObject(char* obj);
        void loadNormalTexture(char* txt);
        void loadSpecularTexture(char* txt);
        void loadVBO();
        void loadTextureIDS();
        void setLight();
        void drawModel();
        void computeMatricesFromInputs();
        void destroy();
        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();
};

#endif /* Render_h */
