#pragma once
class HealthComponent
{
public:
	float Health;
	HealthComponent(float health) : Health(health){}
	float GetHealth()
	{

		return Health;
	}
	void SetHealth(float newHealth)
	{
		Health = newHealth;
	}
};

