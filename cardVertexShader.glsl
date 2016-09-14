#version 330 core

layout(location = 0) in vec3 vertexIn;
layout(location = 1) in vec2 positionIn;
layout(location = 2) in vec3 colorIn;

out vec3 colorVertexShader;

void main() {
    gl_Position = vec4(vertexIn + vec3(positionIn, -0.125), 1.0);
    colorVertexShader = colorIn;
}
