#version 150

// https://github.com/dghost/glslRiftDistort

layout(points) in;
layout(triangle_strip, max_vertices = 8) out;

out vec2 sTexCoord;
invariant out vec2 sScreenCenter;
invariant out vec2 sLensCenter;

uniform float uDistortionOffset;
uniform float uDistortionScale = 0.8f;

void emitQuad(vec4 screen, vec4 coords) {
/*
  screen is a rect describing the screen space coordinates
    of the rectangle to be emitted. screen.xy is the bottom left
    corner, and screen.zw is the upper right corner.
    
  coords is a rect describing the texture coordinates to be emitted
    with coords.xy describing the bottom left corner and coords.zw
    describing the upper right corner
    
*/
  gl_Position = vec4(screen.z, screen.w, 0.0, 1.0 );
  sTexCoord = vec2( coords.z, coords.w);
  EmitVertex();

  gl_Position = vec4(screen.x, screen.w, 0.0, 1.0 );
  sTexCoord = vec2( coords.x, coords.w );
  EmitVertex();

  gl_Position = vec4(screen.z,screen.y, 0.0, 1.0 );
  sTexCoord = vec2( coords.z, coords.y );
  EmitVertex();

  gl_Position = vec4(screen.x,screen.y, 0.0, 1.0 );
  sTexCoord = vec2( coords.x, coords.y );
  EmitVertex();
  
  EndPrimitive();
}

void main() {
  sScreenCenter = vec2(0.25,0.5);
  sLensCenter = vec2(0.25 + uDistortionOffset * 0.25, 0.5);
  
  emitQuad(vec4(-1.0,-1.0,0.0,1.0),vec4(0.0,1.0,0.5,0.0));

  sScreenCenter = vec2(0.75,0.5);
  sLensCenter = vec2(0.75 - uDistortionOffset * 0.25, 0.5);
  
  emitQuad(vec4(0.0,-1.0,1.0,1.0),vec4(0.5,1.0,1.0,0.0));

}
