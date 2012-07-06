#version 420 compatibility

out vec4 vertex_light_position;
out vec4 vertex_normal;
out vec4 vertex_position;
out vec2 texCoord;
out flat uint texID;

uniform mat4 mMVPMatrix;
uniform mat4 mMVMatrix;
uniform mat4 mNormalMatrix;
uniform vec3 mLight0;


// no indices for the mesh drawing

layout (location = 0) in vec3 attribVertPosition; 
layout (location = 1) in vec3 attribNormal;
layout (location = 2) in vec2 attribTexCoord;
layout (location = 3) in uint attribTexID;


void main() {            
    vertex_normal = normalize(mNormalMatrix * vec4(-attribNormal,1.0));
    vertex_light_position = normalize( vec4(mLight0,1.0));
    vertex_position = mMVPMatrix * vec4(attribVertPosition,1.0);
    gl_Position = vertex_position;
    texCoord = attribTexCoord;
    texID = attribTexID;
} 

