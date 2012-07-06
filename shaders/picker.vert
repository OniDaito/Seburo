#version 420 compatibility

out vec4 vVertexPosition;

layout (location = 0) in vec3 attribVertPosition; // 1 is the indicies

uniform mat4 uMVPMatrix;

void main() {            
    vVertexPosition = uMVPMatrix * vec4(attribVertPosition,1.0);
    gl_Position = vVertexPosition;
} 

