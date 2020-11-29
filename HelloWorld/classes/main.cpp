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
    Render render;
    render.createWindow(800, 600, (char*)"OpenGL");
    
    while( glfwGetKey(render.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(render.window) == 0 )
	{
        render.clear();
        render.drawTriangle();
        render.render();
    }
    glfwTerminate();

	return 0;
}

