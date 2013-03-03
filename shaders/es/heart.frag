#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 uResolution;
uniform float uTime;


// The formula for a heart

float heartFormula(vec3 pos){
  float EPISLON = 0.000001;

  float a = (pow(pos.x,2.0) + (9.0/4.0 * pow(pos.z,2.0)) + pow(pos.y,2.0) - 1.0);
  a = pow(a,3.0);
  float b = pow(pos.x,2.0) * pow(pos.y,3.0);
  float c = 9.0/80.0 * pow(pos.z,2.0) * pow(pos.y,3.0);
  float d = a - b - c;

  return d - EPISLON;
}

// Given an origin and a normalized direction, shoot off a ray and raymarch

float castRay(vec3 rayOrigin, vec3 rayDirection ){

  float prevdist = 0.0;
  for( float t = 0.001; t < 4.0; t += 0.07) {

    vec3 p = rayOrigin + rayDirection * t;
    
    vec3 translate = vec3(sin(uTime), 0.0, cos(uTime) / 2.5 );

    float dist = heartFormula(p - translate);
    
    if (dist < 0.0 && prevdist > 0.0){
      return t;
    }
      
    prevdist = dist;
  }
  return 100.0;
}


void main( void ) {

  // Convert the frag coord into its -1 to 1 representation

  vec2 q = gl_FragCoord.xy / uResolution.xy;
  vec2 p = -1.0 + 2.0 * q;
  p.x *= uResolution.x/uResolution.y;


  // We assume the screen is at 0 in distance and our camera is behind it
  float xp = 0.0;
  float zp = -1.5;

  vec3 camera = vec3(xp,0.0,zp);
  vec3 screen = vec3(p.x-xp,p.y,-1.1);

  // A quick optimisation to make sure the ray starts at the screen

  vec3 diff = screen - camera;
  vec3 dir = normalize(diff);
  float dist = length(diff);

  float val = castRay(camera + dir * dist, dir);

  val = clamp(1.0 - val,0.0,1.0);

  
  gl_FragColor = vec4( 1.0,0.1,0.1, val );
}