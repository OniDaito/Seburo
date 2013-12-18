#version 150

// https://github.com/dghost/glslRiftDistort

uniform sampler2DRect uTexSampler0;

invariant in vec2 sLensCenter;
invariant in vec2 sScreenCenter;

// Oculus specific uniforms
uniform vec2 uScale = vec2(0.25,0.5);
uniform vec2 uScaleIn = vec2(4.0,2.0);
uniform vec4 uHmdWarpParam  = vec4(1.0,0.22,0.24,0.0);
uniform vec4 uChromAbParam;

uniform float uDistortionScale = 0.8;

in vec2 sTexCoord;

//layout(location = 0) out vec4 outColor; // GLSL 3.30 or higher only

out vec4 sOutColour; // GLSL 1.50 or higher

// Performs Barrel distortion and Chromatic Abberation correction

void main(void)
{
  vec2 tex_size = textureSize(uTexSampler0); 
  vec2 tc = sTexCoord;
  tc.y = 1.0 - tc.y;

  vec2 theta = (tc - sLensCenter) * uScaleIn; // uScales to [-1, 1]
  float rSq = theta.x * theta.x + theta.y * theta.y;
  vec2 rvector= theta * ( uHmdWarpParam.x + uHmdWarpParam.y * rSq +
    uHmdWarpParam.z * rSq * rSq 
    + uHmdWarpParam.w * rSq * rSq * rSq
    );

  vec2 theta_blue = rvector * (uChromAbParam.z + uChromAbParam.w * rSq);
  vec2 tc_blue = sLensCenter + uScale * uDistortionScale * theta_blue;
  if (!all(equal(clamp(tc_blue, sScreenCenter-vec2(0.25,0.5), sScreenCenter+vec2(0.25,0.5)), tc_blue))) {
    sOutColour = vec4(0);
    return;
  }
  

  float blue = texture(uTexSampler0, tc_blue * tex_size).b;

  vec2  tc_green = sLensCenter + uScale * uDistortionScale * rvector;
  vec4  center = texture(uTexSampler0, tc_green * tex_size);

  vec2  theta_red = rvector * (uChromAbParam.x + uChromAbParam.y * rSq);
  vec2  tc_red = sLensCenter + uScale * uDistortionScale * theta_red;
  float red = texture(uTexSampler0, tc_red * tex_size).r;


  sOutColour = vec4(red, center.g, blue, center.a);

  //sOutColour = texture(uTexSampler0, tc * tex_size);
}
