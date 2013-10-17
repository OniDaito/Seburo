/**
* @brief Camera Classes
* @file camera.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/


#ifndef S9_CAMERA_HPP
#define S9_CAMERA_HPP

#include "common.hpp"
#include "events.hpp"
#include "visualapp.hpp"

/** 
 * Camera Class for a camera that pitches and yaws whilst keeping up always parallel with y
 * \todo ORTHO Camera! 
 * \todo certain camera classes could listen for events but we need to set a precidence (as sometimes we want
 * \todo update and resize hooks on all cameras
 * \todo create GL Cameras that extend this so we get viewport and such for free
 * camera movement and othertimes we want selection for example) - bubbling
 */
 
namespace s9{
	 
	class SEBUROAPI Camera {

	public:

		Camera();
		
		virtual void reset();
		
		void resize(size_t w, size_t h){ 
			set_ratio( static_cast<float>(w) / h);
			w_ = w;
			h_ = h;
		}

		void set_near(float n) {near_ = n; };
		void set_far(float n) {far_ = n; };
		
		void set_ratio(float r);
		void setField(float a) {field_ = a; };
		
		glm::vec3 pos() {return pos_; };
		glm::vec3 look() {return look_; };
		glm::vec3 up() {return up_; };


		void set_pos(glm::vec3 p) { pos_ = p; }
		void set_look(glm::vec3 p) { look_ = p; }
		void set_up(glm::vec3 p) { up_ = p; }
		
		glm::mat4 view_matrix() { return view_matrix_; };
		glm::mat4 projection_matrix() { return projection_matrix_; };

		virtual void update() {};
		virtual void update(double_t dt) {};

	protected:
		
		glm::mat4 view_matrix_;
		glm::mat4 projection_matrix_;

		float ratio_, far_, near_, field_;
		
		glm::vec3 pos_, look_, up_;

		size_t w_, h_; // Screen / FBO size

		
	};

	/* 
	 * Orbit camera revolves around one point as it were
	 */
	 
	class SEBUROAPI OrbitCamera : public Camera{
	public:
		OrbitCamera();
		void zoom(float z);
		void shift(glm::vec2 s);
		
		void yaw(float a);
		void pitch(float a);
		void roll(float a);
		void reset();

		/*
		 * Basic mouse movement for an orbiting camera
		 */

		virtual void processEvent(MouseEvent e) { 
			if(e.flag & MOUSE_LEFT_DOWN){
				glm::vec2 mouse_d = glm::vec2(e.x,e.y) - mouse_pre_;
				yaw(static_cast<float>(mouse_d.x) * sense_);
				pitch(static_cast<float>(mouse_d.y) * sense_);
			}

			if (e.flag & MOUSE_WHEEL_DOWN){
				zoom(-sense_ * 2);
			}
			else if (e.flag & MOUSE_WHEEL_UP){
				zoom(sense_ * 2);
			}

			if (e.flag & MOUSE_MIDDLE_DOWN){
				shift(glm::vec2(static_cast<float>(e.x - mouse_pre_.x) * sense_ * 0.8, 
						static_cast<float>(e.y - mouse_pre_.y) * sense_ * 0.8));
			}

			mouse_pre_ = glm::vec2(e.x, e.y);
		}

		void update() {
			view_matrix_ = glm::lookAt(pos_, look_, up_);
			projection_matrix_ = glm::perspective(field_, ratio_, near_, far_);
		}

	protected:
		
		float sense_;
		glm::vec2 mouse_pre_;
		
	};
	
	/*
	 * Screen Camera
	 */
	 
	class SEBUROAPI OrthoCamera : public Camera {
	public:
		OrthoCamera() {};

		void update() {
			view_matrix_ = glm::mat4(1.0f);
			projection_matrix_ = glm::ortho(static_cast<float>(0.0), static_cast<float>(w_), 
			static_cast<float>(h_), static_cast<float>(0.0));
		}
		
	};

	/*
	 * A template that adds inertia and mouse drag movements
	 * \todo this needs some proper signals
	 */

	class SEBUROAPI InertiaCam : public OrbitCamera {

	protected:
		glm::vec3 now_;
		glm::vec3 prev_;
		float angle_, degrade_;
		glm::vec3 p_;
		double_t dt_;
		bool held_;
		float zoom_, shift_;

	public:

		InertiaCam() { 
			now_ = glm::vec3(0.0f,0.0f,0.0f); 
		 	angle_ = 50.0;
			degrade_ = 2.0;
			dt_ = 0;
			held_ = false;
			zoom_ = 0.5;
			shift_ = 0.001;
		}

		void mouseEvent(MouseEvent e){

			if (e.flag & MOUSE_LEFT_DOWN){

				if (glm::length(p_) > 0){

					now_ = glm::vec3(static_cast<float>(e.x - p_.x), static_cast<float>(e.y - p_.y),0.0f);
					held_ = true;
				}

			}
			else if (e.flag & MOUSE_LEFT_UP ){
				p_ = glm::vec3(0.0f,0.0f,0.0f);
				held_ = false;
			}
			if (e.flag & MOUSE_WHEEL_DOWN){
				this->zoom(-zoom_);
			}
			else if (e.flag & MOUSE_WHEEL_UP){
				this->zoom(zoom_);
			}

			if (e.flag & MOUSE_MIDDLE_DOWN){
				this->shift(glm::vec2(static_cast<float>(e.x - p_.x) * shift_, 
						static_cast<float>(e.y - p_.y) * shift_));
			}

			p_ = glm::vec3(static_cast<float>(e.x), static_cast<float>(e.y),0.0f);
		}

		/*
		 * Update is called as often as possible to complete inertia
		 * \todo work out how best to do zooming and shifting
		 */ 

		void update(double_t dt){
			
			double_t ds = angle_ * dt;

			if (glm::length(now_) < 0.01){
				 now_ = glm::vec3(0.0f,0.0f,0.0f);
			}	
			else {
				this->yaw(now_.x * ds);
				this->pitch(now_.y * ds);
			}

			if (!held_) {
				now_ *= 1.0 - (dt * degrade_);
			}

			view_matrix_ = glm::lookAt(pos_, look_, up_);
			projection_matrix_ = glm::perspective(field_, ratio_, near_, far_);
		
		}
	};

 }
	 

#endif

