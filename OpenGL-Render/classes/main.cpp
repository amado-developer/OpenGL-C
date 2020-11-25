// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>


// Include GLM
#include "glm/glm.hpp"

#include "Render.h"
using namespace glm;

int main()
{
    //Some of the code (right now) is taken from http://www.opengl-tutorial.org/
    //and it is still being modified (adapted).
    Render render;
    render.createWindow(1024, 768, (char*)"OpenGL");
    render.loadShader();
    render.loadTexture("spiderman.dds");
    render.loadObject("spiderman.obj");
    render.loadVBO();
    render.setLight();
    while( glfwGetKey(render.getWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(render.getWindow()) == 0 )
	{
        render.clear();
        render.drawModel();
        render.render();
    }
    glfwTerminate();
    render.destroy();

	return 0;
}

