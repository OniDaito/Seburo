#version 330
precision highp float;

in vec4 vVertexPosition;
in vec4 vColour;

void main() {
	gl_FragColor = vColour;
}
