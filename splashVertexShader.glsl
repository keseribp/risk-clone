#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 squareVertices;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

//uniform vec2 UVMod; 

void main(){
    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  vec4(squareVertices, 1.0);

    // UV of the vertex. No special space for this one.
    UV = 0.5 * squareVertices.xy + vec2(0.5, 0.5);
    //UV = vec2(UVMod.x * (squareVertices.x + 1.0f), UVMod.y * (squareVertices.y + 1.0f)); // UVMod is encoded this way
    //color
}
