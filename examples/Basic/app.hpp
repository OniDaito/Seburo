/**
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef GLAPP_HPP
#define GLAPP_HPP

#include "s9gear.hpp"
#include "common.hpp"
#include "gl/shapes_gl.hpp"
#include "gl/shader.hpp"
#include "gl/glfw_app.hpp"

#include <anttweakbar/AntTweakBar.h>

 
namespace s9 {

	/*
 	 * An Basic App that draws a quad and allows movement
 	 */

	class BasicApp : public VisualApp{
	public:
		
		void display();
		void fireEvent(Event e);
		
	protected:

			
	};
}

#endif
