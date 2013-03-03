#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 uResolution;
uniform float uTime;


// The formula for returning the distance from a point to a sphere

float sdSphere( vec3 p, float s ) {
  return length(p)-s;
}

// Given an origin and a normalized direction, shoot off a ray and raymarch

float castRay(vec3 rayOrigin, vec3 rayDirection ){

  float prevdist = 0.0;
  for( float t = 0.001; t < 10.0; t += 0.1) {
    vec3 p = rayOrigin + rayDirection * t;

    // Move the sphere 8 units in the z direction, away from the screen
    // Also, use a sine function based on time to move the sphere left and right
    vec3 translate = vec3( uTime, 0.0,8.0);
    
    float dist = sdSphere(p - translate, 1.0);
    
    if (dist < 0.0 && prevdist > 0.0){
      return 20.0 / (t * t);
    }
      
    prevdist = dist;
  }
  return 0.0;
}

void main( void ) {

  // Convert the frag coord into its -1 to 1 representation

  vec2 q = gl_FragCoord.xy / uResolution.xy;
  vec2 p = -1.0 + 2.0 * q;
  p.x *= uResolution.x/uResolution.y;


  // We assume the screen is at 0 in distance and our camera is behind it

  vec3 camera = vec3(0.0,0.0,-10.0);
  vec3 screen = vec3(p.x,p.y,0.0);

  // A quick optimisation to make sure the ray starts at the screen

  vec3 diff = screen - camera;
  vec3 dir = normalize(diff);
  float dist = length(diff);

  float val = castRay(camera + dir * dist, dir);
  
  gl_FragColor = vec4( 0,val,val, 1.0 );
}