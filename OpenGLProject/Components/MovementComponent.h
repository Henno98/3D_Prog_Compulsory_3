#pragma once
#include "includes/glm/glm.hpp"
class MovementComponent
{public:
	glm::vec3 Velocity;
	float Mass;
	float Speed;
	MovementComponent() = default;
	MovementComponent(glm::vec3 velocity,float mass, float speed) : Velocity(velocity), Mass(mass), Speed(speed){}

	glm::vec3 GetVelocity()
	{
		return  Velocity;
	};
	void SetVelocity(glm::vec3 newvelocity)
	{
		Velocity = newvelocity;

	};
	float GetMass()
	{

		return Mass;
	}
	void SetMass(float newMass)
	{

		Mass = newMass;
	}
	float GetSpeed()
	{

		return Speed;
	}
	void SetSpeed(float newspeed)
	{
		Speed = newspeed;
	}
};

