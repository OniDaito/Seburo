/*
---------------------------------------------------------------------------
| Q U A T E R N I O N F R A C T A L S D E M O |
---------------------------------------------------------------------------

Copyright (c) 2009 - 2010 Denis Bogolepov ( denisbogol @ gmail.com )

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <http://www.gnu.org/licenses/>.
*/

///////////////////////////////////////////////////////////////////////////////
// Specific data types

struct SCamera
{
vec3 Position;

vec3 Side;

vec3 Up;

vec3 View;

vec2 Scale;
};

//-----------------------------------------------------------------------------

struct SRay
{
vec3 Origin;

vec3 Direction;
};

//-----------------------------------------------------------------------------

struct SFractal
{
vec4 Center;

float Epsilon;

int Iterations;
};

///////////////////////////////////////////////////////////////////////////////
// Shader interface

/* Camera position and orientation */

uniform SCamera Camera;

//-----------------------------------------------------------------------------

/* Julia fractal parameters */

uniform SFractal Fractal;

//-----------------------------------------------------------------------------

/* Width and height of a viewport ( for anti-aliasing ) */

uniform int Width;

uniform int Height;

///////////////////////////////////////////////////////////////////////////////
// Shader constants

/* Minimum and maximum point of bounding box ( the scene is in this box ) */

const vec3 BoxMinimum = vec3 ( -5.0, -5.0, -5.0 );

const vec3 BoxMaximum = vec3 ( 5.0, 5.0, 5.0 );

//-----------------------------------------------------------------------------

/* Some useful vector constants */

const vec3 Zero = vec3 ( 0.0, 0.0, 0.0 );

const vec3 Unit = vec3 ( 1.0, 1.0, 1.0 );

const vec3 AxisX = vec3 ( 1.0, 0.0, 0.0 );

const vec3 AxisY = vec3 ( 0.0, 1.0, 0.0 );

const vec3 AxisZ = vec3 ( 0.0, 0.0, 1.0 );

///////////////////////////////////////////////////////////////////////////////
// Math Functions

vec4 QuaternionMult ( vec4 left, vec4 right )
{
vec4 r;

r.x = left.x * right.x - dot ( left.yzw, right.yzw );

r.yzw = left.x * right.yzw +
right.x * left.yzw +
cross ( left.yzw, right.yzw );

return r;
}

vec4 QuaternionSquare ( vec4 quaternion )
{
vec4 r;

r.x = quaternion.x * quaternion.x -
dot ( quaternion.yzw, quaternion.yzw );

r.yzw = 2.0 * quaternion.x * quaternion.yzw;

return r;
}

///////////////////////////////////////////////////////////////////////////////
// Functions for compute ray-object intersection

/*
* Intersects ray with sphere in coordinates origin.
*
* Function implements simple algorithm for ray-sphere
* intersection based on equating of the equations of
* ray and sphere. For details see:
*
* Ray Sphere Intersection ( CGS Wiki )
* http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection
*/

//bool IntersectSphere ( in SRay ray /* ray origin and direction */,
// in float radius /* sphere radius */,
// out float start /* time of 1st intersection */,
// out float final /* time of 2nd intersection */ )
bool IntersectSphere ( SRay ray, float sqrradius, out float start )
{
float A = dot ( ray.Direction, ray.Direction );

float B = dot ( ray.Direction, ray.Origin );

float C = dot ( ray.Origin, ray.Origin ) - sqrradius;

float D = B * B - A * C;

if ( D > 0.0 )
{
D = sqrt ( D );

start = max ( 0.0, ( -B - D ) / A );

return D > B;
}

return false;
}

//-----------------------------------------------------------------------------

