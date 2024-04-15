#include "NPC.h"

#include <iostream>
#include <vector>
#include <Eigen/Dense>

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "Shaders/EBO.h"
#include "Shaders/VAO.h"
using namespace Eigen;
//GLuint NPCIndices[]
//{
//	0,1,2,
//	0,2,3,
//	3,4,2,
//	4,2,1
//};


NPC::NPC()
{
	Color = glm::vec3(1, 0, 1);
	
}



void NPC::DrawNPC(Shader& shader, const char* uniform)
{
	GLfloat Indices[] =
	{
		0.f,0.f,0.f,0,0,0,
		1,0,0,1,1,1,
		0,0,1,0,0,0,
		1,0,1,1,1,1,
		0.5,1,0.5,0,0,0,

	};

	GLuint NPCIndices[]
	{
		0,1,4,
		0,2,4,
		1,3,4,
		2,3,4
	};

	
	//Draw Square
	VAO NPCVAO;
	NPCVAO.Bind();

	VBO NPCVBO(Indices, sizeof(Indices));
	NPCVBO.Bind();

	EBO NPCEBO(NPCIndices, sizeof(NPCIndices));
	NPCEBO.Bind();

	//Specify vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(NPCVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(NPCVertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int modelloc = glGetUniformLocation(shader.ID, "model");
	glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(NPCMatrix));

	glDrawElements(GL_TRIANGLES, sizeof(NPCIndices), GL_UNSIGNED_INT, nullptr);

	NPCVAO.Unbind();
	NPCVBO.Unbind();
	NPCEBO.Unbind();

	NPCVAO.Delete();
	NPCVBO.Delete();
	NPCEBO.Delete();
}

std::vector<NPCVertex> NPC::CalculateMovement(Eigen::MatrixXd solution, int range)
{
	std::vector<NPCVertex> Movementpoints;
	float xval;
	double zval;
	
		float z = (solution(0, 0) * pow(range, 3)) + (solution(1, 0) * pow(range, 2)) + (solution(2, 0) * range) + solution(3, 0);
		std::cout << "for X value: " << range << " Z value is: " << z << std::endl;

		xval = range;
		zval = z;

		Movementpoints.push_back(NPCVertex{ xval,0,z ,1.f,0.f,0.f });
		Movementpoints.push_back(NPCVertex{ xval + 1,0,z,1.f,0.f,0.f });
		Movementpoints.push_back(NPCVertex{ xval + 0.5f, 1,z + 0.5f,Color.x,Color.y,Color.z });
		Movementpoints.push_back(NPCVertex{ xval,0,z + 1,1.f,0.f,0.f });
		Movementpoints.push_back(NPCVertex{ xval + 1,0,z + 1,1.f,0.f,0.f });


	
	return Movementpoints;

}

void NPC::Movement(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4, int range)
{
	
		//DrawNPC(CalculateMovement(SolutionMatrix,range));

}

void NPC::CreateNPC(glm::vec3 position, glm::vec3 size, Shader& shader, const char* uniform)
{
	

	
	


	
	
	
	NPCBox.Position = position;
	NPCBox.Extent = glm::vec3(size.x / 2, size.y / 2, size.z / 2);
}
void NPC::UpdateMovement(glm::vec3 position, glm::vec3 Targetlocation)
{
	

}
