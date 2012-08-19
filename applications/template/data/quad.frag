#version 330

in vec4 vVertexPosition;
in vec4 vColour;
out vec4 fragColor

void main() {
  gl_FragColor = vColour;
}
