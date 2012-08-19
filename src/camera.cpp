/**
* @brief Camera Classes
* @file camera.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/

#include "s9/camera.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign;
using namespace s9; 


/*
 * Basic Camera - Simply extends a primitive with viewports
 */

Camera::Camera() {
	reset();
}

void Camera::reset() {
	mUp = glm::vec3(0,1,0);
	mPos = glm::vec3(0,0,1.0);
	mLook = glm::vec3(0,0,-1.0);
	mNear = 1.0f;
	mField = 55.0f;
	mFar = 100.0f;
	mR = 1.0;
	compute();
}

 

void Camera::setRatio(float r) {
	mR = r;
	compute();
}

void Camera::align(Primitive &p) { 
	mLook = p.getLook();
	mUp = p.getUp();
	mPos = p.getPos();
	compute();	
}
	
void Camera::compute() {
	mViewMatrix = glm::lookAt(mPos, mPos + mLook, mUp);
	mProjectionMatrix = glm::perspective(mField,mR,mNear, mFar);
}

/*
 * Orbital Camera as oppose to a truck camera
 */

OrbitCamera::OrbitCamera() : Camera() {
	reset();
}

void OrbitCamera::reset() {
	mPos = glm::vec3(0,0,1.0);
	mLook = glm::vec3(0,0,0);
	mUp = glm::vec3(0,1,0);
	mNear = 1.0f;
	mField = 55.0f;
	mFar = 100.0f;
	_sense = 0.1f;
}

void OrbitCamera::compute() {
	mViewMatrix = glm::lookAt(mPos, mLook, mUp);
	mProjectionMatrix = glm::perspective(mField, mR, mNear, mFar);
}

void OrbitCamera::zoom(float z) {
	glm::vec3 dir = mPos - mLook;
	dir = glm::normalize(dir);
	dir *= z;
	mPos += dir;
	compute();
}

void OrbitCamera::shift(glm::vec2 s) {
	glm::vec3 dir = mPos - mLook;
	dir = glm::normalize(dir);
	glm::vec3 shiftx = glm::cross(dir,mUp);
	shiftx *= s.x;
	glm::vec3 shifty = mUp * s.y;

	mPos += shiftx + shifty;
	mLook += shiftx + shifty;
	
	compute();
}

void OrbitCamera::yaw(float a){
	glm::quat q_rotate;
	q_rotate = glm::rotate( q_rotate, a, mUp );
	mUp = q_rotate * mUp;
	mPos = q_rotate *mPos;
	compute();
}

void OrbitCamera::pitch(float a){
	glm::quat q_rotate;
	
	glm::vec3 right = glm::normalize(glm::cross(mUp, glm::normalize(mLook - mPos)));
	
	q_rotate = glm::rotate( q_rotate, a, right );
	mUp = q_rotate * mUp;
	mPos = q_rotate * mPos;
	
	compute();
}

void OrbitCamera::roll(float a){
	glm::quat q_rotate;
	q_rotate = glm::rotate( q_rotate, a,  glm::normalize(mLook - mPos));
	mUp = q_rotate * mUp;
	
	compute();
}

/*
 * Screen Camera - Simply maps ortho style to screen width and height in pixels
 */
 
void ScreenCamera::compute() {
	mViewMatrix = glm::mat4(1.0f);
	mProjectionMatrix = glm::ortho(static_cast<float>(0.0), static_cast<float>(mW), 
			static_cast<float>(mH), static_cast<float>(0.0));
}



