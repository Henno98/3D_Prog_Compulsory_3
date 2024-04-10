#include "NPC.h"

#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "Shaders/EBO.h"
#include "Shaders/VAO.h"
using namespace Eigen;
GLuint NPCIndices[]
{
	0,1,2,
	0,2,3,
	3,4,2,
	4,2,1
};

NPC::NPC()
{
	Color = glm::vec3(1, 0, 1);
}


void NPC::CreateNPC(glm::vec3 position, glm::vec3 size)
{
	

	std::vector<NPCVertex> NPCPoints;
	NPCPoints.push_back(NPCVertex{ position.x,position.y,position.z ,1.f,0.f,0.f});
	NPCPoints.push_back(NPCVertex{ position.x + size.x,position.y,position.z,1.f,0.f,0.f });
	NPCPoints.push_back(NPCVertex{ position.x+size.x/2,position.y+size.y,position.z+size.z/2,Color.x,Color.y,Color.z });
	NPCPoints.push_back(NPCVertex{ position.x,position.y,position.z + size.z,1.f,0.f,0.f });
	NPCPoints.push_back(NPCVertex{ position.x + size.x,position.y,position.z + size.z,1.f,0.f,0.f });
	DrawNPC(NPCPoints);
	NPCBox.Position = position;
	NPCBox.Extent = glm::vec3(size.x/2,size.y/2,size.z/2);
}



void NPC::DrawNPC(std::vector<NPCVertex> points)
{
	//Draw Square
	VAO NPCVAO;
	NPCVAO.Bind();

	VBO NPCVBO(reinterpret_cast<GLfloat*>(points.data()), static_cast<GLsizeiptr>(points.size() * sizeof(NPCVertex)));
	NPCVBO.Bind();

	EBO NPCEBO(NPCIndices, sizeof(NPCIndices));
	NPCEBO.Bind();

	//Specify vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(NPCVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(NPCVertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glDrawElements(GL_TRIANGLES, sizeof(NPCIndices), GL_UNSIGNED_INT, nullptr);

	NPCVAO.Unbind();
	NPCVBO.Unbind();
	NPCEBO.Unbind();

	NPCVAO.Delete();
	NPCVBO.Delete();
	NPCEBO.Delete();
}

std::vector<NPCVertex> NPC::Movement(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4)
{
	MatrixXd AMatrix(4, 4);
	MatrixXd YMatrix(4, 1);

	double x1 = pow(pos1.x, 3);
	double x2 = pow(pos1.x, 2);

	double x12 = pow(pos2.x, 3);
	double x22 = pow(pos2.x, 2);

	double x13 = pow(pos3.x, 3);
	double x23 = pow(pos3.x, 2);

	double x14 = pow(pos4.x, 3);
	double x24 = pow(pos4.x, 2);

	AMatrix <<
		x1, x2, pos1.x, 1,
		x12, x22, pos2.x, 1,
		x13, x23, pos3.x, 1,
		x14, x24, pos4.x, 1;

	YMatrix <<
		pos1.z,
		pos2.z,
		pos3.z,
		pos4.z;

	MatrixXd AInverse = AMatrix.inverse();
	MatrixXd SolutionMatrix = AInverse * YMatrix;

	//std::cout << "Matrix A Answer: " << AMatrix << std::endl;
	//std::cout << "Matrix Y Answer: " << YMatrix << std::endl;
	//std::cout << "Matrix A-1 Answer: " << AInverse << std::endl;
	//std::cout <<"Matrix Answer: " << SolutionMatrix << std::endl;
	std::vector<NPCVertex> Movementpoints;
	
	for (float x = 0; x < 5; x++)
	{
		float z = (SolutionMatrix(0, 0) * pow(x, 3)) + (SolutionMatrix(1, 0) * pow(x, 2)) + (SolutionMatrix(2, 0) * SolutionMatrix(3, 0);
		std::cout << "for X value: " << x << " Y value is: " << z << std::endl;


		Movementpoints.push_back(NPCVertex{ x,0,z ,1.f,0.f,0.f });
		Movementpoints.push_back(NPCVertex{ x + 1,0,z,1.f,0.f,0.f });
		Movementpoints.push_back(NPCVertex{ x + 0.5f, 1,z + 0.5f,Color.x,Color.y,Color.z });
		Movementpoints.push_back(NPCVertex{ x,0,z + 1,1.f,0.f,0.f });
		Movementpoints.push_back(NPCVertex{ x + 1,0,z + 1,1.f,0.f,0.f });


	}
	return Movementpoints;


}
std::vector<NPCVertex> NPC::CalculateMovement(Eigen::MatrixXd solution)
{	std::vector<NPCVertex> Movementpoints;
	float xval;
	double zval;
	for (float x = 0; x < 5; x ++)
	{
		float z = (solution(0, 0) * pow(x, 3)) + (solution(1, 0) * pow(x, 2)) + (solution(2, 0) * x) + solution(3, 0);
		std::cout << "for X value: " << x << " Y value is: " << z << std::endl;

		xval = x;
		zval = z;
		
		Movementpoints.push_back(NPCVertex{ x,0,z ,1.f,0.f,0.f });
		Movementpoints.push_back(NPCVertex{ x + 1,0,z,1.f,0.f,0.f });
		Movementpoints.push_back(NPCVertex{ x + 0.5f, 1,z + 0.5f,Color.x,Color.y,Color.z });
		Movementpoints.push_back(NPCVertex{x,0,z + 1,1.f,0.f,0.f });
		Movementpoints.push_back(NPCVertex{x + 1,0,z + 1,1.f,0.f,0.f });
		
		
	}
	return Movementpoints;
	
	//CreateNPC(glm::vec3(x, 0, z), glm::vec3(1, 1, 1));
}

void NPC::UpdateMovement(glm::vec3 position, glm::vec3 Targetlocation)
{

}
