#pragma once
#include <vector>
#include <Eigen/Dense>
#include "BoundingBox.h"
#include "glm/vec3.hpp"
struct NPCVertex
{
	float x, y, z, r, g, b;
};
class NPC
{

public:
	BoundingBox NPCBox;
	glm::vec3 Color;
	glm::vec3 currentposition;
	glm::vec3 Targetposition;
	glm::vec3 Oldposition;
	glm::vec3 Newposition;
	NPC();
	std::vector<NPCVertex> Movement(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4);
	void CreateNPC(glm::vec3 position, glm::vec3 size);
	void DrawNPC(std::vector<NPCVertex> points);
	std::vector<NPCVertex> CalculateMovement(Eigen::MatrixXd solution );
	void UpdateMovement(glm::vec3 position, glm::vec3 Targetlocation);
};

