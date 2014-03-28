/*
* @brief GLFW Texturing Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 21/10/2013
*
*/

#ifndef TEXTURE_APP_HPP
#define TEXTURE_APP_HPP

#include "s9/application.hpp"
#include "s9/common.hpp"
#include "s9/file.hpp"
#include "s9/camera.hpp"
#include "s9/shapes.hpp"
#include "s9/node.hpp"
#include "s9/gl/drawable.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/glfw.hpp"
#include "s9/image.hpp"
#include "s9/gl/texture.hpp"
 
namespace s9 {

	/*
 	 * Example of texturing a basic Quad
 	 */

	class TextureApp : public Application, public WindowListener<gl::GLWindow>  {
	public:
		TextureApp();
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

		Quad quad_;
		Image image_;
		gl::Texture texture_;
		gl::Shader shader_;
		Camera camera_;
		Node node_;
		int unit_;

		float rotation_;
		
	};
}

#endif
