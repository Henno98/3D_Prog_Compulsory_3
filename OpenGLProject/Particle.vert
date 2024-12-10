#version 330 core

layout(location = 0) in vec3 aPosition; // Vertex position (from Particle.vertex.position)
layout(location = 1) in vec3 aColor;    // Vertex color (from Particle.vertex.Color)

out vec3 fragColor; // Pass color to fragment shader
// Outputs the current position for the Fragment Shader
out vec3 crntPos;
uniform mat4 model;      // Model transformation
uniform mat4 camMatrix;

void main() {
    fragColor = aColor; // Pass vertex color to the fragment shader
    gl_Position = camMatrix * model * vec4(aPosition, 1.0);
    gl_PointSize = 10.0; // Set size for point rendering
}