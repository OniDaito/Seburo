#version 330
precision highp float;

in vec4 a_vertex;
out vec2 texCoord;

void main(void) {
	gl_Position = a_vertex;
	texCoord = gl_MultiTexCoord0.st;
}
