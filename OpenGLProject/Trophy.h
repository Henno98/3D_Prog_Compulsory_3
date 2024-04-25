#pragma once
#include <vector>

#include "glm/vec3.hpp"
#include "BoundingBox.h"
#include "Shaders/ShaderClass.h"

struct trophyVertex
{
	float x, y, z, r, g, b;
};
class Trophy
{
public:
	bool placed = false;
	glm::mat4 TrophyMatrix = glm::mat4(1.0f);
	glm::vec3 Color;
	BoundingBox AABB;
	bool trophycollected;
	Trophy();
	void PlaceTrophy(glm::vec3 position, glm::vec3 size);
	void DrawTrophy( glm::vec3 size, Shader& shader, const char* uniform);
	void DestroyTrophy();
};

