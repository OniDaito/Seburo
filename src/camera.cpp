/**
* @brief Camera Classes
* @file camera.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/

#include "camera.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign;
using namespace s9; 


/*
 * Basic Camera - This one keeps the up vector always parallel to Y axis
 * \todo add inertia
 */

Camera::Camera(){
	mUp = glm::vec3(0,1,0);
	mPos = glm::vec3(0,0,1.0);
	mLook = glm::vec3(0,0,-1.0);
	mNear = 1.0f;
	mField = 55.0f;
	mFar = 100.0f;
	compute();
}

 
void Camera::yaw(float_t a){
	glm::quat q_rotate;
	q_rotate = glm::rotate( q_rotate, a, mUp );
	mLook = q_rotate * mLook;
	mUp = q_rotate * mUp;
	
	compute();
}

void Camera::pitch(float_t a){
	glm::quat q_rotate;
	
	glm::vec3 right = glm::cross(mUp,mLook);
	
	q_rotate = glm::rotate( q_rotate, a, right );
	mLook = q_rotate * mLook;
	mUp = q_rotate * mUp;
	
	compute();
}

void Camera::roll(float_t a){
	glm::quat q_rotate;
	q_rotate = glm::rotate( q_rotate, a, mLook);
	mLook = q_rotate * mLook;
	mLook = glm::normalize(mLook);
	mUp = q_rotate * mUp;
	
	compute();
}

void Camera::setLook(glm::vec3 l) {
	mLook = glm::normalize(l);
	compute(); 
}
	
void Camera::move(glm::vec3 m){
	mPos += m;
	compute();
}

void Camera::setRatio(float_t r) {
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
	mPos = glm::vec3(0,0,1.0);
	mLook = glm::vec3(0,0,0);
	mUp = glm::vec3(0,1,0);
	mNear = 1.0f;
	mField = 55.0f;
	mFar = 100.0f;
}

void OrbitCamera::compute() {
	mViewMatrix = glm::lookAt(mPos, mLook, mUp);
	mProjectionMatrix = glm::perspective(mField, mR, mNear, mFar);
}

void OrbitCamera::zoom(float_t z) {
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

void OrbitCamera::yaw(float_t a){
	glm::quat q_rotate;
	q_rotate = glm::rotate( q_rotate, a, mUp );
	mUp = q_rotate * mUp;
	mPos = q_rotate *mPos;
	compute();
}

void OrbitCamera::pitch(float_t a){
	glm::quat q_rotate;
	
	glm::vec3 right = glm::normalize(glm::cross(mUp, glm::normalize(mLook - mPos)));
	
	q_rotate = glm::rotate( q_rotate, a, right );
	mUp = q_rotate * mUp;
	mPos = q_rotate * mPos;
	
	compute();
}

void OrbitCamera::roll(float_t a){
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
	mProjectionMatrix = glm::ortho(0.0f, static_cast<float_t>(mW), static_cast<float_t>(mH), 0.0f);
}



