#version 420 compatibility

out vec4 vLightPos;
out vec4 vVertexNormal;
out vec4 vVertexPosition;
out vec4 vTangentDir;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;
uniform mat4 uMInverseMatrix;
uniform vec3 uTangent;

// no indices for the gripper drawing

layout (location = 0) in vec3 attribVertPosition; 
layout (location = 1) in vec3 attribNormal;

void main() {                              
	
	vVertexPosition = uMVMatrix * vec4(attribVertPosition,1.0);
	vVertexNormal = normalize(vec4(attribNormal,1.0) * uMInverseMatrix);
	vTangentDir = normalize(uMVMatrix * vec4(uTangent, 0.0));
	gl_Position = uMVPMatrix * vec4(attribVertPosition,1.0);
	
}
