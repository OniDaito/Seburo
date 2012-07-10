#version 420 compatibility

out vec4 vVertexNormal;
out vec4 vVertexPosition;

uniform mat4 uMVPMatrix;
uniform mat4 uNormalMatrix;
uniform mat4 uMVMatrix;

layout (location = 0) in vec3 attribVertPosition; 
layout (location = 2) in vec3 attribNormal;

void main() {                              
	
	vVertexPosition = uMVMatrix * vec4(attribVertPosition,1.0);
	vVertexNormal = vec4(attribNormal,1.0); //normalize(vec4(attribNormal,1.0) * uNormalMatrix);
	gl_Position = uMVPMatrix * vec4(attribVertPosition,1.0);
	
}
