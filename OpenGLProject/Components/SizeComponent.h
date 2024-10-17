#pragma once
#include "glm/vec3.hpp"

class SizeComponent
{
	glm::vec3 Scale;
	SizeComponent() = default;
	SizeComponent(glm::vec3 scale) : Scale(scale)
	{
	}
	glm::vec3 GetPosition()
	{
		return Scale;
	};
	void SetScale(glm::vec3 newscale)
	{

		Scale = newscale;
	};
};

