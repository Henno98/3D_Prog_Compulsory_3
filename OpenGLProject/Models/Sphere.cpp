#include "Sphere.h"

#include <vector>

#include "../Shaders/EBO.h"
#include "../Shaders/VAO.h"
#include "../Shaders/VBO.h"

#define M_PI 3.1415926535897932384626433832795
const float H_Angle = M_PI / 180 * 72;
const float V_Angle = glm::atan(1.0f / 2);

Sphere::Sphere()
{


}

void Sphere::DrawSphere( Shader& shader, const char* uniform)
{
	
	VAO planevao;
	planevao.Bind();
	VBO planevbo(reinterpret_cast<GLfloat*>(sphere.data()), (sphere.size() * sizeof(Vertex)));
	planevbo.Bind();
	EBO planeebo(reinterpret_cast<GLuint*>(sphereIndices.data()), (sphereIndices.size() * sizeof(SphereIndices)));
	planeebo.Bind();
	Vertex::BindAttributes();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(SphereMatrix));
	glDrawArrays(GL_TRIANGLES, 0, sphere.size());

	planevao.Unbind();
	planevbo.Unbind();
	planeebo.Unbind();

	planevao.Delete();
	planevbo.Delete();
	planeebo.Delete();
	

}

void Sphere::CreateSphere(int id, int subdivison, float scale, glm::vec3 speed)
{
	glm::vec3 v0(0, 0, 1);
	glm::vec3 v1(1, 0, 0);
	glm::vec3 v2(0, 1, 0);
	glm::vec3 v3(-1, 0, 0);
	glm::vec3 v4(0, -1, 0);
	glm::vec3 v5(0, 0, -1);
	ID = id;
	subdivision = subdivison;
	radius = scale;
	Speed = speed;
	Subdivide(v0, v1, v2, subdivision);
	Subdivide(v0, v2, v3, subdivision);
	Subdivide(v0, v3, v4, subdivision);
	Subdivide(v0, v4, v1, subdivision);

	Subdivide(v5, v2, v1, subdivision);
	Subdivide(v5, v3, v2, subdivision);
	Subdivide(v5, v4, v3, subdivision);
	Subdivide(v5, v1, v4, subdivision);
	for (auto& Vertex : sphere)
	{
		Vertex.position *= scale;
	}
	
	AABB.Extent = glm::vec3(scale);
}

void Sphere::Subdivide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, int n)
{
	if(n > 0)
	{
		glm::vec3 v1 = a + b;
		v1 = glm::normalize(v1);
		glm::vec3 v2 = a + c;
		v2 =glm::normalize(v2);
		glm::vec3 v3 = c + b;
		v3= glm::normalize(v3);
		Subdivide(a, v1, v2, n - 1);
		Subdivide(c, v2, v3, n - 1);
		Subdivide(b, v3, v1, n - 1);
		Subdivide(v3, v2, v1, n - 1);
	}
	else
	{
		CreateTriangle(a, b, c);

	}
}

void Sphere::CreateTriangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
{
	Vertex V{glm::vec3( v1.x,v1.y,v1.z),glm::vec3(v1.x,v1.y,v1.z) ,glm::vec3(v1.x,v1.y,v1.z )};
	sphere.emplace_back(V);
	V = Vertex{ glm::vec3(v2.x,v2.y,v2.z),glm::vec3(v2.x,v2.y,v2.z) ,glm::vec3(v2.x,v2.y,v2.z) };
	sphere.emplace_back(V);
	V = Vertex{ glm::vec3(v3.x,v3.y,v3.z),glm::vec3(v3.x,v3.y,v3.z) ,glm::vec3(v3.x,v3.y,v3.z) };
	sphere.emplace_back(V);


}

void Sphere::Movement()
{
	SphereMatrix = glm::translate(SphereMatrix, Speed);
	AABB.Position = SphereMatrix[3];
}

void Sphere::CollideWithBall( std::vector<Sphere>& collisions)
{
	for (int i = 0; i < collisions.size(); i++)
	{

		glm::vec3 min = collisions[i].AABB.Position - collisions[i].AABB.Extent;
		glm::vec3 max = collisions[i].AABB.Position + collisions[i].AABB.Extent;
		glm::vec3 spheremin = SphereMatrix[3];
		glm::vec3 spheremax = collisions[i].SphereMatrix[3];

		glm::vec3 closestpoint = glm::clamp(spheremin, min, max);
		float diameter = glm::distance(spheremin, spheremax);
		glm::vec3 distance = spheremin - spheremax;
		if (diameter <= radius && diameter>0) {
			Speed = reflect(Speed, normalize(distance));
		}
	}
}

void Sphere::CollideWithWall(Cube& otheractor)
{
	//otheractor.Speed = reflect(otheractor.Speed, normalize(distance));
		
		glm::vec3 min = otheractor.AABB.Position-otheractor.AABB.Extent;
		glm::vec3 max = otheractor.AABB.Position + otheractor.AABB.Extent;
		glm::vec3 spheremin = SphereMatrix[3];
		glm::vec3 closestpoint = glm::clamp(spheremin, min, max);
		glm::vec3 distance = spheremin - closestpoint;

	Speed = reflect(Speed, normalize(distance));

	
}

