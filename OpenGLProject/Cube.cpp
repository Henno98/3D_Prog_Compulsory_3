#include "Cube.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>



unsigned int CubeIndices[] =
{
	0,1,2,  // Front face
	3,1,2,

	7,5,4,  // Back face
	6,4,5,

	4,2,6,  // Top face
	3,6,2,

	7,0,5, // Bottom face
	1,5,0,

	2,7,0, // Left face
	2,4,7,

	5,3,1,  // Right face
	5,3,6
};

Cube::Cube()
{
	collectedtrophies = 0;
};

void Cube::DrawCube(std::vector<CubeVertex> points)
{
	//Draw Square
	VAO CubeVAO;
	CubeVAO.Bind();

	VBO CubeVBO(reinterpret_cast<GLfloat*>(points.data()), static_cast<GLsizeiptr>(points.size() * sizeof(CubeVertex)));
	CubeVBO.Bind();

	EBO CubeEBO(CubeIndices, sizeof(CubeIndices));
	CubeEBO.Bind();

	//Specify vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glDrawElements(GL_TRIANGLES, sizeof(CubeIndices), GL_UNSIGNED_INT, nullptr);

	CubeVAO.Unbind();
	CubeVBO.Unbind();
	CubeEBO.Unbind();

	CubeVAO.Delete();
	CubeVBO.Delete();
	CubeEBO.Delete();

};

void Cube::CreateCube(glm::vec3 position, glm::vec3 scale, float rotation)
{

	glm::vec3 Position = position;
	glm::mat4 Matrix;
	GLfloat Matrice[8][3] =
	{
		{ position.x, position.y, position.z},
		{ position.x + scale.x, position.y, position.z },
		{ position.x, position.y + scale.y, position.z },
		{ position.x + scale.x, position.y + scale.y },
		{ position.x, position.y + scale.y, position.z },
		{ position.x + scale.x, position.y, position.z },
		{ position.x + scale.x, position.y + scale.y },
		{ position.x, position.y, position.z + scale.z }
	};

	std::vector<CubeVertex> Cubepoints;
	Cubepoints.push_back(CubeVertex{ position.x, position.y, position.z,1,1,0 });
	Cubepoints.push_back(CubeVertex{ position.x + scale.x, position.y, position.z,1,0,1 });
	Cubepoints.push_back(CubeVertex{ position.x, position.y + scale.y, position.z,1,0,0 });
	Cubepoints.push_back(CubeVertex{ position.x + scale.x, position.y + scale.y, position.z,1,0,1 });

	Cubepoints.push_back(CubeVertex{ position.x, position.y + scale.y, position.z + scale.z,1,0,0.5f });
	Cubepoints.push_back(CubeVertex{ position.x + scale.x, position.y, position.z + scale.z,0.5f,0.2f,0 });
	Cubepoints.push_back(CubeVertex{ position.x + scale.x, position.y + scale.y, position.z + scale.z,1,0,1 });
	Cubepoints.push_back(CubeVertex{ position.x, position.y, position.z + scale.z,1,0.3f,0.4f });

	

	AABB.Position = position  ;
	AABB.Extent = glm::vec3(scale.x/2,scale.y/2,scale.z/2) ;
	DrawCube(Cubepoints);

};

