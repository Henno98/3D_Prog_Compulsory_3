#pragma once
#include <vector>
#include "Shaders/VAO.h"
#include "Shaders/VBO.h"
#include "Shaders/EBO.h"
#include "Shaders/ShaderClass.h"
#include <glm/glm.hpp>
#include "BoundingBox.h"

struct SurfaceVertex
{
	float x, y, z;
	float r, g, b;

};

class Surface
{
	Surface();
	void CreateSurface(glm::vec3 Position, glm::vec3 Extent);
	void DrawSurface(std::vector<SurfaceVertex> points);
	void BarysentricCalc();
};

