#pragma once
#include "glm/fwd.hpp"
#include "vector"
#include "Shaders/VAO.h"
#include "Shaders/VBO.h"
#include "Shaders/EBO.h"
#include "Shaders/ShaderClass.h"
#include <glm/glm.hpp>
#include "BoundingBox.h"


struct LightVertex
{
	float x, y, z;
	float r, g, b;
};


class Light
{
public:

	glm::mat4 LightMatrix = glm::mat4(1.0f);
	BoundingBox AABB;
	Light();
	void DrawLight();
	void CreateLight(glm::vec3 position, glm::vec3 scale, glm::vec3 color ,Shader& shader, const char* uniform);
};





