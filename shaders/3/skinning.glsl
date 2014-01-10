##>VERTEX

#version 330
precision highp float;

out vec4 sVertexPosition;
out vec2 sTexCoord;

// Default layout from Seburo
// 16 being the geometry_max_bones that Seburo sets
// Follows the MD5Mesh way of doing things

layout (location = 0) in vec3 aVertPosition;
layout (location = 1) in vec3 aVertNormal;
layout (location = 2) in vec2 aVertTexCoord;
layout (location = 3) in vec3 aVertTangent;
layout (location = 4) in uvec4 aVertBoneIndex;
layout (location = 5) in vec4 aVertWeight;


// Defaults set by Seburo
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

// Skinning defaults from Seburo
uniform mat4 uBonePalette[128]; // Quite a lot! :O
uniform uint uNumBones;

vec3 qtransform( vec4 q, vec3 v ){ 
  return v + 2.0 * cross(cross(v, q.xyz ) + q.w*v, q.xyz);
}


void main() {            
  vec3 skinnedPosition = vec3(0.0,0.0,0.0);

  // Flatten out the loop
  // Unpack the values
  float bias = aVertWeight.x;
  vec4 bp = vec4(aVertPosition,1.0) * uBonePalette[aVertBoneIndex.x] * bias;
  skinnedPosition += bp.xyz;

  bias = aVertWeight.y;
  bp = vec4(aVertPosition,1.0) * uBonePalette[aVertBoneIndex.y] * bias;
  skinnedPosition += bp.xyz;

  bias = aVertWeight.z;
  bp = vec4(aVertPosition,1.0) * uBonePalette[aVertBoneIndex.z] * bias;
  skinnedPosition += bp.xyz;

  bias = aVertWeight.w;
  bp = vec4(aVertPosition,1.0) * uBonePalette[aVertBoneIndex.w] * bias;
  skinnedPosition += bp.xyz;

  sVertexPosition = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(skinnedPosition,1.0);
  gl_Position = sVertexPosition;
  sTexCoord = aVertTexCoord;
} 

##>FRAGMENT

#version 330
precision highp float;

in vec4 sVertexPosition;
in vec2 sTexCoord;

out vec4 fragColor;

///\todo choose between both in the uber shader
// At present rect works well but we are sending normalised coordinates
//uniform sampler2DRect uTexSampler0;
uniform sampler2D uTexSampler0;

void main() {
  //vec2 texsize = textureSize(uTexSampler0); 
  vec4 texcolor = texture(uTexSampler0, sTexCoord);
  fragColor = vec4(texcolor.rgb,1.0);
}
