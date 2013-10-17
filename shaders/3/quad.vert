#version 330
precision highp float;

out vec4 vVertexPosition;
out vec4 vColour;

layout (location = 0) in vec4 attribVertPosition; 
layout (location = 1) in vec4 attribVertNormal;
layout (location = 2) in vec4 attribVertColour;
layout (location = 3) in vec2 attribVertTexCoord;
layout (location = 4) in vec4 attribVertTangent; 

uniform mat4 uMVPMatrix;

void main() {            
  vVertexPosition = attribVertPosition;
  gl_Position = uMVPMatrix * vVertexPosition;
  vColour = attribVertColour;
} 
