#version 420 compatibility

out vec4 vVertexPosition;
out vec4 vColour;

layout (location = 0) in vec3 attribVertPosition; 
layout (location = 1) in vec3 attribVertNormal;
layout (location = 2) in vec4 attribVertColour;
layout (location = 3) in vec2 attribVertTexCoord;

uniform mat4 uMVPMatrix;

void main() {            
    vVertexPosition = uMVPMatrix * vec4(attribVertPosition,1.0);
    gl_Position = vVertexPosition;
	vColour = attribVertColour;
} 
