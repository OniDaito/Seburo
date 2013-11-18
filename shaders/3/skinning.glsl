##>VERTEX

#version 330
precision highp float;

out vec4 vVertexPosition;
out vec2 vTexCoord;

// Default layout from Seburo
// 16 being the geometry_max_bones that Seburo sets
// Follows the MD5Mesh way of doing things

layout (location = 0) in vec3 aVertPosition;
layout (location = 1) in vec3 aVertNormal;
layout (location = 2) in vec2 aVertTexCoord;
layout (location = 3) in vec3 aVertTangent;
layout (location = 4) in uvec4 aVertBoneIndex[2];
layout (location = 6) in vec4 aVertWeightPosBias[6];


// Defaults set by Seburo
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

// Skinning defaults from Seburo
uniform mat4 uBonePalette[128]; // Quite a lot! :O
uniform uint uNumBones;

vec3 qtransform( vec4 q, vec3 v ){ 
  return v + 2.0*cross(cross(v, q.xyz ) + q.w*v, q.xyz);
}


void main() {            
  vec3 skinnedPosition = vec3(0.0,0.0,0.0);

  // Flatten out the loop
  // Unpack the values
  float bias = aVertWeightPosBias[0].w;
  vec4 weight_position =  vec4(aVertWeightPosBias[0].xyz, 1.0);
  vec4 bp = uBonePalette[aVertBoneIndex[0].x] * weight_position;
  skinnedPosition += (vec3(bp.xyz) * bias);


  bias = aVertWeightPosBias[1].w;
  weight_position =  vec4(aVertWeightPosBias[1].xyz, 1.0);
  bp = uBonePalette[aVertBoneIndex[0].y] * weight_position;
  skinnedPosition += (vec3(bp.xyz) * bias);


  bias = aVertWeightPosBias[2].w;
  weight_position =  vec4(aVertWeightPosBias[2].xyz, 1.0);
  bp = uBonePalette[aVertBoneIndex[0].z] * weight_position;
  skinnedPosition += (vec3(bp.xyz) * bias);


  bias = aVertWeightPosBias[3].w;
  weight_position =  vec4(aVertWeightPosBias[3].xyz, 1.0);
  bp = uBonePalette[aVertBoneIndex[0].w] * weight_position;
  skinnedPosition += (vec3(bp.xyz) * bias);

  bias = aVertWeightPosBias[4].w;
  weight_position =  vec4(aVertWeightPosBias[4].xyz, 1.0);
  bp = uBonePalette[aVertBoneIndex[1].x] * weight_position;
  skinnedPosition += (vec3(bp.xyz) * bias);


  bias = aVertWeightPosBias[5].w;
  weight_position =  vec4(aVertWeightPosBias[5].xyz, 1.0);
  bp = uBonePalette[aVertBoneIndex[1].y] * weight_position;
  skinnedPosition += (vec3(bp.xyz) * bias);

  vVertexPosition = uProjectionMatrix * uViewMatrix  * uModelMatrix * vec4(skinnedPosition,1.0);
  gl_Position = vVertexPosition;
  vTexCoord = aVertTexCoord;
} 

##>FRAGMENT

#version 330
precision highp float;

in vec4 vVertexPosition;
in vec2 vTexCoord;

out vec4 fragColor;

///\todo choose between both in the uber shader
// At present rect works well but we are sending normalised coordinates
uniform sampler2DRect uBaseTex;
//uniform sampler2D uBaseTex;

void main() {
  vec2 texsize = textureSize(uBaseTex); 
  vec4 texcolor = texture(uBaseTex,vTexCoord * texsize);
  fragColor = vec4(texcolor.rgb,1.0);
}
