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
 * A perspective or orthographic camera that provides the usual functions a camera model may want
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

		void Init(); // Called to init this class with default params
			
		void Resize(size_t w, size_t h, size_t l=0, size_t b=0);

		void set_near(float n) {obj_->near = n;};
		void set_far(float n) {obj_->far = n;};
		
		void set_ratio(float r);
		void set_field(float a) {obj_->field = a;};
		
		inline glm::vec3 pos()	{return obj_->pos; };
		inline glm::vec3 look() {return obj_->look; };
		inline glm::vec3 up() {return obj_->up; };

		inline size_t left()	{return obj_->left; };
		inline size_t bottom() {return obj_->bottom; };
		inline size_t width() {return obj_->width; };
		inline size_t height() {return obj_->height; };

		void Zoom(float z);
		void Shift(glm::vec2 s);
		void Yaw(float a);
		void Pitch(float a);
		void Roll(float a);
		void Rotate(const glm::quat &q);

		void set_orthographic(bool b) {obj_->orthographic = b;}
		bool orthographic() {return obj_->orthographic;}

		void Defaults();

		void set_pos(const glm::vec3 &p) { obj_->pos = p;}
		void set_look(const glm::vec3 &p) { obj_->look = p;}
		void set_up(const glm::vec3 &p) { obj_->up = p;}
		void set_left(size_t p) { obj_->left = p; }
		void set_height(size_t p) { obj_->height = p;}
		void set_width(size_t p) { obj_->width = p; }
		void set_bottom(size_t p) { obj_->bottom = p;}
		
		// Not ideal but provides an override for when we want to update the matrices manually
		bool update_on_node_draw() {return obj_->update_on_node_draw; }
		void set_update_on_node_draw(bool b) {obj_->update_on_node_draw =b; }

		///\todo Could this be misleading? Its mostly used in setting shaders through a contract
		glm::mat4& view_matrix() { return obj_->view_matrix; };
		glm::mat4& projection_matrix() { return obj_->projection_matrix; };

		void set_view_matrix(const glm::mat4  &m) { obj_->view_matrix = m; };
		void set_projection_matrix(const glm::mat4  &m) { obj_->projection_matrix = m; }; 

		virtual void Update();
		virtual void Update(double_t dt) { Update(); };

	protected:

		struct SharedObject {

			SharedObject() {
				up = glm::vec3(0,1.0f,0);
				pos = glm::vec3(0,0,10.0f);
				look = glm::vec3(0,0,0);
				near = 0.1f;
				far = 100.0f;
				ratio = 1.0;
			
				field = 55.0f;
				orthographic = false;
				update_on_node_draw = true;
				left = 0;
				bottom = 0;
				width = 800;
				height = 600;
	
			}

			glm::mat4 view_matrix;
			glm::mat4 projection_matrix;

			bool orthographic, update_on_node_draw;
			float ratio, far, near, field;
			size_t left, width, bottom, height; // Held for orthographic as well
		
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