/*
* GLSL version of function from project of Keenan Crane
* "Ray Tracing Quaternion Julia Sets on the GPU"
* http://www.devmaster.net/forums/showthread.php?t=4448
*
* Iterates the quaternion for the purposes of intersection.
* This function also produces an estimate of the derivative
* at specified quaternion, which is required for the distance
* estimate.
*
* To estimate membership in the set, we recursively evaluate
*
* q = q * q + c
*
* until q has a magnitude greater than the threshold value
* ( it probably diverges ) or we've reached the maximum number
* of allowable iterations ( it probably converges ). More
* iterations reveal greater detail in the set.
*
* To estimate the derivative at q, we recursively evaluate
*
* q' = 2 * q * q'
*
* concurrently with the evaluation of q.
*/

/* If magnitude exceed this threshold series are considered divergent */

#define ESCAPE_THRESHOLD 10.0

void Iterate ( inout vec4 quaternion, inout vec4 derivative )
{
for ( int i = 0; i < Fractal.Iterations; ++i )
{
derivative = 2.0 * QuaternionMult ( quaternion, derivative );

quaternion = QuaternionSquare ( quaternion ) + Fractal.Center;

if ( dot ( quaternion, quaternion ) > ESCAPE_THRESHOLD )
{
break;
}
}
}

//-----------------------------------------------------------------------------

/*
* GLSL version of function from project of Keenan Crane
* "Ray Tracing Quaternion Julia Sets on the GPU"
* http://www.devmaster.net/forums/showthread.php?t=4448
*
* Finds the intersection of a ray with the quaternion Julia set.
* The intersection is found using iterative sphere tracing, which
* takes a conservative step along the ray at each iteration by
* estimating the minimum distance between the current ray origin
* and the closest point in the Julia set.
*/

/* Square radius of a bounding sphere for the set */

#define SQUARE_RADIUS 3.0

float Intersect ( inout SRay ray )
{
float distance = 0.0;

while ( true )
{
vec4 quaternion = vec4 ( ray.Origin, 0.0 );

vec4 derivative = vec4 ( 1.0, 0.0, 0.0, 0.0 );

Iterate ( quaternion, derivative );

float norm = length ( quaternion );

distance = 0.5 * norm * log ( norm ) / length ( derivative );

ray.Origin += ray.Direction * distance;

if ( distance < Fractal.Epsilon ||
dot ( ray.Origin, ray.Origin ) > SQUARE_RADIUS )
{
break;
}
}

return distance;
}

//-----------------------------------------------------------------------------

/*
* GLSL version of function from project of Keenan Crane
* "Ray Tracing Quaternion Julia Sets on the GPU"
* http://www.devmaster.net/forums/showthread.php?t=4448
*
* Create a shading normal for the current point. We use an
* approximate normal of the isosurface of the potential function,
* though there are other ways to generate a normal.
*/

/* Delta is used in the finite difference approximation of the gradient */

#define DELTA 1e-4

/* Number of iterations for normal estimation */

#define NORMAL_ITERATIONS 10

vec3 NormEstimate ( in vec3 point )
{
vec4 quaternion = vec4 ( point, 0.0 );

vec4 xLeft = quaternion - vec4 ( DELTA, 0.0, 0.0, 0.0 );
vec4 xRight = quaternion + vec4 ( DELTA, 0.0, 0.0, 0.0 );

vec4 yLeft = quaternion - vec4 ( 0.0, DELTA, 0.0, 0.0 );
vec4 yRight = quaternion + vec4 ( 0.0, DELTA, 0.0, 0.0 );

vec4 zLeft = quaternion - vec4 ( 0.0, 0.0, DELTA, 0.0 );
vec4 zRight = quaternion + vec4 ( 0.0, 0.0, DELTA, 0.0 );

for ( int i = 0; i < NORMAL_ITERATIONS; ++i )
{
xLeft = QuaternionSquare ( xLeft ) + Fractal.Center;
xRight = QuaternionSquare ( xRight ) + Fractal.Center;

yLeft = QuaternionSquare ( yLeft ) + Fractal.Center;
yRight = QuaternionSquare ( yRight ) + Fractal.Center;

zLeft = QuaternionSquare ( zLeft ) + Fractal.Center;
zRight = QuaternionSquare ( zRight ) + Fractal.Center;
}

vec3 gradient = vec3 ( length ( xRight ) - length ( xLeft ),
length ( yRight ) - length ( yLeft ),
length ( zRight ) - length ( zLeft ) );

return normalize ( gradient );
}

