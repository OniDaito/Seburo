#version 420 compatibility

in vec4 vVertexPosition;
in vec2 vTexCoord;

uniform sampler2DRect uBaseTex;

void main() {
	gl_FragColor = texture(uBaseTex,vTexCoord);
}
