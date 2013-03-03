#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 uResolution;


// The formula for returning the distance from a point to a sphere

float sdSphere( vec3 p, float s ) {
  return length(p)-s;
}

// Given an origin and a normalized direction, shoot off a ray and raymarch

bool castRay(vec3 rayOrigin, vec3 rayDirection ){

  for( float t = 0.001; t < 10.0; t += 0.1) {
    vec3 p = rayOrigin + rayDirection*t;

    // Move the sphere 1 units in the z direction, away from the screen
    vec3 transform = vec3(0.0,0.0,1.0);

    if (sdSphere(p - transform,1.0) < 0.0)
    
      return true;
  }
  return false;
}

void main( void ) {

  // Convert the frag coord into its -1 to 1 representation

  vec2 q = gl_FragCoord.xy / uResolution.xy;
  vec2 p = -1.0 + 2.0 * q;
  p.x *= uResolution.x/uResolution.y;


  // We assume the screen is at 0 in distance and our camera is behind it

  vec3 camera = vec3(0.0,0.0,-0.1);

  vec3 screen = vec3(p.x,p.y,0.0);

  vec3 dir = normalize(screen - camera);

  bool hit = castRay(camera, dir);
  
  gl_FragColor = vec4( 1.0,1.0,1.0, 1.0 );
  if (hit)
    gl_FragColor = vec4( 1.0,0.0,0.0, 1.0 );
}