/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 21/10/2013
*
*/

#ifndef TEXTUREAPP_HPP
#define TEXTUREAPP_HPP

#include "s9/common.hpp"
#include "s9/file.hpp"
#include "s9/camera.hpp"
#include "s9/shapes.hpp"
#include "s9/node.hpp"
#include "s9/gl/drawable.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/glfw_app.hpp"
#include "s9/image.hpp"
#include "s9/gl/texture.hpp"

#include "anttweakbar/AntTweakBar.h"

 
namespace s9 {

	/*
 	 * Example of texturing a basic Quad
 	 */

	class TextureApp : public WindowApp, public WindowResponder {
	public:
		void init();
		void display(double_t dt);
		void update(double_t dt);

		// Event handling - you can choose which to override
		void processEvent(MouseEvent e);
		void processEvent(KeyboardEvent e);
		void processEvent(ResizeEvent e);

		
	protected:
		Quad quad_;
		Image image_;
		gl::Texture texture_;
		gl::Shader shader_;
		Camera camera_;
		Node node_;

		float rotation_;
		
	};
}

#endif
