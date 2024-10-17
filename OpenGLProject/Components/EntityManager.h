#pragma once
#include "iostream"
#include "ComponentManager.h"
struct Entity
{
private:
	
public:
	int EntityID;
	int GetId()
	{
		return EntityID;
	}
	void SetID(int newID)
	{
		EntityID = newID;

	}
	

};

class EntityManager
{
	int CurrentEntity = 0;
public:
	Entity CreateEntity();
};

