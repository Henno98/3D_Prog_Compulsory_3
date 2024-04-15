#pragma once
#include "vector"
#include "Shaders/VAO.h"
#include "Shaders/VBO.h"
#include "Shaders/EBO.h"
#include "Shaders/ShaderClass.h"
#include <glm/glm.hpp>
#include "BoundingBox.h"

#ifndef CUBE_H
#define CUBE_H

struct CubeVertex
{
	float x, y, z;
	float r, g, b;
};


class Cube
{
public:
	glm::mat4 CubeMatrix = glm::mat4(1.0f);
	BoundingBox AABB;
	int collectedtrophies;
	Cube();
	void DrawCube(std::vector<CubeVertex> points, Shader& shader, const char* uniform);
	void CreateCube(glm::vec3 position, glm::vec3 scale, Shader& shader, const char* uniform);
};


#endif // CUBE_H