#version 150 core

vec4 vLightPos;
vec4 vVertexNormal;
vec4 vVertexPosition;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNMatrix;
uniform vec3 uLight0;


glBindAttribLocation(programHandler, 0, "InPosition");
glBindAttribLocation(programHandler, 0, "InPosition");

layout (location = 0) in vec3 attribVertPosition;
layout (location = 1) in vec3 attribNormal;

// Basic Phong Shading

void main() {            
    vVertexNormal = normalize(uNMatrix * vec4(attribNormal,1.0));
    vLightPos = normalize( vec4(uLight0,0.0));
    vVertexPosition = vec4(attribVertPosition,1.0);
    gl_Position = uMVPMatrix * vVertexPosition;

} 