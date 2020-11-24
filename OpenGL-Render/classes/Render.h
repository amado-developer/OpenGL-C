#ifndef Render_h
#define Render_h
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

class Render{

    public:
        GLFWwindow* window;
        void createWindow(int width, int heigth, char* windowName);
        void clearColor(float r, float g, float b);
        void clear();
        void render();
        void drawTriangle();
};

#endif /* Render_h */
