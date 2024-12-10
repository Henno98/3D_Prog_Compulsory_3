#version 330 core

in vec3 fragColor; // Interpolated color from the vertex shader
out vec4 FragColor;

void main() {
    // Simple point shading: Set the output color as the particle's color
    FragColor = vec4(fragColor, 1.0);
}