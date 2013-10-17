/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 17/10/2013
*
*/

#ifndef OCULUSAPP_HPP
#define OCULUSAPP_HPP

#include "s9/common.hpp"
#include "s9/file.hpp"
#include "s9/camera.hpp"
#include "s9/shapes.hpp"
#include "s9/node.hpp"
#include "s9/gl/drawable.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/glfw_app.hpp"

#include "s9/oculus/oculus.hpp"

#include "anttweakbar/AntTweakBar.h"

 
namespace s9 {

	/*
 	 * An Basic App that draws a quad and provides a basic camera
 	 */

	class OculusApp : public WindowApp, WindowResponder{
	public:
		void init();
		void display(double_t dt);
		void update(double_t dt);

		// Event handling - you can choose which to override
		void processEvent(MouseEvent e);
		void processEvent(KeyboardEvent e);
		void processEvent(ResizeEvent e);
		
	protected:
		Cuboid cuboid_;
		gl::Shader shader_;
		InertiaCam camera_;

		oculus::OculusBase oculus_;

		float rotation_;
		
	};
}

#endif
