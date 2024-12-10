#pragma once
#include "../Components/ComponentManager.h"
class AttackCheck
{
	
public:

	bool CheckifOverlap(ComponentManager<CollisionComponent>& collisiondata, ComponentManager<PositionComponent>& pos, int Entityone,int Entitytwo)
	{
		if (collisiondata.HasComponent(Entitytwo)) {
			if (collisiondata.HasComponent(Entityone)) {
			if (collisiondata.GetComponent(Entityone).CheckifHasCollision())
			{
				

					glm::vec3 a = pos.GetComponent(Entityone).GetPosition();
					glm::vec3 b = pos.GetComponent(Entitytwo).GetPosition();
					glm::vec3 ae = collisiondata.GetComponent(Entityone).GetSize();
					glm::vec3 be = collisiondata.GetComponent(Entitytwo).GetSize();

					if (abs(a[0] - b[0]) > (ae[0] + be[0])) return false;
					if (abs(a[1] - b[1]) > (ae[1] + be[1])) return false;
					if (abs(a[2] - b[2]) > (ae[2] + be[2])) return false;
					return true;
				}
			}
			return false;

		}
	}
	void Collision(Entity& actor, Entity& otheractor, ComponentManager<PositionComponent>& positions, ComponentManager<MovementComponent>& speed) {
		// Calculate the vector between the centers of the two objects
		glm::vec3 speed1 = speed.GetComponent(actor.GetId()).GetVelocity();
		glm::vec3 speed2 = speed.GetComponent(otheractor.GetId()).GetVelocity();
		float mass1 = speed.GetComponent(actor.GetId()).GetMass();
		float mass2 = speed.GetComponent(otheractor.GetId()).GetMass();
		glm::vec3 pos1 = positions.GetComponent(actor.GetId()).GetPosition();
		glm::vec3 pos2 = positions.GetComponent(otheractor.GetId()).GetPosition();
		glm::vec3 distanceVector = pos1 - pos2;
		float distance = glm::length(distanceVector);

		// Calculate combined radii of the two objects
		float combinedRadius = 2;

		// If distance is less than combined radii, there is a collision
		if (distance < combinedRadius && distance > 0) {
			// Normalize the collision normal (unit vector in the direction of separation)
			glm::vec3 collisionNormal = glm::normalize(distanceVector);

			// Calculate the penetration depth
			float penetrationDepth = combinedRadius - distance;

			// Resolve the overlap by moving the objects apart proportional to their masses
			glm::vec3 separation = collisionNormal * (penetrationDepth * 0.5f); // Split separation equally
			pos1 += separation * (mass2 / (mass1 + mass2));
			pos2 -= separation * (mass1 / (mass1 + mass2));

			// Exchange velocities based on the masses of the objects (elastic collision)
			glm::vec3 V1 = ((mass1 - mass2) / (mass1 + mass2) * speed1)
				+ ((2 * mass2) / (mass1 + mass2) * speed2);
			glm::vec3 V2 = ((mass2 - mass1) / (mass1 + mass2) * speed2)
				+ ((2 * mass1) / (mass1 + mass2) * speed1);

			speed1 = V1;
			speed2 = V2;

			// Apply damping to the velocities to avoid infinite bouncing
			speed1 *= 0.9f; // Assuming friction value for damping (e.g., 0.9)
			speed2 *= 0.9f;

			// Ensure separation to prevent objects from getting stuck together
			if (penetrationDepth > 0.0f) {
				pos1 += collisionNormal * (penetrationDepth * 0.01f);
				pos2 -= collisionNormal * (penetrationDepth * 0.01f);
			}

			// Update the components back to their respective systems
			positions.GetComponent(actor.GetId()).SetPosition(pos1);
			positions.GetComponent(otheractor.GetId()).SetPosition(pos2);

			speed.GetComponent(actor.GetId()).SetVelocity(speed1);
			speed.GetComponent(otheractor.GetId()).SetVelocity(speed2);
			
		}
	}
	void PickUpHandler(ComponentManager<PickUpComponent>& pickup, Entity& entity)
	{
		//Borrowed Code for Ideas

		//switch (pickup.GetComponent()) {
		//case PickUp::Type::Health:
		//	std::cout << "Player picked up health: " << pickup.value << "\n";
		//	// Add health logic here
		//	break;
		//case PickUp::Type::Ammo:
		//	std::cout << "Player picked up ammo: " << pickup.value << "\n";
		//	// Add ammo logic here
		//	break;
		//case PickUp::Type::Points:
		//	std::cout << "Player picked up points: " << pickup.value << "\n";
		//	// Add points logic here
		//	break;
		//case PickUp::Type::PowerUp:
		//	std::cout << "Player picked up a power-up!\n";
		//	// Add power-up logic here
		//	break;
		//}
	}


};

