#pragma once
#include "glm/vec3.hpp"

class CollisionComponent
{
public:
	bool Hascollision;
	glm::vec3 Extent;
	CollisionComponent(bool state, glm::vec3 size) : Hascollision(state),Extent(size){}
	bool CheckifHasCollision()
	{

		return Hascollision;
	}
	void setCollision(bool newstate)
	{
		Hascollision = newstate;
	}
	glm::vec3 GetSize()
	{

		return Extent;
	}
	void SetSize(glm::vec3 newSize)
	{
		Extent = newSize;
	}
};

