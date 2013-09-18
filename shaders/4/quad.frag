#version 420

in vec4 vVertexPosition;
in vec4 vColour;

out vec4 fragColor;

void main() {
	fragColor = vColour;
}
