#pragma once
class CollisionComponent
{
	bool Hascollision;
	CollisionComponent(bool state) : Hascollision(state){}
	bool CheckifHasCollision()
	{

		return Hascollision;
	}
	void setCollision(bool newstate)
	{
		Hascollision = newstate;
	}
};

