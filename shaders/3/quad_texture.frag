#version 330
precision highp float;

in vec4 vVertexPosition;
in vec4 vColour;
in vec2 vTexCoord;

out vec4 fragColor;

uniform sampler2DRect uBaseTex;

void main() {
	vec4 texcolor = texture(uBaseTex,vTexCoord);
	fragColor = texcolor;
}
