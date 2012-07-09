#version 420 compatibility

in vec4 vLightPos;
in vec4 vVertexNormal;
in vec4 vVertexPosition;
in vec4 vTangentDir;


uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;
uniform mat4 uMInverseMatrix;
uniform vec3 uLight0;
uniform vec3 uTangent;
uniform vec3 uCamPos;

void main(){

	vec3 normalDirection = normalize(vVertexNormal.xyz);
	vec3 vTangentDir = normalize(uTangent.xyz);

	vec3 ambient = vec3(0.1,0.1,0.1);
	vec3 colour = vec3(0.9,0.9,0.92);
	vec3 lightcolour = vec3(0.9,0.9,0.92);
	vec3 speccolour = vec3(0.8,0.8,1.0);
	float alphax = 0.99;
	float alphay = 0.99;

	vec3 viewDirection =  normalize(uCamPos - vec3(vVertexPosition.xyz));
	vec3 lightDirection;
	float attenuation;

	// Assuming point light
	vec3 vertexToLightSource = vec3(uLight0 - vVertexPosition.xyz);
	float distance = length(vertexToLightSource);
	attenuation = 1.0 / distance; // linear attenuation 
	lightDirection = normalize(vertexToLightSource);


	vec3 halfwayVector = normalize(lightDirection + viewDirection);
	vec3 binormalDirection = cross(normalDirection, vTangentDir);
	float dotLN = dot(lightDirection, normalDirection); 

	// compute this dot product only once

	vec3 ambientLighting = vec3(ambient) * vec3(colour);
	vec3 diffuseReflection = attenuation * vec3(lightcolour) * vec3(colour) * max(0.0, dotLN);

	vec3 specularReflection;
	
	if (dotLN < 0.0) {
		specularReflection = vec3(0.0, 0.0, 0.0); 
	} else {
		float dotHN = dot(halfwayVector, normalDirection);
		float dotVN = dot(viewDirection, normalDirection);
		float dotHTAlphaX = dot(halfwayVector, vTangentDir) / alphax;
		float dotHBAlphaY = dot(halfwayVector, binormalDirection) / alphay;

		specularReflection = attenuation * vec3(speccolour) 
			* sqrt(max(0.0, dotLN / dotVN)) 
			* exp(-2.0 * (dotHTAlphaX * dotHTAlphaX 
			+ dotHBAlphaY * dotHBAlphaY) / (1.0 + dotHN));
	}

	gl_FragColor = vec4(ambientLighting + diffuseReflection + specularReflection, 1.0);
}
