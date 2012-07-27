/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef LEEDSAPP_HPP
#define LEEDSAPP_HPP

#include "s9gear.hpp"
#include "common.hpp"
#include "gl/shapes.hpp"
#include "gl/shader.hpp"
#include "gl/video.hpp"
#include "gl/geometry.hpp"
#include "gl/glfw_app.hpp"


#include <anttweakbar/AntTweakBar.h>

#ifdef _GEAR_X11_GLX	
#include "linux/gtk_functions.hpp"
#endif

 
namespace s9 {

	// special typedefs

	typedef VertexPNT8<float_t,float_t,float_t> VertPNT8F;
	typedef Geometry<VertPNT8F> GeometryLeeds;

	/*
 	 * An Basic App that draws a quad and provides a basic camera
 	 */

	class Leeds : public VisualApp{
	public:
		void init();
		void display(double_t dt);
		void parseXML(std::string filename);

		// Display functions
		void drawCameras();


		// Event handling - you can choose which to override
		void fireEvent(MouseEvent &e);
		void fireEvent(KeyboardEvent &e);
		void fireEvent(ResizeEvent &e);
		
	protected:
		// Geometry
		gl::Quad mTestQuad;
		gl::Quad mCamQuad;
		gl::Geometry<GeometryPNF> mGripper;
		gl::Geometry<GeometryLeeds> mMeshTextured;
		gl::Geometry<GeometryPNF> mMesh;

		// Cameras
		InertiaCam<OrbitCamera> mCamera;
		ScreenCamera mScreenCamera;

		// Settings
		XMLSettings mSettings;

		// Video Cameras
		std::vector<gl::VidCam> vCameras;
		std::vector<gl::CVVidCam> vCVCameras;

		// Shaders
		gl::Shader mShaderCamera;
		gl::Shader mShaderBasic;
		gl::Shader mShaderLighting;

		uint32_t mScreenW, mScreenH;
	};
}

#endif
