#pragma once
class PickUpComponent
{
public:
	enum class Type
	{
		Health,
		Ammo,
		PowerUp

	};
	Type type;
	float value;
	bool isActive;
	PickUpComponent(Type type = Type::Health,float value) : type(type),value(value),isActive(true)
	{
		
	}
};

