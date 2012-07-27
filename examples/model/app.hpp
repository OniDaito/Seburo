/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef MODELAPP_HPP
#define MODELAPP_HPP

#include "s9gear.hpp"
#include "common.hpp"
#include "asset.hpp"
#include "gl/shader.hpp"
#include "gl/geometry.hpp"
#include "gl/glfw_app.hpp"

#include <anttweakbar/AntTweakBar.h>

 
namespace s9 {

	/*
 	 * The Model App loads the Stanford Bunny and shades it
 	 */

	class ModelApp : public VisualApp{
	public:
		void init();
		void display(double_t dt);

		// Event handling - you can choose which to override
		void fireEvent(MouseEvent &e);
		void fireEvent(KeyboardEvent &e);
		void fireEvent(ResizeEvent &e);
		
	protected:
		gl::Geometry<GeometryPNF> mGeometry;
		gl::Shader mShader;
		InertiaCam<OrbitCamera> mCamera;
		
		double_t mPrevT;
	};
}

#endif
