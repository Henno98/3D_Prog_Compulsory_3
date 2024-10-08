#include "Collision.h"

Collision::Collision()
{

}

void Collision::CollideWithBall(Sphere& otheractor, Sphere& sphere)
{
	
	

		glm::vec3 min = otheractor.AABB.Position - otheractor.AABB.Extent;
		glm::vec3 max = otheractor.AABB.Position + otheractor.AABB.Extent;
		glm::vec3 spheremin = sphere.SphereMatrix[3];
		glm::vec3 spheremax = otheractor.SphereMatrix[3];
		glm::vec3 closestpoint = glm::clamp(spheremin, min, max);
		float diameter = glm::distance(spheremin, spheremax);
		glm::vec3 distance = spheremin - closestpoint;
		if (diameter <= sphere.radius*2 && diameter > 0) {

			sphere.SphereMatrix = glm::translate(sphere.SphereMatrix, glm::vec3( distance.x/2, distance.y/2, distance.z/2));
			//otheractor.SphereMatrix = glm::translate(otheractor.SphereMatrix, distance);
			glm::vec3 V1 = ((sphere.mass - otheractor.mass) / (sphere.mass + otheractor.mass) * sphere.Speed)
				+ ((2 * otheractor.mass) / (sphere.mass + otheractor.mass) * otheractor.Speed);
			glm::vec3 V2 = ((sphere.mass * 2) / (sphere.mass + otheractor.mass) * sphere.Speed)
				+ ((otheractor.mass - sphere.mass) / (sphere.mass + otheractor.mass) * otheractor.Speed);
			sphere.Speed = V1;
			otheractor.Speed = V2;
		}
	
}

void Collision::CollideWithWall(Cube& otheractor, Sphere& sphere)
{

	glm::vec3 min = otheractor.AABB.Position - otheractor.AABB.Extent;
	glm::vec3 max = otheractor.AABB.Position + otheractor.AABB.Extent;
	glm::vec3 spheremin = sphere.SphereMatrix[3];
	glm::vec3 closestpoint = glm::clamp(spheremin, min, max);
	glm::vec3 distance = spheremin - closestpoint;
	sphere.SphereMatrix = glm::translate(sphere.SphereMatrix, glm::vec3(distance.x/2, distance.y/2,distance.z/2));
	sphere.Speed = reflect(sphere.Speed, normalize(distance));
}
