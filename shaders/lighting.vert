#version 420 compatibility

out vec4 vLightPos;
out vec4 vVertexNormal;
out vec4 vVertexPosition;
out vec2 vTexCoord;
flat out uint vTexID;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;
uniform vec3 uLight0;


layout (location = 0) in vec3 attribVertPosition; // 1 is the indices
layout (location = 2) in vec3 attribNormal;
layout (location = 3) in vec2 attribTexCoord;
layout (location = 4) in uint attribTexID;


void main() {            
    vVertexNormal = normalize(uNormalMatrix * vec4(-attribNormal,1.0));
    vLightPos = normalize( vec4(uLight0,1.0));
    vVertexPosition = uMVPMatrix * vec4(attribVertPosition,1.0);
    gl_Position = vVertexPosition;
    vTexCoord = attribTexCoord;
    vTexID = attribTexID;
} 

