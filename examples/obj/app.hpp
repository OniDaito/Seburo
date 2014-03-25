/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 17/01/2014
*
*/

#ifndef OBJ_APP_HPP
#define OBJ_APP_HPP

#include "s9/common.hpp"
#include "s9/file.hpp"
#include "s9/camera.hpp"
#include "s9/shapes.hpp"
#include "s9/node.hpp"
#include "s9/gl/drawable.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/glfw.hpp"
#include "s9/obj_mesh.hpp"

 
namespace s9 {

	/*
 	 * An Basic App that draws a quad and provides a basic camera
 	 * A little multi-inheritence but one is only an interface so its cool
 	 */

	class ObjApp : public WindowApp<GLFWwindow*> {
	public:
		void Init(Context context);
		void Display(Context context, GLFWwindow*  window, double_t dt);
		void Update(double_t dt);
		

		// Event handling - you can choose which to override
		void ProcessEvent(MouseEvent e, GLFWwindow* window);
		void ProcessEvent(KeyboardEvent e, GLFWwindow* window);
		void ProcessEvent(ResizeEvent e, GLFWwindow* window );
		void ProcessEvent(CloseWindowEvent e, GLFWwindow* window );
		
	protected:

		Node node_;
		ObjMesh obj_mesh_;
		gl::Shader shader_;
		Camera camera_;
	

		float rotation_;
		
	};
}

#endif
