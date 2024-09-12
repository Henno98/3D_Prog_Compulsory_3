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
	Camera camera(width, height, glm::vec3((-0.1f), 5.f, (-0.1f)));
	Light light;
	// Shader for light cube
	Shader lightShader("Light.vert", "Light.frag");


	NPC npc;
	Trophy trophy;
	Cube cubeleft;
	Cube cuberight;
	Cube cubefront;
	Cube cubeback;
	Cube cubebottom;

	Sphere sphere;
	Sphere sphere_2;
	sphere.CreateSphere(2, 0.1f);
	sphere.SphereMatrix = translate(sphere.SphereMatrix, vec3(-1, 0, 0));
	sphere_2.CreateSphere(2, 0.1f);
	sphere_2.SphereMatrix = translate(sphere_2.SphereMatrix, vec3(1, 0, 0));
	

	trophy.TrophyMatrix = translate(trophy.TrophyMatrix, vec3(10, 2, 10));
	cubeleft.CubeMatrix = translate(cubeleft.CubeMatrix, vec3(-2, 0, 0));
	cuberight.CubeMatrix = translate(cuberight.CubeMatrix, vec3(2, 0, 0));
	cubefront.CubeMatrix = translate(cubefront.CubeMatrix, vec3(0, 0, 2));
	cubeback.CubeMatrix = translate(cubeback.CubeMatrix, vec3(0, 0, -2));
	cubebottom.CubeMatrix = translate(cubebottom.CubeMatrix, vec3(0, -0.2, 0));


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
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(Doormatrix));

		
		sphere.DrawSphere(shaderProgram,"model");
		sphere.Movement(vec3(0.01, 0, 0));
		sphere_2.DrawSphere(shaderProgram, "model");
		sphere_2.Movement(vec3(-0.01, 0, 0));

		if(sphere.AABB.TestAABBAABB(sphere_2.AABB))
		{
			sphere.CollideWithBall(sphere_2);

		}
		cubeleft.DrawCube(vec3(0.2, 0.4, 2), vec3(0, 1, 1), shaderProgram, "model");
		cuberight.DrawCube(vec3(0.2, 0.4, 2), vec3(0, 1, 1), shaderProgram, "model");
		cubefront.DrawCube(vec3(2, 0.4, 0.2), vec3(0, 1, 1), shaderProgram, "model");
		cubeback.DrawCube(vec3(2, 0.4, 0.2), vec3(0, 1, 1), shaderProgram, "model");
		cubebottom.DrawCube(vec3(2, 0.2, 2), vec3(1, 0, 1), shaderProgram, "model");


		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		camera.Matrix(45.f, 0.1f, 100.f, shaderProgram, "camMatrix");
		camera.Inputs(window);

		cout << npc.NPCMatrix[3].x << " " << npc.NPCMatrix[3].y << " " << npc.NPCMatrix[3].z << endl;

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


