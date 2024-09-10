#include "Sphere.h"

#include <vector>

#include "Shaders/EBO.h"
#include "Shaders/VAO.h"
#include "Shaders/VBO.h"

#define M_PI 3.1415926535897932384626433832795

Sphere::Sphere()
{


}

void Sphere::DrawSphere( Shader& shader, const char* uniform)
{
	VAO planevao;
	planevao.Bind();
	VBO planevbo(reinterpret_cast<GLfloat*>(sphere.data()), (sphere.size() * sizeof(SphereVertex)));
	planevbo.Bind();
	EBO planeebo(reinterpret_cast<GLuint*>(Indices.data()), (Indices.size() * sizeof(SphereIndices)));
	planeebo.Bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(SphereMatrix));
	glDrawElements(GL_TRIANGLES, sizeof(Indices)/2, GL_UNSIGNED_INT, nullptr);

	planevao.Unbind();
	planevbo.Unbind();
	planeebo.Unbind();


}

void Sphere::CreateSphere( int subdivison, float Radius)
{
	radius = Radius;
	subdivision = subdivison;
	float sectorStep = 2 * M_PI / length;
	float stackStep = M_PI / width;
	

	for( int i = 0; i < subdivision; i++)
	{
		float stackangle = (M_PI / 2) - i * stackStep;
		float xy = radius * cos(stackangle);
		float z = radius * sin(stackangle);

		for( int j = 0; j < subdivision; j++)
		{
			float sectorAngle = j * sectorStep;
			float x = xy * cos(sectorAngle);
			float y = xy * sin(sectorAngle);

			sphere.push_back(SphereVertex{ x,y,z,x,0,y,0,0,0 });
		}

	}
	for (int i = 0; i < sphere.size()/3;i++)
	{
		for (int j = 0; j < sphere.size()/3; j++)
		{
			unsigned int v0 = i * j;
			unsigned int v1 = i +1 *j;
			unsigned int v2 = i + subdivision *j;
			unsigned int v3 = i + subdivision * j + 1;


			Indices.push_back(SphereIndices{ v0,v1,v2 });
			Indices.push_back(SphereIndices{ v3,v1,v2 });



		}

	}

}

