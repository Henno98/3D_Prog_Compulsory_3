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

#include "Shaders/ShaderClass.h"
#include "Shaders/VAO.h"
#include "Shaders/VBO.h"
#include "Shaders/EBO.h"
#include "Camera.h"
#include "Cube.h"
#include "Shaders/Light.h"
#include "Sphere.h"
#include "Trophy.h"




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


struct Planevertex
{

	GLfloat x, y, z;
	float r, g, b;
	float n1, n2, n3;



};

struct Triangle
{
	GLuint v0, v1, v2;

};



float curveplane(float x, float y)
{

	return cos(x) + sin(y) - cos(y);
}



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

	Camera camera(width, height, glm::vec3((-0.1f), 5.f, (-0.1f)));




	mat4 Doormatrix = mat4(1.0f);
	


	vector<Planevertex> PlaneVertices;
	vector<Triangle> Indices;
	vector<Triangle> QuadIndices;

	float step = 0.5f;
	float size = 40;
	for (float i = 0; i < size; i += step)
	{
		for (float j = 0; j < size; j += step) {

			PlaneVertices.emplace_back(Planevertex{ i, curveplane(i,j),j,0,1,0,i/size,0,j/size });

		}


	}
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - 1; j++) {
			unsigned int v0 = i * (size / step) + j;
			unsigned int v1 = v0 + 1;
			unsigned int v2 = v0 + (size / step);
			unsigned int v3 = v2 + 1;

			Indices.emplace_back(Triangle{ v0, v1, v2 });
			Indices.emplace_back(Triangle{ v1, v2, v3 });
		}
	}



	Light light;


	VAO planevao;
	planevao.Bind();
	VBO planevbo(reinterpret_cast<GLfloat*>(PlaneVertices.data()), (PlaneVertices.size() * sizeof(Planevertex)));
	planevbo.Bind();
	EBO planeebo(reinterpret_cast<GLuint*>(Indices.data()), (Indices.size() * sizeof(Triangle)));
	planeebo.Bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	planevao.Unbind();
	planevbo.Unbind();
	planeebo.Unbind();




	//

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
	sphere.CreateSphere(5, 2);

	vec3 pos1 = vec3(1, 0, 1);
	vec3 pos2 = vec3(5, 0, 6);
	vec3 pos3 = vec3(15, 0, 17);
	vec3 pos4 = vec3(20, 0, 5);
	auto Bez = Bezier<vec3>(pos1,pos2,pos3,pos4);
	float t = 0.f;

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

		t = fmod(t + Deltatime, 1.0f);

		shaderProgram.Activate();
		//npc.DrawNPC(shaderProgram, "model");

		//npc.NPCMatrix[3] = vec4(Bez(t), 1);
		

		//trophy.DrawTrophy(vec3(1,1,1),shaderProgram,"model");
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightColor"), light.lightColor.x, light.lightColor.y, light.lightColor.z);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), light.lightPos.x, light.lightPos.y, light.lightPos.z);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(Doormatrix));

		//planevao.Bind();
		//glDrawElements(GL_TRIANGLES, Indices.size() * 3, GL_UNSIGNED_INT, nullptr);
		//glDrawElements(GL_TRIANGLES, QuadIndices.size() * 3, GL_UNSIGNED_INT, nullptr);

		sphere.DrawSphere(shaderProgram,"model");
		cubeleft.DrawCube(vec3(0.2, 0.4, 2), vec3(0, 1, 1), shaderProgram, "model");
		cuberight.DrawCube(vec3(0.2, 0.4, 2), vec3(0, 1, 1), shaderProgram, "model");
		cubefront.DrawCube(vec3(2, 0.4, 0.2), vec3(0, 1, 1), shaderProgram, "model");
		cubeback.DrawCube(vec3(2, 0.4, 0.2), vec3(0, 1, 1), shaderProgram, "model");
		cubebottom.DrawCube(vec3(2, 0.2, 2), vec3(1, 0, 1), shaderProgram, "model");


		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		camera.Matrix(45.f, 0.1f, 100.f, shaderProgram, "camMatrix");
		camera.Inputs(window);
		//cube.CubeMatrix[3].y = barycentric.y;




		// Check if the cube position lies within the current grid cell
		for (int i = 0; i < Indices.size(); i++)
		{

			//unsigned int Index0 = Indices[i].v0;
			//unsigned int Index1 = Indices[i].v1;
			//unsigned int Index2 = Indices[i].v2;
			//unsigned int Index3 = Indices[i].v2 + 1;





			//// Calculate barycentric coordinates
			//vec3 barycentric = cube.barycentricCoordinates(vec3(PlaneVertices[Index0].x, PlaneVertices[Index0].y, PlaneVertices[Index0].z),
			//	vec3(PlaneVertices[Index1].x, PlaneVertices[Index1].y, PlaneVertices[Index1].z),
			//	vec3(PlaneVertices[Index2].x, PlaneVertices[Index2].y, PlaneVertices[Index2].z),
			//	vec3(cube.CubeMatrix[3].x, cube.CubeMatrix[3].y, cube.CubeMatrix[3].z));

			//// Calculate barycentric coordinates
			//vec3 barycentric2 = npc.barycentricCoordinates(vec3(PlaneVertices[Index0].x, PlaneVertices[Index0].y, PlaneVertices[Index0].z),
			//	vec3(PlaneVertices[Index1].x, PlaneVertices[Index1].y, PlaneVertices[Index1].z),
			//	vec3(PlaneVertices[Index2].x, PlaneVertices[Index2].y, PlaneVertices[Index2].z),
			//	vec3(npc.NPCMatrix[3].x,npc.NPCMatrix[3].y,npc.NPCMatrix[3].z));

			//// Calculate barycentric coordinates
			//vec3 barycentric3 = trophy.barycentricCoordinates(vec3(PlaneVertices[Index0].x, PlaneVertices[Index0].y, PlaneVertices[Index0].z),
			//	vec3(PlaneVertices[Index1].x, PlaneVertices[Index1].y, PlaneVertices[Index1].z),
			//	vec3(PlaneVertices[Index2].x, PlaneVertices[Index2].y, PlaneVertices[Index2].z),
			//	vec3(trophy.TrophyMatrix[3].x, trophy.TrophyMatrix[3].y, trophy.TrophyMatrix[3].z));




			//if (barycentric.x < 1 && barycentric.x > 0 && barycentric.y < 1 && barycentric.y > 0 && barycentric.z < 1 && barycentric.z > 0) {

			//	// Calculate interpolated y position
			//	float interpolatedX = (PlaneVertices[Index0].y * barycentric.x);
			//	float InterpolatedY = (PlaneVertices[Index1].y * barycentric.y);
			//	float InterpolatedZ = (PlaneVertices[Index2].y * barycentric.z);
			//	float InterpolatedPos = interpolatedX + InterpolatedY + InterpolatedZ;
			//	// Update the translation matrix of the cube with the interpolated y position
			//	cube.CubeMatrix[3].y = InterpolatedPos;
			//	//cout << InterpolatedPos << endl;
			//	


			//}
			//if(barycentric2.x < 1 && barycentric2.x > 0 && barycentric2.y < 1 && barycentric2.y > 0 && barycentric2.z < 1 && barycentric2.z > 0)
			//{
			//	// Calculate interpolated y position
			//	float interpolatedX = (PlaneVertices[Index0].y * barycentric2.x);
			//	float InterpolatedY = (PlaneVertices[Index1].y * barycentric2.y);
			//	float InterpolatedZ = (PlaneVertices[Index2].y * barycentric2.z);
			//	float InterpolatedPos = interpolatedX + InterpolatedY + InterpolatedZ;
			//	// Update the translation matrix of the cube with the interpolated y position
			//	npc.NPCMatrix[3].y = InterpolatedPos;
			//	//cout << InterpolatedPos << endl;
			//


			//}
			//if (barycentric3.x < 1 && barycentric3.x > 0 && barycentric3.y < 1 && barycentric3.y > 0 && barycentric3.z < 1 && barycentric3.z > 0)
			//{
			//	// Calculate interpolated y position
			//	float interpolatedX = (PlaneVertices[Index0].y * barycentric3.x);
			//	float InterpolatedY = (PlaneVertices[Index1].y * barycentric3.y);
			//	float InterpolatedZ = (PlaneVertices[Index2].y * barycentric3.z);
			//	float InterpolatedPos = interpolatedX + InterpolatedY + InterpolatedZ;
			//	// Update the translation matrix of the cube with the interpolated y position
			//	trophy.TrophyMatrix[3].y = InterpolatedPos;
			//	//cout << InterpolatedPos << endl;



			//}


		}

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


		//cout << "cube y: " << cube.CubeMatrix[3].y << endl;

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


