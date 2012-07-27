#version 420 compatibility

in vec4 vVertexPosition;
in vec4 vColour;

void main() {
	gl_FragColor = vColour;
}