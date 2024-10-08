#include "Sphere.h"

#include <vector>

#include "../Shaders/EBO.h"
#include "../Shaders/VAO.h"
#include "../Shaders/VBO.h"



Sphere::Sphere()
{


}

void Sphere::DrawSphere( Shader& shader, const char* uniform)
{
	
	
	sphereVAO.Bind();
	sphereVBO.Bind();
	Vertex::BindAttributes();
	
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(SphereMatrix));
	glDrawArrays(GL_TRIANGLES, 0, sphere.size());

	sphereVAO.Unbind();
	sphereVBO.Unbind();
	

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
	sphereVAO.Initialize();
	sphereVBO.Initialize(reinterpret_cast<GLfloat*>(sphere.data()), (sphere.size() * sizeof(Vertex)));
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
	
}

void Sphere::CollideWithWall(Cube& otheractor)
{
	
}

void Sphere::DeleteVAO()
{
	sphereVAO.Delete();
	sphereVBO.Delete();
}

