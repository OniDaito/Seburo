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

// Oculus specific uniforms
uniform vec2 uLensCenter;
uniform vec2 uScreenCenter;
uniform vec2 uScale;
uniform vec2 uScaleIn;
uniform vec4 uHmdWarpParam;

// uScales input texture coordinates for distortion.
vec2 HmdWarp(vec2 in01) {
  vec2 theta = (in01 - uLensCenter) * uScaleIn; // uScales to [-1, 1]
  float rSq = theta.x * theta.x + theta.y * theta.y;
  vec2 rvector = theta * (uHmdWarpParam.x + uHmdWarpParam.y * rSq +
    uHmdWarpParam.z * rSq * rSq + uHmdWarpParam.w * rSq * rSq * rSq);
  
  return uLensCenter + uScale * rvector;
}

vec4 oculus(in vec4 oPosition, in vec2 oTexCoord) {
  vec2 tc = HmdWarp(oTexCoord);
  
  // Not sure abiout this just yet
  //if ( clamp(tc, uScreenCenter-vec2(0.25,0.5), uScreenCenter+vec2(0.25, 0.5))  ))
  //  return vec4(0.0,0.0,0.0,1.0);

  return texture(uTexSampler0, tc);
}

void main() {

  //vec4 colour = oculus(vVertexPosition, vTexCoord );
  vec2 texsize = textureSize(uTexSampler0); 

  // This texture comes from an FBO so flip it
  vec2 tc = vTexCoord * texsize;
  tc.y = texsize.y - tc.y;
  vec4 texcolor = texture(uTexSampler0,tc);
  fragColor = vec4(texcolor.rgb,1.0);
  //fragColor = colour;
  //fragColor= vec4(1.0,0.0,0.0,1.0);
}