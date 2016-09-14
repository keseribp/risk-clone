#version 330 core

in vec3 colorVertexShader;

out vec4 color;

void main() {
    color = vec4(colorVertexShader, 1.0);
}
