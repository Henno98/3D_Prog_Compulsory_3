#pragma once
#include "../Vertex.h"
#include "vector"
#include "includes/glm/glm.hpp"
class PositionComponent
{
public:
	std::vector<Vertex> vertices;
	glm::vec3 Position;
	PositionComponent() = default;
	PositionComponent(glm::vec3 pos) : Position(pos)
	{
		vertices.emplace_back(Position);
	}
	glm::vec3 GetPosition()
	{
		return Position;
	};
	void SetPosition(glm::vec3 newpos)
	{
		vertices.clear();
		Position = newpos;
		vertices.emplace_back(Position);
	};
	std::vector<Vertex>& GetVertex()
	{

		return vertices;
	}
};

