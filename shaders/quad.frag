#version 420 compatibility

in vec4 vVertexPosition;
in vec4 vColour;
in vec4 vTexCoord;

void main() {
	gl_FragColor = vColour;
}
