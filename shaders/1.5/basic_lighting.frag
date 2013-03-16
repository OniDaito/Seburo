vec4 vLightPos;
vec4 vVertexNormal;
vec4 vVertexPosition;

// Works with one light using the phong model

uniform float uShininess;


void main() {
	vec3 n = normalize(vVertexNormal.xyz);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);
	
	// the material properties are embedded in the shader (for now)
	vec4 mat_ambient = vec4(0.5, 0.5, 0.5, 1.0);
	vec4 mat_diffuse = vec4(1.0, 0.9, 1.0, 1.0);

	vec4 mat_specular = vec4(1.0, 1.0, 1.0, 1.0);
	
	// the light properties are embedded in the shader (for now)
	vec4 light_ambient = vec4(0.1, 0.1, 0.1, 1.0);
	vec4 light_diffuse = vec4(0.9, 0.9, 0.9, 1.0);
	vec4 light_specular = vec4(1.0, 1.0, 1.0, 1.0);
	
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
