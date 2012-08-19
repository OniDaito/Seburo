#version 330
precision highp float;

out vec4 fragColor;

uniform sampler2DRect baseTex0;
uniform sampler2DRect newTex1;
in vec2 texCoord;

void main(void){
	
    float col = texture2DRect(baseTex0, texCoord).r;
    float col2 = texture2DRect(newTex1, texCoord).r;
    
    float d = min(col,col2);
    
	fragColor = vec4(d,d,d,1.0);
}
