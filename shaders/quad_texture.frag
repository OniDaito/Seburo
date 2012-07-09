#version 420 compatibility

in vec4 vVertexPosition;
in vec4 vColour;
in vec2 vTexCoord;

uniform sampler2DRect uBaseTex;

void main() {
	vec4 texcolor = texture(uBaseTex,vTexCoord);
	gl_FragColor = texcolor;
}
