#include "Sphere.h"

#define M_PI 3.1415926535897932384626433832795

Sphere::Sphere()
{


}

void Sphere::DrawSphere(glm::vec3 scale, glm::vec3 color, Shader& shader, const char* uniform)
{


}

void Sphere::CreateSphere(glm::vec3 position, glm::vec3 scale, float subdivison, float angle, Shader& shader, const char* uniform)
{
	float sectorStep = 2 * M_PI / length;
	float stackStep = M_PI / width;
	

	for( int i = 0; i < length; i++)
	{
		for( int j = 0; j < width; j++)
		{
			

		}

	}

}

