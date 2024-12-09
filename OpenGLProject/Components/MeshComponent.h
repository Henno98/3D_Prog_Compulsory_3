#pragma once
#include <iostream>
#include <vector>


#include "PositionComponent.h"
#include "../Vertex.h"
#include "../Dependencies/includes/glm/glm.hpp"
#include "../Shaders/ObjectBinders.h"
#include "../BoundingBox.h"
#include "../Shaders/ShaderClass.h"
#include "glm/gtx/transform.hpp"

enum MeshType
{

	Pyramid,
	Cube,
	Sphere,
	Line,
	unknown,
	Custom
};
class MeshComponent
{
public:
	std::vector<Vertex> Vertices;
	std::vector<Indices> indicies;
	std::vector<Vertex> BallLineStrip;
	ObjectBinders Binders;
	BoundingBox collider;
	glm::mat4 Matrix = glm::mat4(1.f);
	MeshType Mesh;
	float Mass{ 1.f };
	float Radius{ 1.f };
	glm::vec3 Position;
	glm::vec3 Extent;
	MeshComponent(MeshType meshtype, glm::vec3 extent,glm::vec3 pos) :Mesh(meshtype), Extent(extent),Position(pos)
	{
		
		switch (Mesh) {
		case Pyramid:
			CreateTriangle();
			break;
		case Cube:
			CreateCube();
			break;
		case Sphere:
			CreateSphere(3, 1.f, glm::vec3(1.f));
			break;
		case Custom:
			std::cout << "Call on the CustomCreate function" << std::endl;
			break;
		case Line:
			std::cout << "Call on the CustomCreate function" << std::endl;
			break;
		default:
			std::cout << "Mesh type does not exist" << std::endl;
		}
	};

	void CreateTriangle();
	void CreateCube();
	void CreateSphere(int subdivisions, float scale, glm::vec3 speed);
	void Subdivide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, int n);
	void CreateTriangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);
	void CustomCreateTriangle(std::vector<Vertex>& vertices, std::vector<Indices>& indices);
	void CustomCreateSpline(std::vector<Vertex>& vertices);
	void ReBind(std::vector<Vertex>& vertices);
	void CalculateNormals();
	void Draw(const char* uniform, Shader& shader);
	glm::mat4 MatrixCalc()
	{
		glm::mat4 newMatrix = glm::mat4(1.f);
		newMatrix = glm::translate(glm::mat4(1.f), Position);
		return newMatrix;
	}
};

