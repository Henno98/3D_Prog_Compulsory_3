#pragma once

#include "Shaders/ShaderClass.h"
#include <glm/glm.hpp>
#include "BoundingBox.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <vector>


struct SphereVertex
{
	float x, y, z;
	float r, g, b;
	float n1, n2, n3;
};
struct SphereIndices
{
	GLuint v0, v1, v2;


};

class Sphere
{
public:
	glm::mat4 SphereMatrix = glm::mat4(1.0f);
	std::vector<SphereVertex> sphere;
	std::vector < SphereIndices> Indices;
	BoundingBox AABB;
	float length;
	float width;
	float radius;
	float angle;
	int subdivision;
	Sphere();
	void DrawSphere( Shader& shader, const char* uniform);
	void CreateSphere( int subdivison, float Radius);
};

