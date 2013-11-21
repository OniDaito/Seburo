#version 330
precision highp float;

out vec4 vVertexPosition;
out vec4 vColour;

layout (location = 0) in vec4 aVertPosition; 
layout (location = 1) in vec4 aVertNormal;
layout (location = 2) in vec4 aVertColour;
layout (location = 3) in vec2 aVertTexCoord;
layout (location = 4) in vec4 aVertTangent; 

// Defaults set by Seburo
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main() {            
  vVertexPosition = uProjectionMatrix * uViewMatrix * uModelMatrix * aVertPosition;
  gl_Position = vVertexPosition;
  vColour = aVertColour;
} 
