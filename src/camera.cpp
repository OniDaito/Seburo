/**
* @brief Camera Classes
* @file camera.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/

#include "s9/camera.hpp"

#include "GL/glew.h"

using namespace std;
using namespace s9; 

/*
 * Basic Camera - Simply extends a primitive with viewports
 */

Camera::Camera(glm::vec3 pos) : obj_ (shared_ptr<SharedObject>(new SharedObject())){
	obj_->pos = pos;
}

Camera::Camera(glm::vec3 pos, glm::vec3 look) : obj_ (shared_ptr<SharedObject>(new SharedObject())){
	obj_->pos = pos;
	obj_->look = look;
}


Camera::Camera(glm::vec3 pos, glm::vec3 look, glm::vec3 up) : obj_ (shared_ptr<SharedObject>(new SharedObject())){
	obj_->pos = pos;
	obj_->look = look;
	obj_->up = up;
}


/// Create a new shared object and set the defaults
void Camera::Init() {
	if (obj_ == nullptr){
		obj_ = shared_ptr<SharedObject>(new SharedObject());
		Defaults();
	}
}


/// Resize the view port, given width, height, left and bottom
	
void Camera::Resize(size_t w, size_t h, size_t l, size_t b){ 	
	set_ratio( static_cast<float>(w) / h);

	// Viewport values
	obj_->left = l;
	obj_->bottom = b;
	obj_->width = w;
	obj_->height = h;

}


/// Called by Node when a node is called with an attached camera

void Camera::Update() {	
 	if (obj_ == nullptr) { assert(false); } ///\todo this sort of thing should be caught at compile time if poss?
 	///\todo the above could potentially be moved into a general shared object pointer class?
 	obj_->view_matrix = glm::lookAt(obj_->pos, obj_->look, obj_->up);
 	if (obj_->orthographic)
 		// 0 -> width, 0 -> height for ortho matrix
 		obj_->projection_matrix = glm::ortho(0.0f, static_cast<float>(obj_->width),
 			0.0f, static_cast<float>(obj_->height), obj_->near, obj_->far);
 	else
		obj_->projection_matrix = glm::perspective(obj_->field, obj_->ratio, obj_->near, obj_->far);

}

/// Set the default camera values that are generally useful

void Camera::Defaults() {
	obj_->up = glm::vec3(0,1,0);
	obj_->pos = glm::vec3(0,0,1.0);
	obj_->look = glm::vec3(0,0,0);
	obj_->near = 0.1f;
	obj_->far = 100.0f;
	obj_->ratio = 1.0;
	obj_->left = 0;
	obj_->height = 100;
	obj_->width = 100;
	obj_->bottom = 0;
	obj_->field = 55.0f;

}


void Camera::set_ratio(float r) {
	obj_->ratio = r;
}


/// Zoom the camera in/out with a given value

void Camera::Zoom(float z) {
	glm::vec3 dir = obj_->pos - obj_->look;
	dir = glm::normalize(dir);
	dir *= z;
	obj_->pos += dir;

}


/// Move the camera along a plane - i.e move the eye and look point at the same time

void Camera::Shift(glm::vec2 s) {
	glm::vec3 dir = obj_->pos - obj_->look;
	dir = glm::normalize(dir);
	glm::vec3 shiftx = glm::cross(dir,obj_->up);
	shiftx *= s.x;
	glm::vec3 shifty = obj_->up * s.y;

	obj_->pos += shiftx + shifty;
	obj_->look += shiftx + shifty;
}

/// Yaw by an angle in degrees

void Camera::Yaw(float a){
	glm::quat q_rotate;
	q_rotate = glm::rotate( q_rotate, a, obj_->up );
	obj_->up = q_rotate * obj_->up;
	obj_->pos = q_rotate * obj_->pos;
}

/// Pitch the camera by an angle in degrees

void Camera::Pitch(float a){
	glm::quat q_rotate;
	
	glm::vec3 right = glm::normalize(glm::cross(obj_->up, glm::normalize(obj_->look - obj_->pos)));
	
	q_rotate = glm::rotate( q_rotate, a, right );
	obj_->up = q_rotate * obj_->up;
	obj_->pos = q_rotate * obj_->pos;
}


/// Roll the camera by an angle in degrees

void Camera::Roll(float a){
	glm::quat q_rotate;
	q_rotate = glm::rotate( q_rotate, a,  glm::normalize(obj_->look - obj_->pos));
	obj_->up = q_rotate * obj_->up;	
}


/// Rotate the camera, basically moving the look point with a given quaternion

void Camera::Rotate(const glm::quat &q) {
	glm::vec3 dt = obj_->look - obj_->pos;
	dt = dt * glm::toMat3(q);
	obj_->look = obj_->pos + dt;
}