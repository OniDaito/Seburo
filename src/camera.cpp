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

void Camera::init() {
	if (obj_ == nullptr){
		obj_ = shared_ptr<SharedObject>(new SharedObject());
		reset();
	}
}


	
void Camera::resize(size_t w, size_t h){ 	
	set_ratio( static_cast<float>(w) / h);

	// For ortho, we are setting bottom left to 0,0 :)
	obj_->right = w;
	obj_->bottom = 0;
	obj_->left = 0;
	obj_->top = h;

	update();
}

void Camera::update() {	
 	
 	obj_->view_matrix = glm::lookAt(obj_->pos, obj_->look, obj_->up);
 	if (obj_->orthographic)
 		obj_->projection_matrix = glm::ortho(static_cast<float>(obj_->left), static_cast<float>(obj_->right),
 			static_cast<float>(obj_->bottom), static_cast<float>(obj_->top), obj_->near, obj_->far);
 	else
		obj_->projection_matrix = glm::perspective(obj_->field, obj_->ratio, obj_->near, obj_->far);
}

void Camera::reset() {
	obj_->up = glm::vec3(0,1,0);
	obj_->pos = glm::vec3(0,0,1.0);
	obj_->look = glm::vec3(0,0,0);
	obj_->near = 0.1f;
	obj_->far = 100.0f;
	obj_->ratio = 1.0;
	obj_->left = 0;
	obj_->top = 0;
	obj_->right = 100;
	obj_->bottom = 100;
	obj_->field = 55.0f;

	update();
}


void Camera::set_ratio(float r) {
	obj_->ratio = r;
	update();
}


void Camera::zoom(float z) {
	glm::vec3 dir = obj_->pos - obj_->look;
	dir = glm::normalize(dir);
	dir *= z;
	obj_->pos += dir;
	update();

}

void Camera::shift(glm::vec2 s) {
	glm::vec3 dir = obj_->pos - obj_->look;
	dir = glm::normalize(dir);
	glm::vec3 shiftx = glm::cross(dir,obj_->up);
	shiftx *= s.x;
	glm::vec3 shifty = obj_->up * s.y;

	obj_->pos += shiftx + shifty;
	obj_->look += shiftx + shifty;
	update();
}

void Camera::yaw(float a){
	glm::quat q_rotate;
	q_rotate = glm::rotate( q_rotate, a, obj_->up );
	obj_->up = q_rotate * obj_->up;
	obj_->pos = q_rotate * obj_->pos;
	update();
}

void Camera::pitch(float a){
	glm::quat q_rotate;
	
	glm::vec3 right = glm::normalize(glm::cross(obj_->up, glm::normalize(obj_->look - obj_->pos)));
	
	q_rotate = glm::rotate( q_rotate, a, right );
	obj_->up = q_rotate * obj_->up;
	obj_->pos = q_rotate * obj_->pos;
	update();
}

void Camera::roll(float a){
	glm::quat q_rotate;
	q_rotate = glm::rotate( q_rotate, a,  glm::normalize(obj_->look - obj_->pos));
	obj_->up = q_rotate * obj_->up;	
	update();
}



