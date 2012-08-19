#version 330
precision highp float;

out vec4 vVertexPosition;
out vec2 vTexCoord;

layout (location = 0) in vec3 attribVertPosition; // 1 is the indicies
layout (location = 2) in vec2 attribVertTexCoord;

uniform mat4 uMVPMatrix;

void main() {            
    vVertexPosition = uMVPMatrix * vec4(attribVertPosition,1.0);
    gl_Position = vVertexPosition;
    vTexCoord = attribVertTexCoord;
} 

