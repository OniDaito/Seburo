#version 420 compatibility

in vec4 vertex_position;

uniform vec4 mColour;

void main() {
	gl_FragColor = mColour;
}
