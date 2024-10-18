#pragma once
#include "../Components/ComponentManager.h"
class AttackCheck
{
	
public:

	bool CheckifOverlap(ComponentManager<CollisionComponent>& collisiondata,ComponentManager<PositionComponent>& pos, int EntityID)
	{
		
		if (collisiondata.GetComponent(EntityID).CheckifHasCollision())
		{
			if (collisiondata.HasComponent(EntityID + 1)) {

				auto a = pos.GetComponent(EntityID).GetPosition();
				auto b = pos.GetComponent(EntityID + 1).GetPosition();
				auto ae = collisiondata.GetComponent(EntityID).GetSize();
				auto be = collisiondata.GetComponent(EntityID + 1).GetSize();

				if (abs(a[0] - b[0]) > (ae[0] + be[0])) return false;
				if (abs(a[1] - b[1]) > (ae[1] + be[1])) return false;
				if (abs(a[2] - b[2]) > (ae[2] + be[2])) return false;
				return true;
			}
		}
		return false;

	}
	void Collision(ComponentManager<MovementComponent>& Speed, int EntityID)
	{
	
			glm::vec3 V1 = ((Speed.GetComponent(EntityID).GetMass() - Speed.GetComponent(EntityID + 1).GetMass()) / (Speed.GetComponent(EntityID).GetMass() + Speed.GetComponent(EntityID + 1).GetMass()) * Speed.GetComponent(EntityID).GetVelocity())
				+ ((2 * Speed.GetComponent(EntityID + 1).GetMass()) / (Speed.GetComponent(EntityID).GetMass() + Speed.GetComponent(EntityID + 1).GetMass()) * Speed.GetComponent(EntityID+1).GetVelocity());
			glm::vec3 V2 = ((Speed.GetComponent(EntityID).GetMass() * 2) / (Speed.GetComponent(EntityID).GetMass() + Speed.GetComponent(EntityID + 1).GetMass()) * Speed.GetComponent(EntityID).GetVelocity())
				+ ((Speed.GetComponent(EntityID + 1).GetMass() - Speed.GetComponent(EntityID).GetMass()) / (Speed.GetComponent(EntityID).GetMass() + Speed.GetComponent(EntityID + 1).GetMass()) * Speed.GetComponent(EntityID+1).GetVelocity());
			Speed.GetComponent(EntityID).SetVelocity(V1);
			Speed.GetComponent(EntityID + 1).SetVelocity(V2);
			
	}
	

		
	

};

