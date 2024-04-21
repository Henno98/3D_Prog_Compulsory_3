#pragma once
#include "glm/fwd.hpp"
#include "vector"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "ShaderClass.h"
#include <glm/glm.hpp>


struct LightVertex
{
	float x, y, z;
	float r, g, b;
};


class Light
{
public:
	glm::vec4 lightColor;
	glm::vec3 lightPos;
	glm::mat4 lightModel = glm::mat4(1.0f);
	
	Light();
	void DrawLight();
	void CreateLight(glm::vec3 scale, glm::vec3 color);
};





