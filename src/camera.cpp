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

void Camera::update() {	
 	view_matrix_ = glm::lookAt(pos_, look_, up_);
	projection_matrix_ = glm::perspective(field_, ratio_, near_, far_);
}

void Camera::reset() {
	up_ = glm::vec3(0,1,0);
	pos_ = glm::vec3(0,0,1.0);
	look_ = glm::vec3(0,0,0);
	near_ = 0.1f;
	field_ = 55.0f;
	far_ = 100.0f;
	ratio_ = 1.0;
}


void Camera::set_ratio(float r) {
	ratio_ = r;
}


void Camera::zoom(float z) {
	glm::vec3 dir = pos_ - look_;
	dir = glm::normalize(dir);
	dir *= z;
	pos_ += dir;

}

void Camera::shift(glm::vec2 s) {
	glm::vec3 dir = pos_ - look_;
	dir = glm::normalize(dir);
	glm::vec3 shiftx = glm::cross(dir,up_);
	shiftx *= s.x;
	glm::vec3 shifty = up_ * s.y;

	pos_ += shiftx + shifty;
	look_ += shiftx + shifty;
	
}

void Camera::yaw(float a){
	glm::quat q_rotate;
	q_rotate = glm::rotate( q_rotate, a, up_ );
	up_ = q_rotate * up_;
	pos_ = q_rotate *pos_;

}

void Camera::pitch(float a){
	glm::quat q_rotate;
	
	glm::vec3 right = glm::normalize(glm::cross(up_, glm::normalize(look_ - pos_)));
	
	q_rotate = glm::rotate( q_rotate, a, right );
	up_ = q_rotate * up_;
	pos_ = q_rotate * pos_;
	
}

void Camera::roll(float a){
	glm::quat q_rotate;
	q_rotate = glm::rotate( q_rotate, a,  glm::normalize(look_ - pos_));
	up_ = q_rotate * up_;	
}



