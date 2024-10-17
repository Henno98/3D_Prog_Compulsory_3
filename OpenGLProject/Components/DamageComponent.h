#pragma once
class DamageComponent
{
public:
	float Damage;
	DamageComponent(float damage) : Damage(damage){}
	float GetDamage()
	{
		return Damage;

	}
	void SetDamage(float newDamage)
	{

		Damage = newDamage;
	}
};

