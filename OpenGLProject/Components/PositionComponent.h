#pragma once
#include "includes/glm/glm.hpp"
class PositionComponent
{
public:
	glm::vec3 Position;
	PositionComponent() = default;
	PositionComponent(glm::vec3 pos) : Position(pos)
	{
	}
	glm::vec3 GetPosition()
	{
		return Position;
	};
	void SetPosition(glm::vec3 newpos)
	{

		Position = newpos;
	};
};

