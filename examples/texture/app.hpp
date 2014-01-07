/*
* @brief GLFW Texturing Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 21/10/2013
*
*/

#ifndef TEXTURE_APP_HPP
#define TEXTURE_APP_HPP

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

	class TextureApp : public WindowApp<GLFWwindow*> {
	public:
		void Init();
		void Display(GLFWwindow* window, double_t dt);
		void Update(double_t dt);

		// Event handling - you can choose which to override
		void ProcessEvent(MouseEvent e, GLFWwindow *window);
		void ProcessEvent(KeyboardEvent e, GLFWwindow *window);
		void ProcessEvent(ResizeEvent e, GLFWwindow *window);

		
	protected:
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
