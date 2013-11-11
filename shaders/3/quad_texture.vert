#version 330
precision highp float;

out vec4 vVertexPosition;
out vec2 vTexCoord;
out vec4 vColour;

// Default layout from Seburo
layout (location = 0) in vec3 aVertPosition;
layout (location = 1) in vec3 aVertNormal;
layout (location = 2) in vec4 aVertColour;
layout (location = 3) in vec2 aVertTexCoord;

// Defaults set by Seburo
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;


void main() {            
  vVertexPosition = uProjectionMatrix * uViewMatrix  * uModelMatrix * vec4(aVertPosition,1.0);
  gl_Position = vVertexPosition;
	vColour = aVertColour;
	vTexCoord = aVertTexCoord;
} 
