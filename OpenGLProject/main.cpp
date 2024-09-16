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
#include "../Models/NPC.h"
#include <Eigen/Dense>

#include "Shaders/ShaderClass.h"
#include "Shaders/VAO.h"
#include "Shaders/VBO.h"
#include "Shaders/EBO.h"
#include "Camera.h"
#include "Models/Cube.h"
#include "Shaders/Light.h"
#include "Models/Sphere.h"
#include "Models/Trophy.h"




using namespace std;
using namespace Eigen;
using namespace glm;

// Window dimensions
const unsigned int width = 1200;
const unsigned int height = 1200;

template <typename T>
struct Bezier
{
	T p0, p1, p2, p3;

	Bezier() : p0(T()), p1(T()), p2(T()), p3(T())
	{
	}

	Bezier(T p0, T p1, T p2, T p3) : p0(p0), p1(p1), p2(p2), p3(p3)
	{
	}

	T operator()(const float t)
	{
		const auto u = 1 - t;
		const auto tp0 = powf(u, 3) * p0;
		const auto tp1 = 3 * powf(u, 2) * t * p1;
		const auto tp2 = 3 * u * powf(t, 2) * p2;
		const auto tp3 = powf(t, 3) * p3;
		return tp0 + tp1 + tp2 + tp3;
	}
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
	Camera camera(width, height, glm::vec3(0.f));
	Light light;
	// Shader for light cube
	Shader lightShader("Light.vert", "Light.frag");


	vector<Cube> Board;
	for(int i = 0; i < 5;i++)
	{
		Cube cube;
		Board.emplace_back(cube);

	}
	
	
	srand(time(NULL));
	vector<Sphere> Spherelist;
	for(int i = 0; i < 4;i++)
	{
		float xdir = -1.f + rand() % 2;
		float zdir = -1.f + rand() % 2;
		Sphere sphere;
		sphere.SphereMatrix = translate(sphere.SphereMatrix, vec3(i*2 , 0, zdir ));
		sphere.CreateSphere(2, 2.f, vec3(xdir, 0, zdir));
	//	sphere.SphereMatrix = translate(sphere.SphereMatrix, vec3(i, 0, i));
		Spherelist.emplace_back(sphere);

	}
	
	/*Sphere sphere;
	Sphere sphere_2;
	sphere.CreateSphere(2, 1.f,vec3(0.01,0,0));
	sphere.SphereMatrix = translate(sphere.SphereMatrix, vec3(-2, 0, -0.5));
	sphere_2.CreateSphere(2, 0.5f,vec3(-0.01,0,0));
	sphere_2.SphereMatrix = translate(sphere_2.SphereMatrix, vec3(2, 0, 0.05));
	*/

	
	Board[0].CubeMatrix = translate(Board[0].CubeMatrix, vec3(-25, 0, 0));
	Board[1].CubeMatrix = translate(Board[1].CubeMatrix, vec3(25, 0, 0));
	Board[2].CubeMatrix = translate(Board[2].CubeMatrix, vec3(0, 0, 25));
	Board[3].CubeMatrix = translate(Board[3].CubeMatrix, vec3(0, 0, -25));
	Board[4].CubeMatrix = translate(Board[4].CubeMatrix, vec3(0, -6, 0));


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	float lastFrame = 0.f;

//	Doormatrix = translate(Doormatrix, vec3(-5, 0, -5));
	while (!glfwWindowShouldClose(window))
	{

		//Gets the current frame
		float currentFrame = static_cast<float>(glfwGetTime());
		float Deltatime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Specify the color of the background
		glClearColor(0.01f, 0.01f, 0.1f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use


		shaderProgram.Activate();
		
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightColor"), light.lightColor.x, light.lightColor.y, light.lightColor.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), light.lightPos.x, light.lightPos.y, light.lightPos.z);
		
		/*
		sphere.DrawSphere(shaderProgram,"model");
		sphere.Movement();
		sphere_2.DrawSphere(shaderProgram, "model");
		sphere_2.Movement();*/

		Board[0].DrawCube(vec3(2, 4, 26), vec3(0, 1, 1), shaderProgram, "model");
		Board[1].DrawCube(vec3(2, 4, 26), vec3(0, 1, 1), shaderProgram, "model");
		Board[2].DrawCube(vec3(26, 4, 2), vec3(0, 1, 1), shaderProgram, "model");
		Board[3].DrawCube(vec3(26, 4, 2), vec3(0, 1, 1), shaderProgram, "model");
		Board[4].DrawCube(vec3(25, 2, 25), vec3(1, 0, 1), shaderProgram, "model");

		for (int i = 0; i < Spherelist.size();i++)
		{
			Spherelist[i].Movement();
			Spherelist[i].DrawSphere(shaderProgram, "model");
			cout << Spherelist[i].SphereMatrix[3].x  <<" " << Spherelist[i].SphereMatrix[3].z << endl;

		}
		for (int i = 0; i < Spherelist.size(); i++)
		{


			for (int k = 0; k<Board.size();k++)
			{
				if(Spherelist[i].AABB.TestAABBAABB(Board[k].AABB))
				{

					Spherelist[i].CollideWithWall(Board[k]);
					cout << i << " collides with wall " << k << endl;
					break;
				}

			}
			for (int j = i + 1; j < Spherelist.size(); j++)
			{
				if (Spherelist[i].AABB.TestAABBAABB(Spherelist[j].AABB))
				{
					
					Spherelist[i].CollideWithBall(Spherelist[j]);
					cout << i << " collides with " << j << endl;
					break;
				}
				
			}
			
		}
		
	


		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		camera.Matrix(45.f, 0.1f, 100.f, shaderProgram, "camMatrix");
		camera.Inputs(window);

		lightShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(light.lightModel));
		glUniform3f(glGetUniformLocation(lightShader.ID, "lightColor"), light.lightColor.x, light.lightColor.y, light.lightColor.z);
		camera.Matrix(45.f, 0.1f, 100.f, lightShader, "camMatrix");
		light.CreateLight(vec3(1, 1, 1), vec3(1, 1, 1));


		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //left
		{
			
			camera.Position.x += 5 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //right
		{
			
			camera.Position.x += -5 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //back
		{
			
			camera.Position.z += 5 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //forward
		{
			
			camera.Position.z += -5 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) //back
		{

			camera.Position.y += 5 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) //forward
		{

			camera.Position.y += -5 * Deltatime;

		}

		
		

		glGetError();

		//Camera

		//Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	shaderProgram.Delete();
	lightShader.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;

}


