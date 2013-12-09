/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/12/2013
*
*/

#ifndef BASICAPP_HPP
#define BASICAPP_HPP

#include "s9/common.hpp"
#include "s9/file.hpp"
#include "s9/camera.hpp"
#include "s9/shapes.hpp"
#include "s9/node.hpp"
#include "s9/gl/fbo.hpp"
#include "s9/gl/drawable.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/glfw_app.hpp"

#include "anttweakbar/AntTweakBar.h"

 
namespace s9 {

	/*
 	 * An Basic App that draws a quad and provides a basic camera
 	 */

	class FBOApp : public WindowApp, WindowResponder{
	public:
		void init();
		void display(double_t dt);
		void update(double_t dt);

		// Event handling - you can choose which to override
		void processEvent(MouseEvent e);
		void processEvent(KeyboardEvent e);
		void processEvent(ResizeEvent e);
		
	protected:
		Cuboid 			cuboid_;
		Node				node_quad_;
		Node 				node_;
		Quad				quad_;
		gl::Shader 	shader_;
		gl::Shader 	shader_warp_;
		Camera 			camera_;
		Camera 			camera_ortho_;
		gl::FBO 		fbo_;

		float rotation_;
		
	};
}

#endif
