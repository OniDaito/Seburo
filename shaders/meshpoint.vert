#version 420 compatibility

out vec4 vertex_position;

layout (location = 0) in vec3 attribVertPosition; // 1 is the indicies

uniform mat4 mMVPMatrix;

void main() {            
    vertex_position = mMVPMatrix * vec4(attribVertPosition,1.0);
    gl_Position = vertex_position;
} 

