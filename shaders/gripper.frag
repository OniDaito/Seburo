#version 420 compatibility

in vec4 vVertexNormal;
in vec4 vVertexPosition;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform vec3 uLight0;
uniform vec3 uCamPos;

uniform float uShininess;

void main(){

	vec3 N = normalize(vVertexNormal.xyz);
	vec3 L =  normalize(uLight0 - vVertexPosition.xyz);
	float lambertTerm = dot(N,L);
	vec4 final_colour = vec4(0.0);
    if (lambertTerm > 0.0) {
      final_colour += vec4(0.5,0.45,0.58,1.0) * lambertTerm;	
      vec3 E = normalize( -vVertexPosition.xyz );
      vec3 R = reflect(-L, N);
      float specular = pow(max(dot(R, E), 0.0),  uShininess);
      final_colour += specular;	
    }
    gl_FragColor = final_colour;
}
