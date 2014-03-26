/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef BASICAPP_HPP
#define BASICAPP_HPP

#include "s9/application.hpp"
#include "s9/window.hpp"
#include "s9/common.hpp"
#include "s9/file.hpp"
#include "s9/camera.hpp"
#include "s9/shapes.hpp"
#include "s9/node.hpp"
#include "s9/gl/drawable.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/glfw.hpp"

 
namespace s9 {

	/*
 	 * An Basic App that draws a quad and provides a basic camera
 	 * A little multi-inheritence but one is only an interface so its cool
 	 */

	class BasicApp : public WindowListener<gl::GLWindow> {
	public:
		BasicApp();
		void Init();
		void MainLoop(double_t dt);
		void Draw(double_t dt);
		

		// Event handling - you can choose which to override
		void ProcessEvent(const gl::GLWindow & window, MouseEvent e);
		void ProcessEvent(const gl::GLWindow & window, KeyboardEvent e);
		void ProcessEvent(const gl::GLWindow & window, ResizeEvent e);
		void ProcessEvent(const gl::GLWindow & window, CloseWindowEvent e);
		
	protected:

		gl::GLFWWindowManager<BasicApp> window_manager_;

		Cuboid cuboid_;
		Node node_;
		Node top_node_;
		gl::Shader shader_;
		Camera camera_;
		Node top_node_2_;

		float rotation_;
		
	};
}

#endif
