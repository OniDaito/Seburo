#version 420 compatibility

out vec4 vertex_light_position;
out vec4 vertex_normal;
out vec4 vertex_position;
out vec4 tangent_direction;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;
uniform mat4 uMInverseMatrix;
uniform vec3 uTangent;

// no indices for the gripper drawing

layout (location = 0) in vec3 attribVertPosition; 
layout (location = 1) in vec3 attribNormal;

void main() {                              
	
	vertex_position = uMVMatrix * vec4(attribVertPosition,1.0);
	vertex_normal = normalize(vec4(attribNormal,1.0) * uMInverseMatrix);
	tangent_direction = normalize(uMVMatrix * vec4(uTangent, 0.0));
	gl_Position = uMVPMatrix * vec4(attribVertPosition,1.0);
	
}
