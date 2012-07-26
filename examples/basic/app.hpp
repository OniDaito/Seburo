/*
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
#include "gl/shapes.hpp"
#include "gl/shader.hpp"
#include "gl/glfw_app.hpp"

#include <anttweakbar/AntTweakBar.h>

 
namespace s9 {

	/*
 	 * An Basic App that draws a quad and provides a basic camera
 	 */

	class BasicApp : public VisualApp{
	public:
		void init();
		void display(double_t dt);

		// Event handling - you can choose which to override
		void fireEvent(MouseEvent &e);
		void fireEvent(KeyboardEvent &e);
		void fireEvent(ResizeEvent &e);
		
	protected:
		gl::Quad mTestQuad;
		gl::Shader mShader;
		InertiaCam<OrbitCamera> mCamera;
		
		double_t mPrevT;
	};
}

#endif
