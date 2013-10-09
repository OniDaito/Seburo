/**
* @brief Camera Classes
* @file camera.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/

#include "s9/camera.hpp"

using namespace std;
using namespace s9; 

/*
 * Basic Camera - Simply extends a primitive with viewports
 */

Camera::Camera() {
	reset();
}

void Camera::reset() {
	up_ = glm::vec3(0,1,0);
	pos_ = glm::vec3(0,0,1.0);
	look_ = glm::vec3(0,0,-1.0);
	near_ = 1.0f;
	field_ = 55.0f;
	far_ = 100.0f;
	r_ = 1.0;
	compute();
}


void Camera::setRatio(float r) {
	r_ = r;
	compute();
}

void Camera::align(Node &p) { 
	///\todo align with a node
}
	
void Camera::compute() {
	view_matrix_ = glm::lookAt(pos_, pos_ + look_, up_);
	projection_matrix_ = glm::perspective(field_,r_,near_, far_);
}

/*
 * Orbital Camera as oppose to a truck camera
 */

OrbitCamera::OrbitCamera() : Camera() {
	reset();
}

void OrbitCamera::reset() {
	pos_ = glm::vec3(0,0,1.0);
	look_ = glm::vec3(0,0,0);
	up_ = glm::vec3(0,1,0);
	near_ = 1.0f;
	field_ = 55.0f;
	far_ = 100.0f;
	sense_ = 0.1f;
}

void OrbitCamera::compute() {
	view_matrix_ = glm::lookAt(pos_, look_, up_);
	projection_matrix_ = glm::perspective(field_, r_, near_, far_);
}

void OrbitCamera::zoom(float z) {
	glm::vec3 dir = pos_ - look_;
	dir = glm::normalize(dir);
	dir *= z;
	pos_ += dir;
	compute();
}

void OrbitCamera::shift(glm::vec2 s) {
	glm::vec3 dir = pos_ - look_;
	dir = glm::normalize(dir);
	glm::vec3 shiftx = glm::cross(dir,up_);
	shiftx *= s.x;
	glm::vec3 shifty = up_ * s.y;

	pos_ += shiftx + shifty;
	look_ += shiftx + shifty;
	
	compute();
}

void OrbitCamera::yaw(float a){
	glm::quat q_rotate;
	q_rotate = glm::rotate( q_rotate, a, up_ );
	up_ = q_rotate * up_;
	pos_ = q_rotate *pos_;
	compute();
}

void OrbitCamera::pitch(float a){
	glm::quat q_rotate;
	
	glm::vec3 right = glm::normalize(glm::cross(up_, glm::normalize(look_ - pos_)));
	
	q_rotate = glm::rotate( q_rotate, a, right );
	up_ = q_rotate * up_;
	pos_ = q_rotate * pos_;
	
	compute();
}

void OrbitCamera::roll(float a){
	glm::quat q_rotate;
	q_rotate = glm::rotate( q_rotate, a,  glm::normalize(look_ - pos_));
	up_ = q_rotate * up_;
	
	compute();
}

/*
 * Screen Camera - Simply maps ortho style to screen width and height in pixels
 */
 
void ScreenCamera::compute() {
	view_matrix_ = glm::mat4(1.0f);
	projection_matrix_ = glm::ortho(static_cast<float>(0.0), static_cast<float>(w_), 
			static_cast<float>(h_), static_cast<float>(0.0));
}



