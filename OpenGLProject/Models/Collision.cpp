#include "Collision.h"

Collision::Collision()
{

}

void Collision::CollideWithBall(std::vector<Sphere>& collisions, Sphere& sphere)
{
	for (int i = 0; i < collisions.size(); i++)
	{

		glm::vec3 min = collisions[i].AABB.Position - collisions[i].AABB.Extent;
		glm::vec3 max = collisions[i].AABB.Position + collisions[i].AABB.Extent;
		glm::vec3 spheremin = sphere.SphereMatrix[3];
		glm::vec3 spheremax = collisions[i].SphereMatrix[3];

		glm::vec3 closestpoint = glm::clamp(spheremin, min, max);
		float diameter = glm::distance(spheremin, spheremax);
		glm::vec3 distance = spheremin - spheremax;
		if (diameter <= sphere.radius && diameter > 0) {
			sphere.Speed = reflect(sphere.Speed, normalize(distance));
		}
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
