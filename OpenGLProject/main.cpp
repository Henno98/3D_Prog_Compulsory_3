#include <iostream>
#include <fstream>
#include <iomanip>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <cstddef>
#include <string>
#include "NPC.h"
#include <Eigen/Dense>
#include "Trophy.h"
#include "Shaders/ShaderClass.h"
#include "Shaders/VAO.h"
#include "Shaders/VBO.h"
#include "Shaders/EBO.h"
#include "Camera.h"
#include "Cube.h"
#include "Shaders/Light.h"
#include "Wall.h"
using namespace std;
using namespace Eigen;
using namespace glm;

// Window dimensions
const unsigned int width = 1200;
const unsigned int height = 1200;


struct Planevertex
{

	GLfloat x,y,z;
	float r, g, b;
	float n1,n2,n3;
	
	

};

struct Triangle
{
	GLuint v0,v1,v2;

};

float curveplane(float x, float y)
{

	return cos(x)+ sin(y);
}
//vec3 barycentricCoordinatesForQuad(const vec2& p1, const vec2& p2, const vec2& p3, const vec2& p4, const vec2& point)
//{
//	// Calculate area of the two triangles formed by the quad
//	float areaTotal = abs((p1.x - p3.x) * (p2.y - p4.y) - (p2.x - p4.x) * (p1.y - p3.y));
//
//	// Calculate barycentric coordinates for each triangle
//	float alpha1 = abs((point.x - p3.x) * (p2.y - p4.y) - (p2.x - p4.x) * (point.y - p3.y)) / areaTotal;
//	float beta1 = abs((point.x - p2.x) * (p3.y - p1.y) - (p3.x - p1.x) * (point.y - p2.y)) / areaTotal;
//
//	// Calculate barycentric coordinates for the other triangle
//	float alpha2 = abs((point.x - p1.x) * (p4.y - p2.y) - (p4.x - p2.x) * (point.y - p1.y)) / areaTotal;
//	float beta2 = abs((point.x - p4.x) * (p1.y - p3.y) - (p1.x - p3.x) * (point.y - p4.y)) / areaTotal;
//
//	// Use the barycentric coordinates of the triangle with the point inside it
//	if (alpha1 + beta1 <= 1.0)
//		return vec3(alpha1, beta1, 1.0f - alpha1 - beta1);
//	else
//		return vec3(alpha2, beta2, 1.0f - alpha2 - beta2);
//}



GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};


int main()
{
	// Initialize GLFW
	glfwInit();
	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create a GLFWwindow object of 800 by 800 pixels, naming it "OpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGLProject", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, width, height);


	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	
