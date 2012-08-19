#version 330
precision highp float;

out vec4 vVertexPosition;
out vec4 vColour;

layout (location = 0) in vec3 attribVertPosition;
layout (location = 1) in vec4 attribColour; 

uniform mat4 uMVPMatrix;

void main() {            
    vVertexPosition = uMVPMatrix * vec4(attribVertPosition,1.0);
    vColour = attribColour;
    gl_Position = vVertexPosition;
} 

