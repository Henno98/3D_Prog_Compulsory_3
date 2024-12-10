#pragma once
#include "glad/glad.h"
#include "glm/vec3.hpp"

class Vertex
{
public:
    glm::vec3 position = glm::vec3(0.f);
    glm::vec3 Color = glm::vec3(1.f);
    glm::vec3 normal = glm::vec3(0.f);

    Vertex() = default;
    Vertex(glm::vec3 pos) : position(pos) {}
    Vertex(float x, float y, float z) : position(glm::vec3(x, y, z)) {}
    Vertex(glm::vec3 pos, glm::vec3 color) : position(pos), Color(color) {}
    Vertex(glm::vec3 pos, glm::vec3 color, glm::vec3 nor) : position(pos), normal(nor), Color(color) {}

    // Bind Vertex Attributes for the shader. Remember to change the shader to accept the corrent attributes
    static void BindAttributes() {
        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        

        // Color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
        

        // Normals
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
       

    }
};

struct Indices
{
public:
    GLuint V0, V1, V2;

};