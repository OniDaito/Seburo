#version 330
precision highp float;

in vec4 vVertexPosition;
in vec4 vColour;
in vec2 vTexCoord;

out vec4 fragColor;

///\todo choose between both in the uber shader
// At present rect works well but we are sending normalised coordinates
uniform sampler2DRect uTexSampler0;
//uniform sampler2D uTexSampler0;

void main() {
  vec2 texsize = textureSize(uTexSampler0); 
	vec4 texcolor = texture(uTexSampler0,vTexCoord * texsize);
	fragColor = vec4(texcolor.rgb,1.0);
}
