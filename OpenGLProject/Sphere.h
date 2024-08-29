#pragma once

#include "Shaders/ShaderClass.h"
#include <glm/glm.hpp>
#include "BoundingBox.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <math.h>


struct SphereVertex
{
	float x, y, z;
	float r, g, b;
	float n1, n2, n3;
};

class Sphere
{
public:
	glm::mat4 SphereMatrix = glm::mat4(1.0f);
	BoundingBox AABB;
	float length;
	float width;
	float radius;
	float angle;
	float subdivision;
	Sphere();
	void DrawSphere(glm::vec3 scale, glm::vec3 color, Shader& shader, const char* uniform);
	void CreateSphere(glm::vec3 position, glm::vec3 scale, float subdivison, float angle, Shader& shader, const char* uniform);
};

