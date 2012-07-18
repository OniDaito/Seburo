#version 420 compatibility

out vec4 vLightPos;
out vec4 vVertexNormal;
out vec4 vVertexPosition;
out vec2 vTexCoord0;
out vec2 vTexCoord1;
out vec2 vTexCoord2;
out vec2 vTexCoord3;
out vec2 vTexCoord4;
out vec2 vTexCoord5;
out vec2 vTexCoord6;
out vec2 vTexCoord7;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;
uniform vec3 uLight0;

layout (location = 0) in vec3 attribVertPosition;
layout (location = 1) in vec3 attribNormal;
layout (location = 2) in vec2 attribTexCoord0;
layout (location = 3) in vec2 attribTexCoord1;
layout (location = 4) in vec2 attribTexCoord2;
layout (location = 5) in vec2 attribTexCoord3;
layout (location = 6) in vec2 attribTexCoord4;
layout (location = 7) in vec2 attribTexCoord5;
layout (location = 8) in vec2 attribTexCoord6;
layout (location = 9) in vec2 attribTexCoord7;


void main() {            
    vVertexNormal = vec4(-attribNormal,1.0); //normalize(uNormalMatrix * vec4(-attribNormal,1.0));
    vLightPos = normalize( vec4(uLight0,1.0));
    vVertexPosition = uMVPMatrix * vec4(attribVertPosition,1.0);
    gl_Position = vVertexPosition;
    
    vTexCoord0 = attribTexCoord0;
    vTexCoord1 = attribTexCoord1;
	vTexCoord2 = attribTexCoord2;
    vTexCoord3 = attribTexCoord3;
    vTexCoord4 = attribTexCoord4;
    vTexCoord5 = attribTexCoord5;
    vTexCoord6 = attribTexCoord6;
    vTexCoord7 = attribTexCoord7;

} 

