#pragma once
#include "includes/glm/glm.hpp"
class MovementComponent
{public:
	glm::vec3 Speed;
	MovementComponent() = default;
	MovementComponent(glm::vec3 speed) : Speed(speed){}

	glm::vec3 GetSpeed()
	{
		return  Speed;
	};
	void SetSpeed(glm::vec3 newspeed)
	{
		Speed = newspeed;

	};
};

