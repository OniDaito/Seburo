#version 330
precision highp float;
out vec4 fragColor;

<<<<<<< HEAD
uniform sampler2D camTexA;
uniform sampler2D camTexB;

=======
uniform sampler2DRect camTex0;
uniform sampler2DRect camTex1;
>>>>>>> 66c096aae473ec318a88ac57c1088ec3fff0067f
uniform int rangeX;
uniform int rangeY;
uniform int stepSize;
uniform float offsetL;
<<<<<<< HEAD
float lod;
uniform float topLod;
=======
>>>>>>> 66c096aae473ec318a88ac57c1088ec3fff0067f

uniform int width;
uniform int height;

in vec2 texCoord;

vec2 poissonDisk[64];

void createPoisson() {
	poissonDisk[0] = vec2(0, 0);
	poissonDisk[1] = vec2(0.170019, -0.040254);
	poissonDisk[2] = vec2(-0.299417, 0.791925);
	poissonDisk[3] = vec2(0.645680, 0.493210);
	poissonDisk[4] = vec2(-0.651784, 0.717887);
	poissonDisk[5] = vec2(0.421003, 0.027070);
	poissonDisk[6] = vec2(-0.817194, -0.271096);
	poissonDisk[7] = vec2(-0.705374, -0.668203);
	poissonDisk[8] = vec2(0.977050, -0.108615);
	poissonDisk[9] = vec2(0.063326, 0.142369);
	poissonDisk[10] = vec2(0.203528, 0.214331);
	poissonDisk[11] = vec2(-0.667531, 0.326090);
	poissonDisk[12] = vec2(-0.098422, -0.295755);
	poissonDisk[13] = vec2(-0.885922, 0.215369);
	poissonDisk[14] = vec2(0.566637, 0.605213);
	poissonDisk[15] = vec2(0.039766, -0.396100);
	poissonDisk[16] = vec2(0.751946, 0.453352);
	poissonDisk[17] = vec2(0.078707, -0.715323);
	poissonDisk[18] = vec2(-0.075838, -0.529344);
	poissonDisk[19] = vec2(0.724479, -0.580798);
	poissonDisk[20] = vec2(0.222999, -0.215125);
	poissonDisk[21] = vec2(-0.467574, -0.405438);
	poissonDisk[22] = vec2(-0.248268, -0.814753);
	poissonDisk[23] = vec2(0.354411, -0.887570);
	poissonDisk[24] = vec2(0.175817, 0.382366);
	poissonDisk[25] = vec2(0.487472, -0.063082);
	poissonDisk[26] = vec2(-0.084078, 0.898312);
	poissonDisk[27] = vec2(0.488876, -0.783441);
	poissonDisk[28] = vec2(0.470016, 0.217933);
	poissonDisk[29] = vec2(-0.696890, -0.549791);
	poissonDisk[30] = vec2(-0.149693, 0.605762);
	poissonDisk[31] = vec2(0.034211, 0.979980);
	poissonDisk[32] = vec2(0.503098, -0.308878);
	poissonDisk[33] = vec2(-0.016205, -0.872921);
	poissonDisk[34] = vec2(0.385784, -0.393902);
	poissonDisk[35] = vec2(-0.146886, -0.859249);
	poissonDisk[36] = vec2(0.643361, 0.164098);
	poissonDisk[37] = vec2(0.634388, -0.049471);
	poissonDisk[38] = vec2(-0.688894, 0.007843);
	poissonDisk[39] = vec2(0.464034, -0.188818);
	poissonDisk[40] = vec2(-0.440840, 0.137486);
	poissonDisk[41] = vec2(0.364483, 0.511704);
	poissonDisk[42] = vec2(0.034028, 0.325968);
	poissonDisk[43] = vec2(0.099094, -0.308023);
	poissonDisk[44] = vec2(0.693960, -0.366253);
	poissonDisk[45] = vec2(0.678884, -0.204688);
	poissonDisk[46] = vec2(0.001801, 0.780328);
	poissonDisk[47] = vec2(0.145177, -0.898984);
	poissonDisk[48] = vec2(0.062655, -0.611866);
	poissonDisk[49] = vec2(0.315226, -0.604297);
	poissonDisk[50] = vec2(-0.780145, 0.486251);
	poissonDisk[51] = vec2(-0.371868, 0.882138);
	poissonDisk[52] = vec2(0.200476, 0.494430);
	poissonDisk[53] = vec2(-0.494552, -0.711051);
	poissonDisk[54] = vec2(0.612476, 0.705252);
	poissonDisk[55] = vec2(-0.578845, -0.768792);
	poissonDisk[56] = vec2(-0.772454, -0.090976);
	poissonDisk[57] = vec2(0.504440, 0.372295);
	poissonDisk[58] = vec2(0.155736, 0.065157);
	poissonDisk[59] = vec2(0.391522, 0.849605);
	poissonDisk[60] = vec2(-0.620106, -0.328104);
	poissonDisk[61] = vec2(0.789239, -0.419965);
	poissonDisk[62] = vec2(-0.545396, 0.538133);
	poissonDisk[63] = vec2(-0.178564, -0.596057);
}

