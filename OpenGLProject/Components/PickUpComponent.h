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
	float value{0};
	bool isActive{};
	PickUpComponent() = default;
	PickUpComponent(Type type , float value) : type(type), value(value), isActive(true)
	{
		
	}
};