//	NPC npc;

	Camera camera(width, height, glm::vec3((-0.1f ), 5.f, (-0.1f )));
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);



	mat4 Doormatrix = mat4(1.0f);
	Doormatrix = translate(Doormatrix, vec3(0, 0, 0));
	

	vector<Planevertex> PlaneVertices;
	vector<Triangle> Indices;
	
	float step = 0.5f;
	float size = 24;
	for (float i = 0; i < size; i+=step)
	{
		for (float j = 0; j < size; j+=step) {

			PlaneVertices.emplace_back(Planevertex{ i, curveplane(i,j),j,0,0,1,i/size,0,j/size});
		
		}
		

	}
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - 1; j++) {
			unsigned int v0 = i * (size/step) + j;
			unsigned int v1 = v0 + 1;
			unsigned int v2 = v0 + (size/step);
			unsigned int v3 = v2 + 1;

			Indices.emplace_back(Triangle{ v0, v2, v1 });
			Indices.emplace_back(Triangle{ v1, v2, v3 });
		}
	}
	//cube.CubeMatrix = translate(cube.CubeMatrix, vec3(5, 0, 5));
	
	Light light;


	VAO planevao;
	planevao.Bind();
	VBO planevbo(reinterpret_cast<GLfloat*>(PlaneVertices.data()), (PlaneVertices.size() * sizeof(Planevertex)));
	planevbo.Bind();
	EBO planeebo(reinterpret_cast<GLuint*>(Indices.data()), (Indices.size() * sizeof(Triangle)));
	planeebo.Bind();
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	planevao.Unbind();
	planevbo.Unbind();
	planeebo.Unbind();

	


	

	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");


	glm::vec3 CubePos = glm::vec3(0.f, 0.f, 0.f);
	glm::mat4 Cubemodel = glm::mat4(1.0f);
	Cubemodel = glm::translate(Cubemodel, CubePos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(light.lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), light.lightColor.x, light.lightColor.y, light.lightColor.z, light.lightColor.w);
	shaderProgram.Activate();
	
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), light.lightColor.x, light.lightColor.y, light.lightColor.z, light.lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), light.lightPos.x, light. lightPos.y, light.lightPos.z);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model") , 1, GL_FALSE, glm::value_ptr(Doormatrix));
	Cube cube;

	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.01f, 0.01f, 0.1f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		planevao.Bind();
		glDrawElements(GL_TRIANGLES, Indices.size()*3, GL_UNSIGNED_INT, nullptr);

		

		   // Check if the cube position lies within the current grid cell
		for (int i = 0; i < PlaneVertices.size() - size; i++)
		{
			if (cube.CubeMatrix[3].z >= PlaneVertices[i].z && cube.CubeMatrix[3].z <= PlaneVertices[i + 1].z &&
				cube.CubeMatrix[3].x >= PlaneVertices[i].x && cube.CubeMatrix[3].x <= PlaneVertices[i + size].x)
			{
				// Calculate barycentric coordinates
				vec3 barycentric = cube.barycentricCoordinates(vec2(PlaneVertices[i].x, PlaneVertices[i].z),
					vec2(PlaneVertices[i+1 ].x, PlaneVertices[i+1 ].z),
					vec2(PlaneVertices[i+size ].x, PlaneVertices[i+size].z),
					vec2(cube.CubeMatrix[3].x, cube.CubeMatrix[3].z));

				

				// Calculate interpolated y position
				float interpolatedY = PlaneVertices[i].y * barycentric.x +
					PlaneVertices[i + 1].y * barycentric.y +
					PlaneVertices[i + size].y * barycentric.z;


				//cout << "interpolation y: " << interpolatedY << endl;
				//cout << "cube y: "<<cube.CubeMatrix[3].y << endl;
				//cout << "light y: " << light.lightModel[3].y << endl;

				// Update the translation matrix of the cube with the interpolated y position
				cube.CubeMatrix = translate(cube.CubeMatrix, vec3(0, interpolatedY-cube.CubeMatrix[3].y, 0));

				// Break out of the loop once the cube position is found
				break;
				
			}
		
		}
		
	


		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		camera.Matrix(45.f, 0.1f, 100.f, shaderProgram, "camMatrix");
		camera.Inputs(window);
		//cube.CubeMatrix[3].y = barycentric.y;

	
		//Cube movement
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //left
		{
			cube.CubeMatrix = translate(cube.CubeMatrix, vec3(-0.1f, 0, 0));
			//Doormatrix = translate(Doormatrix, vec3(-1.f, 0, 0));
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //right
		{
			cube.CubeMatrix = translate(cube.CubeMatrix, vec3(0.1f, 0, 0));
			//Doormatrix = translate(Doormatrix, vec3(1.f, 0, 0));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //back
		{
			cube.CubeMatrix = translate(cube.CubeMatrix, vec3(0, 0, 0.1f));
			//Doormatrix = translate(Doormatrix, vec3(0.f, 0, 0.1f));
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //forward
		{
			cube.CubeMatrix = translate(cube.CubeMatrix, vec3(0, 0, -0.1f));
			//Doormatrix = translate(Doormatrix, vec3(-1.f, 0, -0.1f));
		}
	


		

		lightShader.Activate();
		camera.Matrix(45.f,0.1f,100.f,lightShader, "camMatrix");

		cube.DrawCube(vec3(0.2, 0.2, 0.2), vec3(0, 1, 0), shaderProgram, "model");
		light.CreateLight(vec3(1,1,1),vec3(1,1,1));
	

	
		

		
		
		glGetError();
		
		//Camera
		
		//Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}
	
	planevao.Delete();
	planevbo.Delete();
	planeebo.Delete();
	shaderProgram.Delete();
	lightShader.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;

}
