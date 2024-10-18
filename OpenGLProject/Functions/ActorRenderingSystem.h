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
struct Indice
{

	GLint v0, v1, v2;
};
class ActorRenderingSystem 
{

public:
	VAO ActorVAO;
	VBO ActorVBO;
	EBO ActorEBO;
	std::vector<Vertex> Datapoints;
	std::vector<Indice> Indices;
	
	//Init the Matrix
	glm::mat4 Matrix = glm::mat4(1.f);

	void InsertData(glm::vec3 v1)
	{
		float halfSize = 0.5f;


		Vertex V{ glm::vec3(-halfSize, -halfSize, -halfSize),glm::vec3(v1.x,v1.y,v1.z) ,glm::vec3(1.f) };
		Datapoints.emplace_back(V);
		V = Vertex{ glm::vec3(halfSize, -halfSize, -halfSize),glm::vec3(v1.x,v1.y,v1.z) ,glm::vec3(1.f) };
		Datapoints.emplace_back(V);
		V = Vertex{ glm::vec3(halfSize,  halfSize, -halfSize),glm::vec3(v1.x,v1.y,v1.z) ,glm::vec3(1.f) };
		Datapoints.emplace_back(V);
		V = Vertex{ glm::vec3(-halfSize,  halfSize, -halfSize),glm::vec3(v1.x,v1.y,v1.z) ,glm::vec3(1.f) };
		Datapoints.emplace_back(V);
		V = Vertex{ glm::vec3(-halfSize, -halfSize,  halfSize),glm::vec3(v1.x,v1.y,v1.z) ,glm::vec3(1.f) };
		Datapoints.emplace_back(V);
		V = Vertex{ glm::vec3(halfSize, -halfSize,  halfSize),glm::vec3(v1.x,v1.y,v1.z) ,glm::vec3(1.f) };
		Datapoints.emplace_back(V);
		V = Vertex{ glm::vec3(halfSize,  halfSize,  halfSize),glm::vec3(v1.x,v1.y,v1.z) ,glm::vec3(1.f) };
		Datapoints.emplace_back(V);
		V = Vertex{ glm::vec3(-halfSize,  halfSize,  halfSize),glm::vec3(v1.x,v1.y,v1.z) ,glm::vec3(1.f) };
		Datapoints.emplace_back(V);


		/*Indices.emplace_back(Indice{ 0,1,2 });
		Indices.emplace_back(Indice{ 2,3,0 });
		Indices.emplace_back(Indice{ 4, 5, 6 });
		Indices.emplace_back(Indice{ 6, 7, 4 });
		Indices.emplace_back(Indice{ 0, 3, 7 });
		Indices.emplace_back(Indice{ 7, 4, 0 });
		Indices.emplace_back(Indice{ 1, 5, 6 });
		Indices.emplace_back(Indice{ 6, 2, 1 });
		Indices.emplace_back(Indice{ 0, 4, 5 });
		Indices.emplace_back(Indice{ 5, 1, 0 });
		Indices.emplace_back(Indice{ 3, 2, 6 });
		Indices.emplace_back(Indice{ 6, 7, 3 });*/

	}
	void initBinders()
	{
		unsigned int cubeIndices[] = {
			// Front face
			0, 1, 2,
			2, 3, 0,
			// Back face
			4, 5, 6,
			6, 7, 4,
			// Left face
			0, 3, 7,
			7, 4, 0,
			// Right face
			1, 5, 6,
			6, 2, 1,
			// Bottom face
			0, 4, 5,
			5, 1, 0,
			// Top face
			3, 2, 6,
			6, 7, 3
		};
		//init binders
		ActorVAO.Initialize();
		ActorVBO.Initialize(reinterpret_cast<GLfloat*>(Datapoints.data()), static_cast<GLsizeiptr>(Datapoints.size() * sizeof(Vertex)));
		ActorEBO.init(cubeIndices,sizeof(cubeIndices));
		//ActorEBO.init(cubeIndices, sizeof(cubeIndices));
	}
	
	//Draws all inputted Actors
	void DrawActor(Shader& shader, const char* uniform, ComponentManager<PositionComponent>& pos, const std::vector<Entity>& entities)
	{

		unsigned int cubeIndices[] = {
			// Front face
			0, 1, 2,
			2, 3, 0,
			// Back face
			4, 5, 6,
			6, 7, 4,
			// Left face
			0, 3, 7,
			7, 4, 0,
			// Right face
			1, 5, 6,
			6, 2, 1,
			// Bottom face
			0, 4, 5,
			5, 1, 0,
			// Top face
			3, 2, 6,
			6, 7, 3
		};
		

		ActorVAO.Bind();
		ActorVBO.Bind();
		ActorEBO.Bind();
		for (int i = 0; i < entities.size(); i++)
		{

			Matrix = MatrixCalc(pos.GetComponent(entities[i].EntityID).GetPosition());

			Vertex::BindAttributes();
			glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(Matrix));
			glDrawElements(GL_TRIANGLES, sizeof(cubeIndices), GL_UNSIGNED_INT, nullptr);

		}

		
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

