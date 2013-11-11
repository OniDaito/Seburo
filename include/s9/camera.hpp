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

/** 
 * Camera Class
 * A perspective camera that provides the usual functions a camera model may want
 * such as pitch, pan, zoom, resize etc.
 * SharedObject as it is passed around between nodes often
 */
 
namespace s9{
	 
	class SEBUROAPI Camera {

	public:

		Camera();
			
		void resize(size_t w, size_t h);

		void set_near(float n) {obj_->near = n; };
		void set_far(float n) {obj_->far = n; };
		
		void set_ratio(float r);
		void set_field(float a) {obj_->field = a; };
		
		inline glm::vec3 pos()	{return obj_->pos; };
		inline glm::vec3 look() {return obj_->look; };
		inline glm::vec3 up() {return obj_->up; };

		void zoom(float z);
		void shift(glm::vec2 s);
		void yaw(float a);
		void pitch(float a);
		void roll(float a);

		void set_orthographic(bool b) {obj_->orthographic = b;}
		bool orthographic() {return obj_->orthographic;}

		void reset();

		void set_pos(glm::vec3 p) { obj_->pos = p; }
		void set_look(glm::vec3 p) { obj_->look = p; }
		void set_up(glm::vec3 p) { obj_->up = p; }
		void set_left(size_t p) { obj_->left = p; }
		void set_top(size_t p) { obj_->top = p; }
		void set_right(size_t p) { obj_->right = p; }
		void set_bottom(size_t p) { obj_->bottom = p; }
		
		///\todo Could this be misleading? Its mostly used in setting shaders through a contract
		glm::mat4& view_matrix() { return obj_->view_matrix; };
		glm::mat4& projection_matrix() { return obj_->projection_matrix; };

		virtual void update();
		virtual void update(double_t dt) { update(); };

	protected:

		struct SharedObject {
			glm::mat4 view_matrix;
			glm::mat4 projection_matrix;

			bool orthographic;
			float ratio, far, near, field;
			size_t left, top, bottom, right; // Held for orthographic as well
		
			glm::vec3 pos, look, up;
		};
		
		std::shared_ptr<SharedObject> obj_ = nullptr;

	};



	/*

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
		*/


	/*
	 * A template that adds inertia and mouse drag movements
	 * \todo this needs some proper signals and inheriting properly
	 */

	class SEBUROAPI InertiaCam : public Camera {

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

			obj_->view_matrix = glm::lookAt(obj_->pos, obj_->look, obj_->up);
			obj_->projection_matrix = glm::perspective(obj_->field, obj_->ratio, obj_->near, obj_->far);
		
		}
	};

 }
	 

#endif

