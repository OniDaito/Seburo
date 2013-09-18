#version 420

in vec4 vVertexPosition;
in vec4 vColour;
in vec2 vTexCoord;

uniform sampler2DRect uBaseTex;

out vec4 fragColor;

void main() {
	vec4 texcolor = texture(uBaseTex,vTexCoord);
	fragColor = texcolor;
}
