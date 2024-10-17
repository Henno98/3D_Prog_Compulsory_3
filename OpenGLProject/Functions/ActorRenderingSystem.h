#pragma once
#include "../Components/ComponentManager.h"
#include "../Shaders/ShaderClass.h"
#include "../Shaders/VAO.h"
#include "../Shaders/VBO.h"
#include "../Shaders/EBO.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"




class ActorRenderingSystem 
{

public:
	glm::mat4 Matrix = glm::mat4(1.f);
	void Update(const std::vector<std::shared_ptr<Entity>>& entities) 
	{
		//const ActorData& actor = ActorData;
		for( int i = 0; i < entities.size(); i++)
		{
			

		}
	};
	
	void DrawActor(Shader& shader,const char* uniform, ComponentManager<PositionComponent>& pos,  const std::vector<Entity>& entities)
	{
		VAO ActorVAO;
		VBO ActorVBO;
		
		GLfloat Vertices[] = {
			//  x,  y,  z,    R,  G,  B,   Nx, Ny, Nz
	 1,  1,  1,   1, 1, 1,    1,  1,  1,   // Vertex 0
	 1,  1, -1,   1, 1,   0,    1,  1, -1,   // Vertex 1
	 1, -1,  1,   1,   0, 1,    1, -1,  1,   // Vertex 2
	 1, -1, -1,   1,   0,   0,    1, -1, -1,   // Vertex 3
	-1,  1,  1,     0, 1, 1,   -1,  1,  1,   // Vertex 4
	-1,  1, -1,     0, 1,   0,   -1,  1, -1,   // Vertex 5
	-1, -1,  1,     0,   0, 1,   -1, -1,  1,   // Vertex 6
	-1, -1, -1,     0,   0,   0,   -1, -1, -1    // Vertex 7
		};
		unsigned int indices[] = {
			// Front face
			0, 2, 4,   // First triangle
			2, 4, 6,   // Second triangle

			// Back face
			1, 3, 5,   // First triangle
			3, 5, 7,   // Second triangle

			// Top face
			0, 1, 4,   // First triangle
			1, 4, 5,   // Second triangle

			// Bottom face
			2, 3, 6,   // First triangle
			3, 6, 7,   // Second triangle

			// Left face
			4, 5, 6,   // First triangle
			5, 6, 7,   // Second triangle

			// Right face
			0, 1, 2,   // First triangle
			1, 2, 3    // Second triangle
		};
		ActorVAO.Initialize();
		ActorVBO.Initialize(Vertices, sizeof(Vertices));
		ActorVAO.Bind();
		ActorVBO.Bind();
		
		EBO ActorEBO(indices, sizeof(indices));
		ActorEBO.Bind();
		for(int i = 0; i < entities.size();i++)
		{
			
			Matrix = MatrixCalc(pos.GetComponent(entities[i].EntityID).GetPosition());
			
			//Specify vertex attribute pointers
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,  9 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(Matrix));
			glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr);

		}
		ActorVAO.Unbind();
		ActorVBO.Unbind();
		ActorEBO.Unbind();
		Matrix = glm::mat4(1.f);

	};
	void UpdateActor()
	{


	};
	
	glm::mat4 MatrixCalc(glm::vec3 pos)
	{

		glm::mat4 ActorMatrix = glm::mat4(1.f);
		ActorMatrix = glm::translate(ActorMatrix, pos);

		return ActorMatrix;
	}
};

