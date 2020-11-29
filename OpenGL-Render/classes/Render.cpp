#include "Render.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace std;

#include "shader.hpp"
#include "texture.hpp"
#include "objloader.hpp"
#include "vboindexer.hpp"
#include "tangentspace.hpp"


//Initialize all stuff needed to render our model
void Render::createWindow(int width, int heigth, char* windowName)
{
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
	}

	glfwWindowHint(GLFW_SAMPLES, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( width, heigth, windowName, NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) 
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  	glfwPollEvents();
    glfwSetCursorPos(window, width/2, heigth/2);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	glEnable(GL_CULL_FACE);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
}

//Cleans the buffer current color to paint anything
void Render::clearColor(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
}

//Clenas up the whole screen with the current color
void Render::clear()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

//Shows up what's in the buffer
void Render::render()
{
    glfwSwapBuffers(window);
	glfwPollEvents();    
}

void Render::drawTriangle()
{
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );


	static const GLfloat g_vertex_buffer_data[] = { 
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glUseProgram(programID);
	glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                 
			3,              
			GL_FLOAT,          
			GL_FALSE,        
			0,                
			(void*)0           
		);

		glDrawArrays(GL_TRIANGLES, 0, 3); 

		glDisableVertexAttribArray(0);
}

void Render::loadShader(char* shader)
{
	this -> programID = LoadShaders
	( 
		"../shaders/NormalMapping.vertexshader", shader 
	);

	this -> MatrixID = glGetUniformLocation(programID, "MVP");
	this -> ViewMatrixID = glGetUniformLocation(programID, "V");
	this -> ModelMatrixID = glGetUniformLocation(programID, "M");
	this -> ModelView3x3MatrixID = glGetUniformLocation(programID, "MV3x3");
}

void Render::loadTexture(char* txt)
{
	DiffuseTexture = loadDDS(txt);
}

void Render::loadNormalTexture(char* txt)
{
	NormalTexture = loadBMP_custom(txt);
}

void Render::loadSpecularTexture(char* txt)
{
	SpecularTexture = loadDDS(txt);
}

void Render::loadTextureIDS()
{
	DiffuseTextureID  = glGetUniformLocation(programID, "DiffuseTextureSampler");
	NormalTextureID  = glGetUniformLocation(programID, "NormalTextureSampler");
	SpecularTextureID  = glGetUniformLocation(programID, "SpecularTextureSampler");
}

void Render::loadObject(char* obj)
{
	res = loadOBJ((char*) obj, vertices, uvs, normals);
}

void Render::loadVBO()
{
	computeTangentBasis(
        vertices, uvs, normals,
        tangents, bitangents    
    );
	indexVBO_TBN(
        vertices, uvs, normals, tangents, bitangents,
        indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents
    );

	glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tangentbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(glm::vec3), &indexed_tangents[0], GL_STATIC_DRAW);

	glGenBuffers(1, &bitangentbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(glm::vec3), &indexed_bitangents[0], GL_STATIC_DRAW);

	glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

}

void Render::setLight()
{
	glUseProgram(programID);
	this -> LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
}

void Render::drawModel()
{
	 glUseProgram(programID);

    computeMatricesFromInputs();
    glm::mat4 ProjectionMatrix = getProjectionMatrix();
    glm::mat4 ViewMatrix = getViewMatrix();
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    glm::mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;
    glm::mat3 ModelView3x3Matrix = glm::mat3(ModelViewMatrix);
    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
    glUniformMatrix3fv(ModelView3x3MatrixID, 1, GL_FALSE, &ModelView3x3Matrix[0][0]);
    
    glm::vec3 lightPos = glm::vec3(0,0,4);
    glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, DiffuseTexture);
    glUniform1i(DiffuseTextureID, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, NormalTexture);
    glUniform1i(NormalTextureID, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, SpecularTexture);
    glUniform1i(SpecularTextureID, 2);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,               
        3,           
        GL_FLOAT,         
        GL_FALSE,      
        0,               
        (void*)0          
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
        1,                             
        2,                       
        GL_FLOAT,                     
        GL_FALSE,                     
        0,                             
        (void*)0                        
    );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
        2,                      
        3,                 
        GL_FLOAT,          
        GL_FALSE,                 
        0,                   
        (void*)0                          
    );

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
    glVertexAttribPointer(
        3,                      
        3,                 
        GL_FLOAT,          
        GL_FALSE,            
        0,                   
        (void*)0                          
    );

    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
    glVertexAttribPointer(
        4,                      
        3,                 
        GL_FLOAT,          
        GL_FALSE,                 
        0,                   
        (void*)0                          
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    glDrawElements(
        GL_TRIANGLES,     
        indices.size(),    
        GL_UNSIGNED_SHORT,
        (void*)0           
    );
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

}

GLFWwindow* Render::getWindow()
{
	return this-> window;
}

void Render::computeMatricesFromInputs(){

	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	glfwSetCursorPos(window, 1024/2, 768/2);

	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	glm::vec3 up = glm::cross( right, direction );

	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		auto tempPosition{position + (direction * deltaTime * speed)};
		if((tempPosition.z > 0.3 || tempPosition.z < -0.3) || (tempPosition.y > 2.0f || tempPosition.y < -0.2f) || (tempPosition.x < -1.0f || tempPosition.x > 0.999f))
		{
			position += direction * deltaTime * speed;
		}
	}
	

	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS)
	{
		position += right * deltaTime * speed;
	}

	if (glfwGetKey( window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += right * deltaTime * speed;
	}

	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS)
	{
		position -= right * deltaTime * speed;
	}

	if (glfwGetKey( window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= right * deltaTime * speed;

	}

	if(glfwGetKey( window, GLFW_KEY_W) == GLFW_PRESS){
		position.y += deltaTime * speed;
	}

	if(glfwGetKey( window, GLFW_KEY_S) == GLFW_PRESS){
		position.y -= deltaTime * speed;
	}

	if(glfwGetKey( window, GLFW_KEY_1) == GLFW_PRESS)
	{
		this -> loadShader("../shaders/NormalMapping.fragmentshader");
	}

	if(glfwGetKey( window, GLFW_KEY_2) == GLFW_PRESS)
	{
		this -> loadShader("../shaders/Comic.fragmentshader");
	}

	if(glfwGetKey( window, GLFW_KEY_3) == GLFW_PRESS)
	{
		this -> loadShader("../shaders/Negative.fragmentshader");
	}

	if(glfwGetKey( window, GLFW_KEY_4) == GLFW_PRESS)
	{
		this -> loadShader("../shaders/Static.fragmentshader");
	}

	float FoV = initialFoV;
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	ViewMatrix       = glm::lookAt(
								position,          
								position+direction, 
								up                  
						   );
	lastTime = currentTime;
}

glm::mat4 Render::getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 Render::getProjectionMatrix(){
	return ProjectionMatrix;
}

void Render::destroy()
{
	glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteBuffers(1, &tangentbuffer);
    glDeleteBuffers(1, &bitangentbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &DiffuseTexture);
    glDeleteTextures(1, &NormalTexture);
    glDeleteTextures(1, &SpecularTexture);
    glDeleteVertexArrays(1, &VertexArrayID);

	glfwTerminate();
}