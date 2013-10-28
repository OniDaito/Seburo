#version 330
precision highp float;

in vec4 vVertexPosition;
in vec4 vColour;
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