vec4 blockMatchPoisson(float texw, float texh) {
	vec2 ss = textureSize(camTexB,int(lod));
	vec4 col2 = textureLod(camTexB, texCoord, lod);
	int i;
	int j;
	float running  = 255.0;
	vec4 current = vec4(0.0,0.0,0.0,0.0);
	
	for (i =0; i < 64; i++){
		
		vec2 offset =  texCoord + poissonDisk[i];
		
		vec4 col = textureLod(camTexA, texCoord + poissonDisk[i], lod);
		float diff = distance(col2,col);
		if (diff < running){
			running  = diff;
			
			// Move towards 0-1 range with < 0.5 being negative
			current = vec4( (1.0 + poissonDisk[i].x ) * 0.5, (1.0 + poissonDisk[i].y ) * 0.5, 0.0, 1.0);
		}
		
	}
	return current;
}

vec4 blockMatch(float texw, float texh) {
	vec4 col2 = textureLod(camTexB, texCoord, lod);
	float i;
	float j;
	float running = 255.0;
	vec4 current = vec4(0,0,0,0);
	
	for (j = -rangeY; j < rangeY; j = j + stepSize) {
	
		for (i = -rangeX; i < rangeX; i = i + stepSize){
			vec2 tc = vec2(i * texw, j * texh );
			
			vec2 offset =  texCoord + tc;
			
			vec4 col = textureLod(camTexA, offset, lod);

			float diff = distance(col2,col);
			if (diff < running){
				running  = diff;
				
				current = vec4( (rangeX + i) / (rangeX * 2.0), (rangeY + j) / (rangeY * 2.0), 0, 1);
			}
			
		}	
	}
	return current;
}


vec4 refineBlock() {	
	
	float texw = 1.0 / width;
	float texh = 1.0 / height;
	
	vec4 match = blockMatch(texw, texh);
	
	while (lod > 0.0 && match != vec4(0,0,0,0)){
		match = blockMatch(texw,texh);
		lod = lod -1.0;
	}
	
	return match;
}




// The idea is to use the mipmaps in order to generate a hierarchical approach to sampling the area
// If the value is 0, dont bother to refine. Start with a high LOD and work down.

void main(void){
	lod = topLod;
	
<<<<<<< HEAD
	createPoisson();
  
	fragColor = refineBlock();
=======
  //  vec4 col = texture2DRect(camTex0, texCoord);
    vec4 col2 = texture2DRect(camTex1, texCoord);
    
    int i;
    int j;
    float running  = 255.0;
    vec4 current = vec4(0,0,0,1.0);
    
	for (i= -rangeY; i<= rangeY; i = i+stepSize) {
		
		for (j= -rangeX; j<= rangeX; j = j+stepSize) {
		
			vec2 tc = vec2(j,i);
			vec2 offset =  texCoord + tc;
			if (offset.x >-1 && offset.x < width){
				if (offset.y >-1 && offset.y < height){
			
					vec4 col = texture2DRect(camTex0, texCoord + tc);
			
					float diff = distance(col2,col);
					if (diff < running){
				
						running  = diff;
						current = vec4(tc.x * offsetL ,tc.y * offsetL,0.0,1.0);
					}
			
				}
			}
		}
	}
 
	fragColor = current;
>>>>>>> 66c096aae473ec318a88ac57c1088ec3fff0067f
}
