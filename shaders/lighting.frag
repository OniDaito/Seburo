#version 420 compatibility

in vec4 vertex_light_position;
in vec4 vertex_normal;
in vec4 vertex_position;
in vec2 texCoord;
flat in uint texID;

uniform sampler2DRect mBaseTex;
uniform float uShininess;

///\todo pass in face normals so we can work out the best texture to use based in the shader

layout(binding=0) uniform sampler2DRect mCamTex0;
layout(binding=1) uniform sampler2DRect mCamTex1;
layout(binding=2) uniform sampler2DRect mCamTex2;
layout(binding=3) uniform sampler2DRect mCamTex3;
layout(binding=4) uniform sampler2DRect mCamTex4;
layout(binding=5) uniform sampler2DRect mCamTex5;
layout(binding=6) uniform sampler2DRect mCamTex6;
layout(binding=7) uniform sampler2DRect mCamTex7;

///\todo pass in here the number of active cameras - useful to know

void main() {
	vec3 n = normalize(vertex_normal.xyz);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);
	
	// the material properties are embedded in the shader (for now)
	vec4 mat_ambient = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 mat_diffuse = vec4(0.0, 0.0, 1.0, 0.0);
	
	///\todo fix this because it sucks
	if (texID == 0){
		mat_diffuse = texture(mCamTex0,texCoord);
	}
	else if (texID == 1){
		mat_diffuse = texture(mCamTex1,texCoord);
	}
	else if (texID == 2){
		mat_diffuse = texture(mCamTex2,texCoord);
	}
	else if (texID == 3){
		mat_diffuse = texture(mCamTex3,texCoord);
	}
	else if (texID == 4){
		mat_diffuse = texture(mCamTex4,texCoord);
	}
	else if (texID == 5){
		mat_diffuse = texture(mCamTex5,texCoord);
	}
	else if (texID == 6){
		mat_diffuse = texture(mCamTex6,texCoord);
	}
	else if (texID == 7){
		mat_diffuse = texture(mCamTex7,texCoord);
	}
	
	if (length(mat_diffuse) == 0)
		mat_diffuse = vec4(1.0, 0.0, 1.0, 1.0);
	
	
	vec4 mat_specular = vec4(1.0, 1.0, 1.0, 1.0);
	
	// the light properties are embedded in the shader (for now)
	vec4 light_ambient = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 light_diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 light_specular = vec4(0.0, 0.0, 0.0, 1.0);
	
	// ambient term
	vec4 ambient = mat_ambient * light_ambient;
	
	// diffuse color
	vec4 kd = mat_diffuse * light_diffuse;
	
	// specular color
	vec4 ks = mat_specular * light_specular;
	
	// diffuse term
	vec3 lightDir = normalize(vertex_light_position.xyz - vertex_position.xyz);
	float NdotL = dot(n, lightDir);
	
	if (NdotL > 0.0)
		diffuse = kd * NdotL;
	
	// specular term
	vec3 rVector = normalize(2.0 * n * dot(n, lightDir) - lightDir);
	vec3 viewVector = normalize(-vertex_position.xyz);
	float RdotV = dot(rVector, viewVector);
	
	if (RdotV > 0.0)
		specular = ks * pow(RdotV, uShininess);

	gl_FragColor = ambient + diffuse + specular;
}
