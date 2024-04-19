#include "Cube.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>




Cube::Cube()
{
	collectedtrophies = 0;
};

void Cube::DrawCube(glm::vec3 scale, glm::vec3 color, Shader& shader, const char* uniform)
{

	GLfloat Matrice[] =
	{
		scale.x,scale.y,scale.z, color.x,color.y,color.z,
		scale.x,-scale.y,scale.z, color.x,color.y,color.z,
		scale.x,-scale.y,-scale.z, color.x,color.y,color.z,
		scale.x,scale.y,-scale.z, color.x,color.y,color.z,

		-scale.x,scale.y,scale.z, color.x,color.y,color.z,
		-scale.x,-scale.y,scale.z, color.x,color.y,color.z,
		-scale.x,-scale.y,-scale.z, color.x,color.y,color.z,
		-scale.x,scale.y,-scale.z, color.x,color.y,color.z,

		

	};

	unsigned int CubeIndices[] =
	{
		0,1,2,  // Front face
		0,3,2,

		7,4,5,  // Back face
		7,5,6,

		0,3,4,  // Top face
		4,3,7,

		1,2,5, // Bottom face
		6,2,5,

		7,3,2, // Left face
		7,2,6,

		0,4,5,  // Right face
		0,5,1
	};



	//Draw Square
	VAO CubeVAO;
	CubeVAO.Bind();

	VBO CubeVBO(Matrice,sizeof(Matrice));
	CubeVBO.Bind();

	EBO CubeEBO(CubeIndices, sizeof(CubeIndices));
	CubeEBO.Bind();

	//Specify vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(CubeMatrix));
	glDrawElements(GL_TRIANGLES, sizeof(CubeIndices), GL_UNSIGNED_INT, nullptr);

	CubeVAO.Unbind();
	CubeVBO.Unbind();
	CubeEBO.Unbind();

	CubeVAO.Delete();
	CubeVBO.Delete();
	CubeEBO.Delete();

};

void Cube::CreateCube(glm::vec3 position, glm::vec3 scale, Shader& shader, const char* uniform)
{

	

};

