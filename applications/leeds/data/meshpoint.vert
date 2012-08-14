#version 420 compatibility

out vec4 vertex_position;

layout (location = 0) in vec3 attribVertPosition;

uniform mat4 uMVPMatrix;

void main() {            
    vertex_position = uMVPMatrix * vec4(attribVertPosition,1.0);
    gl_Position = vertex_position;
} 

