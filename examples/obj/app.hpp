/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 17/01/2014
*
*/

#ifndef OBJ_APP_HPP
#define OBJ_APP_HPP

#include "s9/application.hpp"
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

	class ObjApp : public Application, public WindowListener<gl::GLWindow> {
	public:

		ObjApp();
		void Init();
		void Draw(double_t dt);
		void MainLoop(double_t dt);

		// Event handling - you can choose which to override
		void ProcessEvent(const gl::GLWindow & window, MouseEvent e);
		void ProcessEvent(const gl::GLWindow & window, KeyboardEvent e);
		void ProcessEvent(const gl::GLWindow & window, ResizeEvent e);
		void ProcessEvent(const gl::GLWindow & window, CloseWindowEvent e);
		
	protected:

		gl::GLFWWindowManager window_manager_;

		Node node_;
		ObjMesh obj_mesh_;
		gl::Shader shader_;
		Camera camera_;
	
		float rotation_;
		
	};
}

#endif
