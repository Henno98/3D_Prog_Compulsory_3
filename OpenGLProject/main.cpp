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
#include "../Models/Collision.h"
#include "Components/ComponentManager.h"
#include "Components/EntityManager.h"
#include "Components/PositionComponent.h"
#include "Functions/MovementSystem.h"
#include "Functions/SystemManager.h"
#include "Functions/ActorRenderingSystem.h"
#include "Functions/AttackCheck.h"
using namespace std;
using namespace Eigen;
using namespace glm;

// Window dimensions
const unsigned int width = 2000;
const unsigned int height = 1440;

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
	
	Light light;
	// Shader for light cube
	Shader lightShader("Light.vert", "Light.frag");
	srand(time(NULL));
	
	
	//Init the Components
	EntityManager EManager;
	ComponentManager<PositionComponent> PositionData;
	ComponentManager<MovementComponent> MovementData;
	ComponentManager<HealthComponent> HealthData;
	ComponentManager<DamageComponent> DamageData;
	ComponentManager<SizeComponent> SizeData;
	ComponentManager<CollisionComponent> CollisionData;
	ComponentManager<PickUpComponent> PickUpData;
	ComponentManager<InputComponent> InputData;
	Cube Box;
	//Init the Functions
	MovementSystem Movement;
	AttackCheck CollisionDetection;
	ActorRenderingSystem Render;

	//Vector Containing All Actors
	std::vector<Entity> AllEntities;

	//CreatePlayer
	Entity Player = EManager.CreateEntity();
	AllEntities.emplace_back(Player);
	PositionData.AddComponent(Player.GetId(), PositionComponent(glm::vec3(1.f)));
	MovementData.AddComponent(Player.GetId(), MovementComponent(glm::vec3(1.F),2.f,10.f));
	HealthData.AddComponent(Player.GetId(), HealthComponent(20));
	//DamageData.AddComponent(Player.GetId(), DamageComponent(5));
	CollisionData.AddComponent(Player.GetId(), CollisionComponent(true, vec3(1.f)));
	InputData.AddComponent(Player.GetId(), InputComponent(true));

	Camera camera(width, height, glm::vec3(1.f));

	//Create Pickup
	Entity HealthPickup = EManager.CreateEntity();
	AllEntities.emplace_back(HealthPickup);
	PositionData.AddComponent(HealthPickup.GetId(), PositionComponent(vec3(3.f, 0.f, 2.f)));
	CollisionData.AddComponent(HealthPickup.GetId(), CollisionComponent(true, vec3(1.f)));
	PickUpData.AddComponent(HealthPickup.GetId(), PickUpComponent(PickUpComponent::Type::Health, 10));

	//Making Enemies
	std::vector<Entity> Boars;
	for(int i = 0; i < 100 ; i++)
	{
		Entity Actor = EManager.CreateEntity();
		
		Boars.emplace_back(Actor);
		AllEntities.emplace_back(Actor);
	}
	for(int i = 0 ; i < Boars.size();i++)
	{
		float randx = rand() %10 -5;
		float randy = rand() %10 ;
		float randz = rand() %10 -5;
		float speed = rand() % 5+2;
		PositionData.AddComponent(Boars[i].GetId(), PositionComponent(glm::vec3(randx*5, randy*5, randy*5)));
		MovementData.AddComponent(Boars[i].GetId(), MovementComponent(glm::vec3(randx, randy, randz),speed,speed));
		HealthData.AddComponent(Boars[i].GetId(), HealthComponent(10));
		DamageData.AddComponent(Boars[i].GetId(), DamageComponent(1));
		CollisionData.AddComponent(Boars[i].GetId(), CollisionComponent(true,vec3(1,1,1)));
	}
	Render.InsertData(vec3(0.25f));
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	float lastFrame = 0.f;
	Render.initBinders();
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
		
		Render.DrawActor(shaderProgram, "model", PositionData, AllEntities);
		//Handles movement for input actors
		Movement.Update(Deltatime,camera.Position, PositionData, MovementData, Boars);
		//Box.DrawCube(vec3(100.f,-0.2f,100.f), vec3(1.f), shaderProgram, "model");
		//Movement.RecieveInput(MovementData, InputData, AllEntities, Deltatime, window);
		
		//Checks collision for all entities
		for(int i = 0; i < AllEntities.size();i++)
		{

			if(CollisionDetection.CheckifOverlap(CollisionData,PositionData, AllEntities[i].GetId()) )
			{
				CollisionDetection.Collision(MovementData, Boars[i].GetId());
			}

		}
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightColor"), light.lightColor.x, light.lightColor.y, light.lightColor.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), light.lightPos.x, light.lightPos.y, light.lightPos.z);
		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(45.f, 0.1f, 1000.f, shaderProgram, "camMatrix");
		camera.Inputs(window);

		lightShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(light.lightModel));
		glUniform3f(glGetUniformLocation(lightShader.ID, "lightColor"), light.lightColor.x, light.lightColor.y, light.lightColor.z);
		camera.Matrix(45.f, 0.1f, 1000.f, lightShader, "camMatrix");
		light.CreateLight(vec3(1, 1, 1), vec3(1, 1, 1));


		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //left
		{
			
			camera.Position.x += 15 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //right
		{
			
			camera.Position.x += -15 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //back
		{
			
			camera.Position.z += 15 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //forward
		{
			
			camera.Position.z += -15 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) //back
		{

			camera.Position.y += 15 * Deltatime;

		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) //forward
		{

			camera.Position.y += -15 * Deltatime;

		}
		glGetError();

		//Camera

		//Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}
	Render.DeleteBinders();
	shaderProgram.Delete();
	lightShader.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;

}


