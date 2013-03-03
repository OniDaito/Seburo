#ifdef GL_ES
  precision highp float;
#endif

uniform vec4 uColour;

void main(void) {
    gl_FragColor = uColour;
}