///////////////////////////////////////////////////////////////////////////////
// Support functions

/*
* Generates primary ray for a current fragment.
*
* You can change camera state by setting function argument.
* It can be used for stereo rendering ( move camera left and
* right for each fragment and tracing two primary rays ) or
* anti-aliasing.
*/

SRay GenerateRay ( in SCamera camera /* current camera state */,
in vec2 position /* fragment position on the screen */ )
{
vec2 coords = position * camera.Scale;

vec3 direction = camera.View -
camera.Side * coords.x +
camera.Up * coords.y;

return SRay ( camera.Position, normalize ( direction ) );
}

//-----------------------------------------------------------------------------

/* Phong material of metaballs */

#define AMBIENT 0.1

#define DIFFUSE 0.8

#define SPECULAR 1.0

#define SHININESS 64.0

/* Computes lighting in an intersection point and its final color */

vec3 Phong ( in vec3 point /* intersection point with surface */,
in vec3 normal /* normal to the surface in this point */,
in vec3 color /* diffuse color in this point */ )
{
vec3 light = normalize ( Camera.Position - point );

vec3 view = normalize ( Camera.Position - point );

vec3 refl = reflect ( -view, normal );

//--------------------------------------------------------------------

float diffuse = max ( dot ( light, normal ), 0.0 );

float specular = pow ( max ( dot ( refl, light ), 0.0 ), SHININESS );

vec3 result = AMBIENT * Unit +
DIFFUSE * diffuse * color +
SPECULAR * specular * Unit;

return result;
}

///////////////////////////////////////////////////////////////////////////////
// Core ray tracing function

vec3 Raytrace ( in SRay ray )
{
vec3 result = Zero;

float time;

if ( IntersectSphere ( ray, SQUARE_RADIUS, time ) )
{
ray.Origin += ray.Direction * time;

time = Intersect ( ray );

if ( time < Fractal.Epsilon )
{
vec3 normal = NormEstimate ( ray.Origin );

result += Phong ( ray.Origin, normal, abs ( ray.Origin ) );
}
}

return result;
}

///////////////////////////////////////////////////////////////////////////////
// Shader entry point

#define SUPER_SAMPLING_ /* Use only on fast GPUs such as AMD Radeon 58xx */

void main ( void )
{
vec2 position = gl_TexCoord [0].xy;

SRay ray = GenerateRay ( Camera, position );

gl_FragColor = vec4 ( Raytrace ( ray ), 1.0 );

#ifdef SUPER_SAMPLING

float dx = 1.0 / Width; /* width of the fragment on image plane */

float dy = 1.0 / Height; /* height of the fragment on image plane */


position = vec2 ( gl_TexCoord [0].x, gl_TexCoord [0].y + dy );

ray = GenerateRay ( Camera, position );

gl_FragColor += vec4 ( Raytrace ( ray ), 1.0 );


position = vec2 ( gl_TexCoord [0].x, gl_TexCoord [0].y - dy );

ray = GenerateRay ( Camera, position );

gl_FragColor += vec4 ( Raytrace ( ray ), 1.0 );


position = vec2 ( gl_TexCoord [0].x + dx, gl_TexCoord [0].y );

ray = GenerateRay ( Camera, position );

gl_FragColor += vec4 ( Raytrace ( ray ), 1.0 );


position = vec2 ( gl_TexCoord [0].x - dx, gl_TexCoord [0].y );

ray = GenerateRay ( Camera, position );

gl_FragColor += vec4 ( Raytrace ( ray ), 1.0 );


gl_FragColor /= 5.0;

#endif
}