#pragma once
#include <vector>
#include <Eigen/Dense>
#include "BoundingBox.h"
#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "Shaders/ShaderClass.h"

struct NPCVertex
{
	GLfloat x, y, z, r, g, b;
};
class NPC
{

public:

	glm::mat4 NPCMatrix = glm::mat4(1.0f);
	std::vector<NPCVertex> Movementpoints;
	BoundingBox NPCBox;
	glm::vec3 Color;
	glm::vec3 currentposition;
	glm::vec3 Targetposition;
	glm::vec3 Oldposition;
	glm::vec3 Newposition;
	NPC();
	void Movement(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4, int range);
	void CreateNPC(glm::vec3 position, glm::vec3 size, Shader& shader, const char* uniform);
	void DrawNPC( Shader& shader, const char* uniform);
	std::vector<NPCVertex> CalculateMovement(Eigen::MatrixXd solution, int range);
	void UpdateMovement(glm::vec3 position, glm::vec3 Targetlocation);
};

