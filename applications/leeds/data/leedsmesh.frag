#version 420 compatibility

in vec4 vLightPos;
in vec4 vVertexNormal;
in vec4 vVertexPosition;
in vec2 vTexCoord0;
in vec2 vTexCoord1;
in vec2 vTexCoord2;
in vec2 vTexCoord3;
in vec2 vTexCoord4;
in vec2 vTexCoord5;
in vec2 vTexCoord6;
in vec2 vTexCoord7;

uniform float uShininess;
uniform float uMaxX;
uniform float uMaxY;
uniform bool uShowPos; 

uniform vec3 uCam0;
uniform vec3 uCam1;
uniform vec3 uCam2;
uniform vec3 uCam3;
uniform vec3 uCam4;
uniform vec3 uCam5;
uniform vec3 uCam6;
uniform vec3 uCam7;

///\todo pass in face normals so we can work out the best texture to use based in the shader

layout(binding=0) uniform sampler2DRect mCamTex0;
layout(binding=1) uniform sampler2DRect mCamTex1;
layout(binding=2) uniform sampler2DRect mCamTex2;
layout(binding=3) uniform sampler2DRect mCamTex3;
layout(binding=4) uniform sampler2DRect mCamTex4;
layout(binding=5) uniform sampler2DRect mCamTex5;
layout(binding=6) uniform sampler2DRect mCamTex6;
layout(binding=7) uniform sampler2DRect mCamTex7;


// For the plane, test via range finding

///\todo pass in here the number of active cameras - useful to know

void main() {
	vec3 n = normalize(vVertexNormal.xyz);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);
	
	// the material properties are embedded in the shader (for now)
	vec4 mat_ambient = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 mat_diffuse = vec4(0.0, 0.0, 1.0, 0.0);
	
	float angle = 1.0 - abs(dot(uCam0,n));
	float max  = angle;
	if (!uShowPos)
		mat_diffuse = texture(mCamTex0,vTexCoord0);
	else
		mat_diffuse = vec4(0.25,0.0,0.0,1.0);


	angle = 1.0 - abs(dot(uCam1,n));
	if (angle < max && vTexCoord1.x < uMaxX && vTexCoord1.y < uMaxY && vTexCoord1.x > 0 && vTexCoord1.y > 0){
		max = angle;
		if (!uShowPos)
			mat_diffuse = texture(mCamTex1,vTexCoord1);
		else
			mat_diffuse = vec4(0.75,0.0,0.0,1.0);
	}
	
	angle = 1.0 - abs(dot(uCam2,n));
	if (angle < max && vTexCoord2.x < uMaxX && vTexCoord2.y < uMaxY && vTexCoord2.x > 0 && vTexCoord2.y > 0){
		max = angle;
		if (!uShowPos)
			mat_diffuse = texture(mCamTex2,vTexCoord2);
		else
			mat_diffuse = vec4(0.25,0.25,0.0,1.0);
	}
		
	angle = 1.0 - abs(dot(uCam3,n));
	if (angle < max && vTexCoord3.x < uMaxX && vTexCoord3.y < uMaxY && vTexCoord3.x > 0 && vTexCoord3.y > 0){
		max = angle;
		if (!uShowPos)
			mat_diffuse = texture(mCamTex3,vTexCoord3);
		else
			mat_diffuse = vec4(0.25,0.25,0.25,1.0);
	}
	
	angle = 1.0 - abs(dot(uCam4,n));
	if (angle < max && vTexCoord4.x < uMaxX && vTexCoord4.y < uMaxY && vTexCoord4.x > 0 && vTexCoord4.y > 0){
		max = angle;
		if (!uShowPos)
			mat_diffuse = texture(mCamTex4,vTexCoord4);
		else
			mat_diffuse = vec4(0.5,0.75,0.0,1.0);
	}
	
	angle = 1.0 - abs(dot(uCam5,n));
	if (angle < max && vTexCoord5.x < uMaxX && vTexCoord5.y < uMaxY && vTexCoord5.x > 0 && vTexCoord5.y > 0){
		max = angle;
		if (!uShowPos)
			mat_diffuse = texture(mCamTex5,vTexCoord5);
		else
			mat_diffuse = vec4(0.0,0.0,0.75,1.0);
	}
	
	angle = 1.0 - abs(dot(uCam6,n));
	if (angle < max && vTexCoord6.x < uMaxX && vTexCoord6.y < uMaxY && vTexCoord6.x > 0 && vTexCoord6.y > 0){
		max = angle;
		if (!uShowPos)
			mat_diffuse = texture(mCamTex6,vTexCoord6);
		else
			mat_diffuse = vec4(0.0,0.75,0.0,1.0);
	}
	
	angle = 1.0 -abs(dot(uCam7,n));
	if (angle < max && vTexCoord7.x < uMaxX && vTexCoord7.y < uMaxY && vTexCoord7.x > 0 && vTexCoord7.y > 0){
		max = angle;
		if (!uShowPos)
			mat_diffuse = texture(mCamTex7,vTexCoord7);
		else
			mat_diffuse = vec4(0.75,0.75,0.75,1.0);
	}
	
	
	if (length(mat_diffuse) == 0)
		mat_diffuse = vec4(1.0, 0.0, 1.0, 1.0);
	
	
/*	vec4 mat_specular = vec4(1.0, 1.0, 1.0, 1.0);
	
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

	gl_FragColor = ambient + diffuse + specular;*/
	
	
	gl_FragColor = mat_diffuse;
}
