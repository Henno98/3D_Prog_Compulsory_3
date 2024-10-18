#pragma once
#include "../Components/ComponentManager.h"
#include "../Shaders/ShaderClass.h"
#include "../Shaders/VAO.h"
#include "../Shaders/VBO.h"
#include "../Shaders/EBO.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct Vertex {
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 Color = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 normal = glm::vec3(0.f);

	Vertex() = default;
	Vertex(glm::vec3 pos) : position(pos) {}
	Vertex(float x, float y, float z) : position(glm::vec3(x, y, z)) {}
	Vertex(glm::vec3 pos, glm::vec3 nor) : position(pos), normal(nor) {}
	Vertex(glm::vec3 pos, glm::vec3 nor, glm::vec3 tex) : position(pos), normal(nor), Color(tex) {}

	// Bind Vertex Attributes for the shader. Remember to change the shader to accept the corrent attributes
	static void BindAttributes() {
		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		// Normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
		glEnableVertexAttribArray(1);

		// Texture Coordinates
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(2);
		/*
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, mTexCoord));
		glEnableVertexAttribArray(2);*/
	}

};


class ActorRenderingSystem 
{

public:
	VAO ActorVAO;
	VBO ActorVBO;
	EBO ActorEBO;
	std::vector<Vertex> Datapoints;
	std::vector<int> Indices;
	//Init the Matrix
	glm::mat4 Matrix = glm::mat4(1.f);

	void InsertData(glm::vec3 v1)
	{
		float halfSize = 0.5f;

		// Define the vertices of the cube based on the center position
		glm::vec3 vertexOffsets[8] = {
			glm::vec3(-halfSize, -halfSize, -halfSize), // Vertex 0
			glm::vec3(halfSize, -halfSize, -halfSize), // Vertex 1
			glm::vec3(halfSize,  halfSize, -halfSize), // Vertex 2
			glm::vec3(-halfSize,  halfSize, -halfSize), // Vertex 3
			glm::vec3(-halfSize, -halfSize,  halfSize), // Vertex 4
			glm::vec3(halfSize, -halfSize,  halfSize), // Vertex 5
			glm::vec3(halfSize,  halfSize,  halfSize), // Vertex 6
			glm::vec3(-halfSize,  halfSize,  halfSize)  // Vertex 7
		};
		Vertex V{ glm::vec3(v1.x,v1.y,v1.z),glm::vec3(v1.x,v1.y,v1.z) ,glm::vec3(1.f)};
		Datapoints.emplace_back(V);
		
	}
	void initBinders()
	{
		//init binders
		ActorVAO.Initialize();
		ActorVBO.Initialize(reinterpret_cast<GLfloat*>(Datapoints.data()), (Datapoints.size() * sizeof(Vertex)));
		ActorEBO.init(Indices, sizeof(Indices));
	}
	
	//Draws all inputted Actors
	void DrawActor(Shader& shader, const char* uniform, ComponentManager<PositionComponent>& pos, const std::vector<Entity>& entities)
	{





		ActorVAO.Bind();
		ActorVBO.Bind();
		ActorEBO.Bind();
		for (int i = 0; i < entities.size(); i++)
		{

			Matrix = MatrixCalc(pos.GetComponent(entities[i].EntityID).GetPosition());

			//Specify vertex attribute pointers
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(Matrix));
			glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);

		}

		//unbind when done
		ActorVAO.Unbind();
		ActorVBO.Unbind();
		ActorEBO.Unbind();
	}

	void DeleteBinders()
	{
		ActorVAO.Delete();
		ActorVBO.Delete();
		ActorEBO.Delete();
	}
	//Calculates where the Matrix for the entity is
	glm::mat4 MatrixCalc(glm::vec3 pos)
	{

		glm::mat4 ActorMatrix = glm::mat4(1.f);
		ActorMatrix = glm::translate(ActorMatrix, pos);

		return ActorMatrix;
	}
};

