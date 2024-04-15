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
const unsigned int width = 1600;
const unsigned int height = 1200;

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
	

	Camera camera(width, height, vec3(glm::vec3((-0.1f * MovementX), 1.f, (-0.1f * MovementZ) + 5)));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glm::mat4 Doormatrix = mat4(1.0f);
	Doormatrix = translate(Doormatrix, vec3(-1.2, 0, -2.f));
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
	 
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.01f, 0.01f, 0.1f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		
		if (changefunction) {
			
			pos1 = vec3(npc.NPCMatrix[3]);
			pos2 = vec3(-10.f, 0.f, -10.f);
			pos3 = vec3(20.f, 0.f, 40.f);
			pos4 = vec3(-3.f, 0.f, 11.f);
		}
		if (!changefunction)
		{
			pos1 = vec3(-2.f,0,12.f);
			pos2 = vec3(16.f, 0.f, 10.f);
			pos3 = vec3(-14.f, 0.f, -30.f);
			pos4 = vec3(npc.NPCMatrix[3]);

		}

		double x0 = pos1.x;
		double x1 = pow(pos1.x, 3);
		double x2 = pow(pos1.x, 2);

		double x02 = pos2.x;
		double x12 = pow(pos2.x, 3);
		double x22 = pow(pos2.x, 2);

		double x03 = pos3.x;
		double x13 = pow(pos3.x, 3);
		double x23 = pow(pos3.x, 2);

		double x04 = pos4.x;
		double x14 = pow(pos4.x, 3);
		double x24 = pow(pos4.x, 2);

		AMatrix <<
			x1, x2, x0, 1.0,
			x12, x22, x02, 1.0,
			x13, x23, x03, 1.0,
			x14, x24, x04, 1.0;

		YMatrix <<
			pos1.z,
			pos2.z,
			pos3.z,
			pos4.z;

		MatrixXd AInverse = AMatrix.inverse();
		MatrixXd SolutionMatrix = AInverse * YMatrix;

		//std::cout << "Matrix A Answer: " << AMatrix << std::endl;
		//std::cout << "Matrix Y Answer: " << YMatrix << std::endl;
		std::cout << "Matrix A-1 Answer: " << AInverse << std::endl;
		std::cout << "Matrix Answer: " << SolutionMatrix << std::endl;
		

		//Cube movement
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //left
		{
			MovementX += speed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //right
		{
			MovementX -= speed;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //back
		{
			MovementZ -= speed;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //forward
		{
			MovementZ += speed;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) //forward
		{
			
			if(changefunction == true)
			{
				changefunction = false;
				
			}
			else if (changefunction == false)
			{
				changefunction = true;
				
				
			}
		}

		//Cube
		cube.CreateCube(glm::vec3(-0.1f * MovementX, 0, -0.1f * MovementZ), glm::vec3(0.2f,0.2f,0.2f), shaderProgram, "model");

		cube2.CreateCube(glm::vec3(1, 0, 1), glm::vec3(1, 0.5f, 1), shaderProgram, "model");

		walls.CreateHouse(glm::vec3(-2,0,-2),glm::vec3(2,1,2));
	//	Door.CreateXWall(vec3(-1.6f, 0.f, -2.f), vec3(0.4, 0.6, 0), vec3(1.f, 1.f, 1.f), vec3(1.f, 1.f, 1.f));

		npc.DrawNPC(shaderProgram, "model");
		
		

		float z = (SolutionMatrix(0, 0) * pow(x, 3)) + (SolutionMatrix(1, 0) * pow(x, 2)) + (SolutionMatrix(2, 0) * SolutionMatrix(3, 0));
		//std::cout << "for X value: " << x << " Z value is: " << z << std::endl;

		if (!turn == true) {
			npc.NPCMatrix = translate(npc.NPCMatrix, vec3(x / 1000, 0, z / 1000));
			x += 0.1f;
			if (x >= 10)
			{
				turn = true;
			}
			
		}
		if(!turn==false)
		{
			npc.NPCMatrix = translate(npc.NPCMatrix, vec3(-x / 1000, 0, -z / 1000));
			x -= 0.1f;
			if(x <=0)
			{
					turn = false;
			}
		
		}

			/*npc.NPCMatrix = translate(npc.NPCMatrix, vec3(-x / 1000, 0, -z / 1000));
			x -= 1;*/
		
		
			
			//npc.NPCMatrix = glm::translate(npc.NPCMatrix, vec3(i, 0, z / 1000));

		
			//Trophy
		trophy.DrawTrophy(vec3(1, .2f, -2), vec3(0.2f, .2f, .2f), shaderProgram, "model");
		
		trophy2.DrawTrophy(vec3(-2, .2f, -3), vec3(0.2f, .2f, .2f), shaderProgram, "model");
		
		trophy3.DrawTrophy(vec3(1, .2f, 0), vec3(0.2f, .2f, .2f), shaderProgram, "model");
		
		trophy4.DrawTrophy(vec3(-1, .2f, 1), vec3(0.2f, .2f, .2f), shaderProgram, "model");
		
		trophy5.DrawTrophy(vec3(-1, .2f, 2), vec3(0.2f, .2f, .2f), shaderProgram, "model");
		
		trophy6.DrawTrophy(vec3(-1, .2f, 3), vec3(0.2f, .2f, .2f), shaderProgram, "model");
		

		float Degree;
		if ( cube.AABB.TestAABBAABB(walls.AABB) )
		{
			walls.WallColor = glm::vec3(0.f, 1.f, 1.f);
			camera.UpdateCamera(glm::vec3(-2.f, 1.0f, -4.f), vec3(1.f, -1.f, 2.f));
		

			if (cube.AABB.TestAABBAABB(walls.AABB) && Doorisclosed == true)
			{
				
				
				//Doormatrix = translate(Doormatrix, vec3(-1.6, 0, -2));
				Doormatrix = rotate(Doormatrix, radians(-110.0f), vec3(0.f, 1.0f, 0.f));
			
				Doorisclosed = false;

			}
			
		}
		else if (!cube.AABB.TestAABBAABB(walls.AABB))
		{
			walls.WallColor = glm::vec3(1.f, 0, 1.f);
			walls.doorPosition = glm::vec3(-2+walls.doorWidth, 0, -2);
			camera.UpdateCamera(glm::vec3((-0.1f * MovementX), 1.f, (-0.1f * MovementZ) + 5), vec3(0.f, 0.f, -2.f));
			npc.Color = glm::vec3(1, 0, 1);
			if (!cube.AABB.TestAABBAABB(walls.AABB) && Doorisclosed == false)
			{


				Doormatrix = rotate(Doormatrix, radians(110.0f), vec3(0.f, 0.1f, 0.f));
				
				Doorisclosed = true;

			}
		}
		
		//Trophy collision (pickups)
		if (cube.AABB.TestAABBAABB(trophy.AABB))
		{
			trophy.DestroyTrophy();
			cube.collectedtrophies += 1;

		}
		if (cube.AABB.TestAABBAABB(trophy2.AABB))
		{
			trophy2.DestroyTrophy();
			cube.collectedtrophies += 1;
		}
		if (cube.AABB.TestAABBAABB(trophy3.AABB))
		{
			
			trophy3.DestroyTrophy();
			cube.collectedtrophies += 1;
		}
		else if (cube.AABB.TestAABBAABB(trophy4.AABB))
		{
			trophy4.DestroyTrophy();
			cube.collectedtrophies += 1;

		}
		else if (cube.AABB.TestAABBAABB(trophy5.AABB))
		{
			trophy5.DestroyTrophy();
			cube.collectedtrophies += 1;
		}
		else if (cube.AABB.TestAABBAABB(trophy6.AABB))
		{
			
			trophy6.DestroyTrophy();
			cube.collectedtrophies += 1;
		}


		
		

		VAO doorvao;
		doorvao.Bind();
		VBO doorvbo(Doorvertices,sizeof(Doorvertices));
		doorvbo.Bind();
		EBO doorebo(doorindices, sizeof(doorindices));
		doorebo.Bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1); 

		int modelloc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(Doormatrix));

		glDrawElements(GL_TRIANGLES, sizeof(doorindices), GL_UNSIGNED_INT, nullptr);

		doorvao.Unbind();
		doorvbo.Unbind();
		doorebo.Unbind();

		doorvao.Delete();
		doorvbo.Delete();
		doorebo.Delete();
		
	
		
		//Camera
		Degree = 45.f;
		camera.Inputs(window);
		camera.Matrix(Degree, .1f, 100.0f, shaderProgram, "camMatrix");
		
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
