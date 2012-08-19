#version 330
precision highp float;

in vec4 vertex_position;

uniform vec4 uColour;

void main() {
	gl_FragColor = uColour;
}
