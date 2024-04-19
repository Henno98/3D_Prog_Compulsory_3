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

GLfloat Doorvertices[] =
{	//extent
	//0.4, 0.6, 0
	//original position
	0.f, 0.f, 0.f,0,0,0,
	-0.4f,0.f,0.f,1,1,1,
	-0.4f,0.6f,0.f,0,0,0,
	0.f,0.6f,0.f,1,1,1,

};
GLuint doorindices[] =
{
	0,1,2,
	0,2,3

};
struct Planevertex
{

	GLfloat x,y,z;
	float r, g, b;
	//vec3 position = vec3(x, y, z);
	

};

struct Triangle
{
	GLuint v0,v1,v2;

};

float curveplane(float x, float y)
{

	return cos(x)+ sin(y);
}
//vec3 barycentricCoordinates(const vec3& p1, const vec3& p2, const vec3& p3,const vec3& p4)
//{
//	vec3 p12 = p2 - p1;
//	vec3 p13 = p3 - p1;
//	vec3 n =p12 * p13;
//	float areal_123 = n.length(); // dobbelt areal
//	vec3 baryc; // til retur. Husk
//	// u
//	vec3 p = p2 - p4;
//	vec3 q = p3 - p4;
//	n = p * q;
//	baryc.x = n.z / areal_123;
//	// v
//	p = p3 - p4;
//	q = p1 - p4;
//	n = p ^ q;
//	baryc.y = n.z / areal_123;
//	// w
//	p = p1 - p4;
//	q = p2 - p4;
//	n = p ^ q;
//	baryc.z = n.z / areal_123;
//	return baryc;
//}

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

	//// Define the dimensions of the door
	//float doorWidth = 0.4f; // Width of the door
	//float doorHeight = 0.6f; // Height of the door

	//// Define the position of the door
	glm::vec3 doorPosition = glm::vec3(-2.0f, 0.0f, -1.0f);

	//// Define the dimensions of the house
	//float houseWidth = doorWidth * 5; // Width of the house
	//float houseHeight = doorHeight + doorHeight / 1; // Height of the house

	//	//Colors
	//glm::vec3 WallColor = glm::vec3(0.2f, 0.3f, 0.2f);
	//glm::vec3 WallColor1 = glm::vec3(0.2f, 0.31f, 0.2f);
	//glm::vec3 DoorColor = glm::vec3(0.6f, 0.3f, 0.1f);
	//glm::vec3 DoorColor1 = glm::vec3(0.61f, 0.31f, 0.11f);

	Cube cube;
	Cube cube2;
	Wall walls;
	Wall Door;
	NPC npc;
	Trophy trophy;
	//trophy.TrophyMatrix = translate(trophy.TrophyMatrix, vec3(-1, 0.2, 1));
	Trophy trophy2;
	Trophy trophy3;
	Trophy trophy4;
	Trophy trophy5;
	Trophy trophy6;



	float speed = 0.1f;
	float MovementX{0};
	float MovementZ{ 0 };
	

	Camera camera(width, height, vec3(glm::vec3((-0.1f ), 1.f, (-0.1f ) + 5)));
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glm::mat4 Doormatrix = mat4(1.0f);
	Doormatrix = translate(Doormatrix, vec3(0, 0, 0));
	bool Doorisclosed = true;

	float x = 0;
	//Calculates npc movement

	MatrixXd AMatrix(4, 4);
	MatrixXd YMatrix(4, 1);
	bool changefunction = false;

	vec3 pos1 = vec3(0.f);
	vec3 pos2 = vec3(12.f);
	vec3 pos3 = vec3(14.f);
	vec3 pos4 = vec3(11.f);
	
	bool turn = false;
	vec3 npcstartpos = vec3(-4,0,-2);
	npc.NPCMatrix = translate(npc.NPCMatrix, npcstartpos);
	// Main while loop
	cube.CubeMatrix = translate(cube.CubeMatrix, vec3(0.5, 0, 0.5));
	vector<Planevertex> PlaneVertices;
	vector<Triangle> Indices;
	
	float xval=0.f;
	float yval=0.f;
	int size = 12;
	for (float i = 0; i < size; i++)
	{
		//xval++;
		for (float j = 0; j < size; j++) {
			//yval ++;
			
			cout << i << " " << j << " " <<
				curveplane(i, j)  << endl;

			
			PlaneVertices.emplace_back(Planevertex{ i, curveplane(i,j),j,0,0,1});
		}
		

	}

	for(int k=0;k<(size*size);k++)
	{
		
		unsigned int v0 = k ;
		unsigned int v1 = k + size;
		unsigned int v2 = k + 1;
		unsigned int v3 = k + size + 1;
		if (v2 < PlaneVertices.size()) {
			Indices.emplace_back(Triangle{ v0,v1,v2 });
			if (v3 < PlaneVertices.size())
			{
				Indices.emplace_back(Triangle{ v1,v2,v3 });
			}
		}
	}
	
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.01f, 0.01f, 0.1f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		for (int i = 0; i + size< size*size;i++)
		{
			if (i < PlaneVertices.size()) {
				if (cube.CubeMatrix[3].z >= PlaneVertices[i].z && cube.CubeMatrix[3].z <= PlaneVertices[i + 1].z)
				{
					if (cube.CubeMatrix[3].x >= PlaneVertices[i].x && cube.CubeMatrix[3].x <= PlaneVertices[i + size].x)
					{
						cout << "box is inside triangle: " << i << endl;
						PlaneVertices[i + size].r = 1;
						PlaneVertices[i].r = 1;
						PlaneVertices[i + 1].r = 1;

						
					}
					

				}
				
					
				
			}
		
		}
		
		
		camera.Matrix(45.f, 0.1f, 100.f, shaderProgram, "camMatrix");
		camera.Inputs(window);
		

		cube.CreateCube(vec3(0, 1, 0), vec3(1, 1, 1), shaderProgram, "model");
		
		


		VAO doorvao;
		doorvao.Bind();
		VBO doorvbo(reinterpret_cast<GLfloat*>(PlaneVertices.data()), static_cast<GLsizeiptr>(PlaneVertices.size() * sizeof(Planevertex)));
		doorvbo.Bind();
		EBO doorebo(reinterpret_cast<GLuint*>(Indices.data()), static_cast<GLsizeiptr>(Indices.size() * sizeof(Triangle)));
		doorebo.Bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1); 

		int modelloc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(Doormatrix));

		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);

		doorvao.Unbind();
		doorvbo.Unbind();
		doorebo.Unbind();

		doorvao.Delete();
		doorvbo.Delete();
		doorebo.Delete();
		
	
		
		//Camera
		
		//Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}
	shaderProgram.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;

}
