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
#include "Wall.h"
using namespace std;
using namespace Eigen;
using namespace glm;

// Window dimensions
const unsigned int width = 1800;
const unsigned int height = 1800;


struct Planevertex
{

	GLfloat x,y,z;
	float r, g, b;
	vec3 Normal;
	
	

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
vec3 barycentricCoordinates(const vec2& p1, const vec2& p2, const vec2& p3, const vec2& p4)
{
	vec2 p12 = p2 - p1;
	vec2 p13 = p3 - p1;
	float areal_123 = abs(p12.x * p13.y - p12.y * p13.x); // double the area

	vec3 baryc; // for return

	// u
	vec2 p = p2 - p4;
	vec2 q = p3 - p4;
	float nu = abs(p.x * q.y - p.y * q.x); // double the area of p4pq
	baryc.x = nu / areal_123;

	// v
	p = p3 - p4;
	q = p1 - p4;
	float nv = abs(p.x * q.y - p.y * q.x); // double the area of p4pq
	baryc.y = nv / areal_123;

	// w
	p = p1 - p4;
	q = p2 - p4;
	float nw = abs(p.x * q.y - p.y * q.x); // double the area of p4pq
	baryc.z = nw / areal_123;
	
		return baryc;
	
}

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};

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
	//Cube cube;
//	NPC npc;

	Camera camera(width, height, vec3(glm::vec3((-0.1f ), 1.f, (-0.1f ) + 5)));
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);



	mat4 Doormatrix = mat4(1.0f);
	Doormatrix = translate(Doormatrix, vec3(0, 0, 0));
	

	vector<Planevertex> PlaneVertices;
	vector<Triangle> Indices;
	
	
	float size = 25;
	for (float i = 0; i < size; i++)
	{
		for (float j = 0; j < size; j++) {

			PlaneVertices.emplace_back(Planevertex{ i, curveplane(i,j),j,0,0,1,vec3(i/size,j/size,0)});
		}
		

	}
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - 1; j++) {
			unsigned int v0 = i * size + j;
			unsigned int v1 = v0 + 1;
			unsigned int v2 = v0 + size;
			unsigned int v3 = v2 + 1;

			Indices.emplace_back(Triangle{ v0, v2, v1 });
			Indices.emplace_back(Triangle{ v1, v2, v3 });
		}
	}
//	cube.CubeMatrix = translate(cube.CubeMatrix, vec3(5, 0, 5));
	float relativeX = 0.0f, relativeZ = 0.0f;



	VAO planevao;
	planevao.Bind();
	VBO planevbo(reinterpret_cast<GLfloat*>(PlaneVertices.data()), static_cast<GLsizeiptr>(PlaneVertices.size() * sizeof(Planevertex)));
	planevbo.Bind();
	EBO planeebo(reinterpret_cast<GLuint*>(Indices.data()), static_cast<GLsizeiptr>(Indices.size() * sizeof(Triangle)));
	planeebo.Bind();
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);

	int modelloc = glGetUniformLocation(shaderProgram.ID, "model");
	glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(Doormatrix));
	

	planevao.Unbind();
	planevbo.Unbind();
	planeebo.Unbind();

	VAO VAO1;
	VAO1.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));
	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.01f, 0.01f, 0.1f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();


		//   // Check if the cube position lies within the current grid cell
		//for (int i = 0; i < PlaneVertices.size() - size; i++)
		//{
		//	if (cube.CubeMatrix[3].z >= PlaneVertices[i].z && cube.CubeMatrix[3].z <= PlaneVertices[i + 1].z &&
		//		cube.CubeMatrix[3].x >= PlaneVertices[i].x && cube.CubeMatrix[3].x <= PlaneVertices[i + size].x)
		//	{
		//		// Calculate barycentric coordinates
		//		vec3 barycentric = barycentricCoordinates(vec2(PlaneVertices[i].x, PlaneVertices[i].z),
		//			vec2(PlaneVertices[i+1 ].x, PlaneVertices[i+1 ].z),
		//			vec2(PlaneVertices[i+size ].x, PlaneVertices[i+size].z),
		//			vec2(cube.CubeMatrix[3].x, cube.CubeMatrix[3].z));

		//		// Calculate interpolated y position
		//		float interpolatedY = PlaneVertices[i].y * barycentric.x +
		//			PlaneVertices[i + 1].y * barycentric.y +
		//			PlaneVertices[i + size].y * barycentric.z;

		//		cout << "interolation y: " << interpolatedY << endl;
		//		cout << "cube y: "<<cube.CubeMatrix[3].y << endl;

		//		// Update the translation matrix of the cube with the interpolated y position
		//		cube.CubeMatrix[3][1] = interpolatedY +0.2f ;

		//		// Break out of the loop once the cube position is found
		//		break;
		//		
		//	}
		//
		//}
		
		//cube.DrawCube(vec3(0.2, 0.2, 0.2), vec3(0, 1, 0), shaderProgram, "model");


		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		camera.Matrix(45.f, 0.1f, 100.f, shaderProgram, "camMatrix");
		camera.Inputs(window);
		//cube.CubeMatrix[3].y = barycentric.y;

	
		////Cube movement
		//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //left
		//{
		//	cube.CubeMatrix = translate(cube.CubeMatrix, vec3(-0.01f, 0, 0));
		//}
		//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //right
		//{
		//	cube.CubeMatrix = translate(cube.CubeMatrix, vec3(0.01f, 0, 0));
		//}
		//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //back
		//{
		//	cube.CubeMatrix = translate(cube.CubeMatrix, vec3(0, 0, 0.01f));
		//}
		//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //forward
		//{
		//	cube.CubeMatrix = translate(cube.CubeMatrix, vec3(0, 0, -0.01f));
		//}
	

		planevao.Bind();
		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);

		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		lightShader.Activate();
		camera.Matrix(45.f,0.1f,100.f,lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
	

	
		

		
		
		glGetError();
		
		//Camera
		
		//Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	planevao.Delete();
	planevbo.Delete();
	planeebo.Delete();
	shaderProgram.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;

}
