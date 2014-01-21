#version 330
precision highp float;

out vec4 vVertexPosition;
out vec4 vColour;
out vec2 vTexCoord;

layout (location = 0) in vec3 aVertPosition; 
layout (location = 1) in vec3 aVertNormal;
layout (location = 2) in vec3 aVertColour;
layout (location = 3) in vec2 aVertTexCoord;
layout (location = 4) in vec3 aVertTangent; 

// Defaults set by Seburo
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main() {            
  vVertexPosition = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aVertPosition,1.0f);
  gl_Position = vVertexPosition;
  vColour = vec4(aVertColour,1.0f);
  vTexCoord = aVertTexCoord;
} 
