##>VERTEX

#version 330
precision highp float;

out vec4 vVertexPosition;
out vec2 vTexCoord;

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
  vVertexPosition = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aVertPosition,1.0);
  gl_Position = vVertexPosition;
  vTexCoord = aVertTexCoord;
}

##>FRAGMENT

#version 330
precision highp float;

in  vec4 vVertexPosition;
in  vec2 vTexCoord;
out vec4 fragColor;

uniform sampler2DRect uTexSampler0;

void main() {
  vec2 texsize = textureSize(uTexSampler0); 

  // This texture comes from an FBO so flip it
  vec2 tc = vTexCoord * texsize;
  tc.y = texsize.y - tc.y;
  vec4 texcolor = texture(uTexSampler0,tc);
  fragColor = vec4(texcolor.rgb,1.0);
}