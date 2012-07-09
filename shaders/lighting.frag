#version 420 compatibility

in vec4 vLightPos;
in vec4 vVertexNormal;
in vec4 vVertexPosition;
in vec2 vTexCoord;
flat in uint vTexID;

uniform sampler2DRect uBaseTex;
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
	vec3 n = normalize(vVertexNormal.xyz);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);
	
	// the material properties are embedded in the shader (for now)
	vec4 mat_ambient = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 mat_diffuse = vec4(0.0, 0.0, 1.0, 0.0);
	
	///\todo fix this because it sucks
	if (vTexID == 0){
		mat_diffuse = texture(mCamTex0,vTexCoord);
	}
	else if (vTexID == 1){
		mat_diffuse = texture(mCamTex1,vTexCoord);
	}
	else if (vTexID == 2){
		mat_diffuse = texture(mCamTex2,vTexCoord);
	}
	else if (vTexID == 3){
		mat_diffuse = texture(mCamTex3,vTexCoord);
	}
	else if (vTexID == 4){
		mat_diffuse = texture(mCamTex4,vTexCoord);
	}
	else if (vTexID == 5){
		mat_diffuse = texture(mCamTex5,vTexCoord);
	}
	else if (vTexID == 6){
		mat_diffuse = texture(mCamTex6,vTexCoord);
	}
	else if (vTexID == 7){
		mat_diffuse = texture(mCamTex7,vTexCoord);
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
	vec3 lightDir = normalize(vLightPos.xyz - vVertexPosition.xyz);
	float NdotL = dot(n, lightDir);
	
	if (NdotL > 0.0)
		diffuse = kd * NdotL;
	
	// specular term
	vec3 rVector = normalize(2.0 * n * dot(n, lightDir) - lightDir);
	vec3 viewVector = normalize(-vVertexPosition.xyz);
	float RdotV = dot(rVector, viewVector);
	
	if (RdotV > 0.0)
		specular = ks * pow(RdotV, uShininess);

	gl_FragColor = ambient + diffuse + specular;
}
