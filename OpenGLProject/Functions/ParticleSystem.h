#pragma once
#include <vector>
#include "../Shaders/ShaderClass.h"
#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "../Vertex.h"
#include "glm/gtc/type_ptr.hpp"
#include "../Shaders/ObjectBinders.h"


class ParticleSystem
{
public:
    struct Particle {
        glm::vec3 position;      // Particle represented as a Vertex (position, color, normal)
        glm::vec3 Color; 
    	glm::vec3 velocity; // Current velocity of the particle
        float lifetime;     // Remaining lifetime of the particle
        bool active;        // If true, particle is active
    };
    std::vector<Particle> particles; // Pool of particles
    GLuint VAO, VBO;                 // OpenGL buffers
    size_t maxParticles;             // Maximum number of particles
    glm::vec3 spawnArea;             // Area for spawning particles
    glm::mat4 Matrix = glm::mat4(1.f);
    ObjectBinders Binders;

    // Constructor
    ParticleSystem(size_t maxParticles, glm::vec3 spawnArea) : maxParticles(maxParticles), spawnArea(spawnArea)
	{  for (int i = 0; i < maxParticles; i++) {
            particles.emplace_back();
        }
        InitializeParticles();
        SetupBuffers();
      
    }

    void UpdateParticles(float deltaTime) {
        for (auto& particle : particles) {
            // Apply velocity to position
            particle.position += particle.velocity * deltaTime;

            // Apply gravity to velocity
            particle.velocity += glm::vec3(0.0f, -9.8f, 0.0f) * deltaTime;

            // Decrease lifetime
            particle.lifetime -= deltaTime;

            // Respawn if necessary
            if (particle.lifetime <= 0.0f || particle.position.y < -10.0f) {
                RespawnParticle(particle);
            }
        }

        // Update GPU buffer with new vertex data
        std::vector<Vertex> vertices;
        vertices.reserve(particles.size());
        for (const auto& particle : particles) {
            vertices.emplace_back(particle.position,particle.Color);
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
    }

    void RenderParticles(Shader& shader) {
        glUseProgram(shader.ID);

        glBindVertexArray(VAO);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(Matrix));
        // Draw all particles
        glPointSize(10.f);
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(particles.size()));

        glBindVertexArray(0);
    }

private:
    void InitializeParticles() {
        particles.resize(maxParticles);
        for (auto& particle : particles) {
            particle.position = RandomPosition();
            particle.Color = glm::vec3(0.0f, 0.5f, 1.0f); // Light blue
            particle.velocity = glm::vec3(0.0f, -1.0f, 0.0f);    // Falling velocity
            particle.lifetime = RandomLifetime();
            particle.active = true;
        }
    }

    void SetupBuffers() {

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Vertex), &particles[0].position, GL_DYNAMIC_DRAW);

        Vertex::BindAttributes();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void RespawnParticle(Particle& particle) {
        particle.position = RandomPosition(); // Respawn above
       // particle.vertex.Color = glm::vec3(0.0f, 0.5f, 1.0f); // Reset color
        particle.velocity = glm::vec3(0.0f, -1.0f, 0.0f);    // Reset velocity
        particle.lifetime = RandomLifetime();               // Reset lifetime
        particle.active = true;
    }

    glm::vec3 RandomPosition() {
        return glm::vec3(
            (static_cast<float>(rand()) / RAND_MAX - 0.5f) * spawnArea.x,
            (static_cast<float>(rand()) / RAND_MAX) * spawnArea.y, // Only spawn above
            (static_cast<float>(rand()) / RAND_MAX - 0.5f) * spawnArea.z
        );
        
    }

    float RandomLifetime() {
        return static_cast<float>(rand()) / RAND_MAX * 5.0f + 1.0f; // Lifetime between 1 and 5 seconds
    }
};