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
		glm::vec3 distance = spheremin - spheremax;
		if (diameter <= sphere.radius*2 && diameter > 0) {
			sphere.Speed = reflect(sphere.Speed, normalize(distance));
		}
	
}

void Collision::CollideWithWall(Cube& otheractor, Sphere& sphere)
{

	glm::vec3 min = otheractor.AABB.Position - otheractor.AABB.Extent;
	glm::vec3 max = otheractor.AABB.Position + otheractor.AABB.Extent;
	glm::vec3 spheremin = sphere.SphereMatrix[3];
	glm::vec3 closestpoint = glm::clamp(spheremin, min, max);
	glm::vec3 distance = spheremin - closestpoint;

	sphere.Speed = reflect(sphere.Speed, normalize(distance));
}
