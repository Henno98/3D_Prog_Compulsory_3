#pragma once

#include "../Shaders/ShaderClass.h"
#include <glm/glm.hpp>
#include "../BoundingBox.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <vector>

#include "Cube.h"


struct Vertex {
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 Color = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 normal = glm::vec3(0.f);

    Vertex() = default;
    Vertex(glm::vec3 pos) : position(pos) {}
    Vertex(float x, float y, float z) : position(glm::vec3(x, y, z)) {}
    Vertex(glm::vec3 pos, glm::vec3 nor) : position(pos), normal(nor) {}
    Vertex(glm::vec3 pos, glm::vec3 nor, glm::vec3 tex) : position(pos), normal(nor), Color(tex) {}

    // Bind Vertex Attributes for the shader. Remember to change the shader to accept the corrent attributes
    static void BindAttributes() {
        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
        glEnableVertexAttribArray(1);

        // Texture Coordinates
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(2);
        /*
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, mTexCoord));
        glEnableVertexAttribArray(2);*/
    }

};

class Sphere
{
public:
    int ID;
	glm::mat4 SphereMatrix = glm::mat4(1.0f);
	std::vector<Vertex> sphere;
	BoundingBox AABB;
	float radius;
	int subdivision;
    glm::vec3 Speed;
    VBO sphereVBO;
	VAO sphereVAO;
    bool bound = false;
	Sphere();
	void DrawSphere( Shader& shader, const char* uniform);
	void CreateSphere(int id, int subdivison, float scale, glm::vec3 speed);
	void Subdivide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, int n);
	void CreateTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    void Movement();
    void CollideWithBall( std::vector<Sphere>& collisions);
    void CollideWithWall(Cube& otheractor);
    void DeleteVAO();

};

