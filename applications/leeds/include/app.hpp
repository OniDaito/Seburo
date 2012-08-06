/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef LEEDSAPP_HPP
#define LEEDSAPP_HPP

#include "s9/s9gear.hpp"
#include "s9/common.hpp"
#include "s9/gl/shapes.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/video.hpp"
#include "s9/gl/glasset.hpp"
#include "s9/gl/glfw_app.hpp"


#include <anttweakbar/AntTweakBar.h>

#ifdef _GEAR_X11_GLX	
#include "s9/linux/gtk_functions.hpp"
#endif

 
namespace s9 {

	// special typedefs
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
		void fireEvent(MouseEvent e);
		void fireEvent(KeyboardEvent e);
		void fireEvent(ResizeEvent e);
		
	protected:

		// Internal functions
		void createTextured();
		void addTweakBar();

		static void TW_CALL _generateTexturedCallback(void * obj);

		TwBar *pBar; 

		// Geometry
		gl::Quad mTestQuad;
		gl::Quad mCamQuad;
		gl::GLAsset<GeometryPNF> mGripper;
		gl::GLAsset<GeometryLeeds> mMeshTextured;
		gl::GLAsset<GeometryPNF> mMesh;

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
		gl::Shader mShaderLeeds;

		uint32_t mScreenW, mScreenH;
	};
}

#endif
