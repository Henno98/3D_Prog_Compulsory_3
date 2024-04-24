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
	

	double x0 = pos1.x;
	double x1 = pow(pos1.x, 3);
	double x2 = pow(pos1.x, 2);

	double x02 = pos2.x;
	double x12 = pow(pos2.x, 3);
	double x22 = pow(pos2.x, 2);

	double x03 = pos3.x;
	double x13 = pow(pos3.x, 3);
	double x23 = pow(pos3.x, 2);

	double x04 = pos4.x;
	double x14 = pow(pos4.x, 3);
	double x24 = pow(pos4.x, 2);

	AMatrix <<
		x1, x2, x0, 1.0,
		x12, x22, x02, 1.0,
		x13, x23, x03, 1.0,
		x14, x24, x04, 1.0;

	YMatrix <<
		pos1.z,
		pos2.z,
		pos3.z,
		pos4.z;

	MatrixXd AInverse = AMatrix.inverse();
	MatrixXd SolutionMatrix = AInverse * YMatrix;


	float z = (SolutionMatrix(0, 0) * pow(x, 3)) + (SolutionMatrix(1, 0) * pow(x, 2)) + (SolutionMatrix(2, 0) * SolutionMatrix(3, 0));
	//std::cout << "for X value: " << x << " Z value is: " << z << std::endl;

	if (!turn == true) {
		NPCMatrix = glm::translate(NPCMatrix, glm::vec3(x / 1000, 0, z / 1000));
		x += 0.1f;
		if (x >= 10)
		{
			turn = true;
		}

	}
	if (!turn == false)
	{
		NPCMatrix = glm::translate(NPCMatrix, glm::vec3(-x / 1000, 0, -z / 1000));
		x -= 0.1f;
		if (x <= 0)
		{
			turn = false;
		}
	}

}

glm::vec3 NPC::barycentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4)
{

	p1.y = 0;
	p2.y = 0;
	p3.y = 0;
	p4.y = 0;

	glm::vec3 p12 = p2 - p1;
	glm::vec3 p13 = p3 - p1;
	glm::vec3 cross = glm::cross(p12, p13);
	float area_123 = cross.y; // double the area
	area_123 /= 2;
	glm::vec3 baryc; // for return

	// u
	glm::vec3 p = p2 - p4;
	glm::vec3 q = p3 - p4;
	glm::vec3 nu = glm::cross(q, p);
	nu.y /= 2;
	// double the area of p4pq
	baryc.x = nu.y / area_123;

	// v
	p = p3 - p4;
	q = p1 - p4;
	glm::vec3 nv = glm::cross(p, q);
	nv.y /= 2;// double the area of p4pq
	baryc.y = nv.y / area_123;

	// w
	p = p1 - p4;
	q = p2 - p4;
	glm::vec3 nw = (glm::cross(p, q));
	nw.y /= 2;// double the area of p4pq
	baryc.z = nw.y / area_123;

	return baryc;
}
;
void NPC::UpdateMovement(glm::vec3 position, glm::vec3 Targetlocation)
{
	

}
