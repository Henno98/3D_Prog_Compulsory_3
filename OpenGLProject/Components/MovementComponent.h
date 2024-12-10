#pragma once
#include "includes/glm/glm.hpp"
enum MovementType
{
	Tracking,
	Falling,
	Stationary

};

class MovementComponent
{public:
	glm::vec3 Velocity;
	float Mass;
	float Speed;
	MovementType Type;
	MovementComponent() = default;
	MovementComponent(MovementType type, glm::vec3 velocity,float mass, float speed) :Type(type), Velocity(velocity), Mass(mass), Speed(speed){}

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
	MovementType GetType()
	{

		return Type;
	}
};

