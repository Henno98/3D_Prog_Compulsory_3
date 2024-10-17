#include "EntityManager.h"

Entity EntityManager::CreateEntity()
{
	Entity newEntity;
	newEntity.EntityID = CurrentEntity;
	CurrentEntity++;
	return newEntity;
}
