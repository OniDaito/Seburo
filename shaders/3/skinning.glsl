##>VERTEX

#version 330
precision highp float;

out vec4 vVertexPosition;
out vec2 vTexCoord;

// Default layout from Seburo
// 16 being the geometry_max_bones that Seburo sets
layout (location = 0) in vec3 aVertPosition;
layout (location = 1) in vec3 aVertNormal;
layout (location = 2) in vec2 aVertTexCoord;
layout (location = 3) in vec3 aVertTangent;
layout (location = 4) in uint aVertBoneIndex[16];
layout (location = 5) in float aVertBoneWeight[16];

// Defaults set by Seburo
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main() {            
  vVertexPosition = uProjectionMatrix * uViewMatrix  * uModelMatrix * vec4(aVertPosition,1.0);
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
