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
#include "string_utils.hpp"

/** 
 * Camera Class
 * A perspective camera that provides the usual functions a camera model may want
 * such as pitch, pan, zoom, resize etc.
 * SharedObject as it is passed around between nodes often
 */
 
namespace s9{
	 
	class SEBUROAPI Camera {

	public:


		Camera() {}

		Camera(glm::vec3 pos);
		Camera(glm::vec3 pos, glm::vec3 look);
		Camera(glm::vec3 pos, glm::vec3 look, glm::vec3 up);

		void init(); // Called to init this class with default params
			
		void resize(size_t w, size_t h);

		void set_near(float n) {obj_->near = n; update();};
		void set_far(float n) {obj_->far = n; update();};
		
		void set_ratio(float r);
		void set_field(float a) {obj_->field = a; update();};
		
		inline glm::vec3 pos()	{return obj_->pos; };
		inline glm::vec3 look() {return obj_->look; };
		inline glm::vec3 up() {return obj_->up; };

		void zoom(float z);
		void shift(glm::vec2 s);
		void yaw(float a);
		void pitch(float a);
		void roll(float a);
		void rotate(const glm::quat &q);

		void set_orthographic(bool b) {obj_->orthographic = b; update();}
		bool orthographic() {return obj_->orthographic;}

		void defaults();

		void set_pos(const glm::vec3 &p) { obj_->pos = p; update();}
		void set_look(const glm::vec3 &p) { obj_->look = p; update();}
		void set_up(const glm::vec3 &p) { obj_->up = p; update();}
		void set_left(size_t p) { obj_->left = p; update();}
		void set_top(size_t p) { obj_->top = p; update();}
		void set_right(size_t p) { obj_->right = p; update();}
		void set_bottom(size_t p) { obj_->bottom = p; update();}
		
		///\todo Could this be misleading? Its mostly used in setting shaders through a contract
		glm::mat4& view_matrix() { return obj_->view_matrix; };
		glm::mat4& projection_matrix() { return obj_->projection_matrix; };

		virtual void update();
		virtual void update(double_t dt) { update(); };

	protected:

		struct SharedObject {

			SharedObject() {
				up = glm::vec3(0,1.0f,0);
				pos = glm::vec3(0,0,10.0f);
				look = glm::vec3(0,0,0);
				near = 0.1f;
				far = 100.0f;
				ratio = 1.0;
				left = 0;
				top = 0;
				right = 100;
				bottom = 100;
				field = 55.0f;
				orthographic = false;
				view_left = 0;
				view_bottom = 0;
				view_width = 800;
				view_height = 600;
	
			}

			glm::mat4 view_matrix;
			glm::mat4 projection_matrix;

			bool orthographic;
			float ratio, far, near, field;
			size_t left, top, bottom, right; // Held for orthographic as well
			size_t view_width, view_left, view_bottom, view_height; // Viewport settings
		
			glm::vec3 pos, look, up;
		};
		
		std::shared_ptr<SharedObject> obj_ = nullptr;

	public:

	  bool operator == (const Camera &ref) const { return this->obj_ == ref.obj_; }
	  typedef std::shared_ptr<SharedObject> Camera::*unspecified_bool_type;
	  operator unspecified_bool_type() const { return ( obj_.get() == 0 ) ? 0 : &Camera::obj_; }
	  void reset() { obj_.reset(); }

	};


	

 }
	 

#endif

