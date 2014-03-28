/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef MULTIPLE_APP_HPP
#define MULTIPLE_APP_HPP

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

#include <AntTweakBar/AntTweakBar.h>

 
namespace s9 {

	/*
 	 * An Basic App that draws a quad and provides a basic camera
 	 * A little multi-inheritence but one is only an interface so its cool
 	 */

	class MultipleApp : public Application, public WindowListener<gl::GLWindow> {
	public:
		MultipleApp();
		
		void InitZero();
		void InitOne();

		void MainLoop(double_t dt);
		
		void DrawZero(double_t dt);
		void DrawOne(double_t dt);

		// Event handling - you can choose which to override
		void ProcessEvent(const gl::GLWindow & window, MouseEvent e);
		void ProcessEvent(const gl::GLWindow & window, KeyboardEvent e);
		void ProcessEvent(const gl::GLWindow & window, ResizeEvent e);
		void ProcessEvent(const gl::GLWindow & window, CloseWindowEvent e);
		
	protected:

		gl::GLFWWindowManager window_manager_;

		gl::Texture texture_;
		Cuboid cuboid_;
		Node node_;
		
		Node top_node_zero_;
		Node top_node_one_;

		TwBar *tweakbar_;

		gl::Shader shader_, shader_texture_;
		
		Camera camera_zero_, camera_one_;
		
		Quad quad_zero_, quad_one_;

		float rotation_;

		size_t frames_;
		double_t time_passed_;
		
	};
}

#endif
