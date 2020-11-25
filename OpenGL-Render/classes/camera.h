#ifndef Camera_h
#define Camera_h
#include <GL/glew.h>
#include <vector>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
class Camera
{
    private:
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        glm::vec3 position;
        float horizontalAngle;
        float verticalAngle;
        float initialFoV;
        float speed;
        float mouseSpeed;

    public:
        Camera();
        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();
        void computeMatricesFromInputs();
};
#endif /* Camera_h */