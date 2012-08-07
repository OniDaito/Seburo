/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef MODELAPP_HPP
#define MODELAPP_HPP

#include "s9/common.hpp"
#include "s9/asset.hpp"
#include "s9/camera.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/glasset.hpp"
#include "s9/gl/glfw_app.hpp"

#include <anttweakbar/AntTweakBar.h>

 
namespace s9 {

	/*
 	 * The Model App loads the Stanford Bunny and shades it
 	 */

	class ModelApp : public WindowApp, WindowResponder{
	public:
		void init();
		void display(double_t dt);

		// Event handling - you can choose which to override
		void processEvent(MouseEvent e);
		void processEvent(KeyboardEvent e);
		void processEvent(ResizeEvent e);
		
	protected:
		gl::GLAsset<GeometryPNF> mGeometry;
		gl::Shader mShader;
		InertiaCam mCamera;
		
		double_t mPrevT;
	};
}

#endif
