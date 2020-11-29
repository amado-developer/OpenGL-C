#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "Render.h"
using namespace glm;
/**
 * Universidad del Valle de Guatemala
 * Amado Garcia
 * This Project has many parts of code taken from http://www.opengl-tutorial.org/, adapted and in many ways modified.
 **/
int main()
{
    Render render;
    render.createWindow(1024, 768, (char*)"OpenGL");
    render.loadShader("../shaders/NormalMapping.fragmentshader");
    render.loadTexture("../textures/spiderman.dds");
    render.loadNormalTexture("../textures/normal_spiderman.bmp");
    render.loadSpecularTexture("../textures/specular_spiderman.dds");
    render.loadTextureIDS();
    render.loadObject("../obj/spiderman.obj");
    render.loadVBO();
    render.setLight();
    while( glfwGetKey(render.getWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(render.getWindow()) == 0 )
	{
        render.clear();
        render.drawModel();
        render.render();
    }
    render.destroy();

	return 0;
}

