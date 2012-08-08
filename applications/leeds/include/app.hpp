/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef LEEDS_APP_HPP
#define LEEDS_APP_HPP

#include "s9/common.hpp"
#include "s9/camera.hpp"
#include "s9/s9xml.hpp"
#include "s9/gl/shapes.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/video.hpp"
#include "s9/gl/glasset.hpp"
#include "s9/gl/glfw_app.hpp"
#include "s9/gl/fbo.hpp"


#include <anttweakbar/AntTweakBar.h>

#ifdef _GEAR_X11_GLX	
#include "s9/linux/gtk_functions.hpp"
#include "s9/linux/defines.hpp"
#endif

#include "scanner.hpp"

using namespace s9;

 
namespace leeds {


	// special typedefs
	typedef Geometry<VertPNT8F> GeometryLeeds;

	/*
 	 * An Basic App that draws a quad and provides a basic camera
 	 */

	class Leeds : public WindowApp, WindowResponder{
	public:
		void init();
		void display(double_t dt);
		void parseXML(std::string filename);


		// Event handling - you can choose which to override
		void processEvent(MouseEvent e);
		void processEvent(KeyboardEvent e);
		void processEvent(ResizeEvent e);
		
	protected:

		// Internal functions
		void createTextured();
		void addTweakBar();

		// Display functions
		void drawCameras();
		void drawMesh(Camera &cam);
		void drawTexturedMesh(Camera &cam);
		void drawGripper(Camera &cam);
		void drawDisplays();
		void drawToPicker();

		// Callbacks for Cameras
		static void TW_CALL _setContrast(const void *value, void *clientData);
		static void TW_CALL _getContrast(void *value, void *clientData) { *(uint8_t *)value =  static_cast<Leeds*>(clientData)->_cam_contrast; };

		static void TW_CALL _setBrightness(const void *value, void *clientData);
		static void TW_CALL _getBrightness(void *value, void *clientData) { *(uint8_t *)value =  static_cast<Leeds*>(clientData)->_cam_brightness; };

		static void TW_CALL _setGain(const void *value, void *clientData);
		static void TW_CALL _getGain(void *value, void *clientData) { *(uint8_t *)value =  static_cast<Leeds*>(clientData)->_cam_gain; };

		static void TW_CALL _setExposure(const void *value, void *clientData);
		static void TW_CALL _getExposure(void *value, void *clientData) { *(uint8_t *)value =  static_cast<Leeds*>(clientData)->_cam_exposure; };

		static void TW_CALL _setSaturation(const void *value, void *clientData);
		static void TW_CALL _getSaturation(void *value, void *clientData) { *(uint8_t *)value =  static_cast<Leeds*>(clientData)->_cam_saturation; };


		static void TW_CALL _generateTexturedCallback(void * obj);

		TwBar *pBar; 

		// Geometry
		gl::Quad _test_quad;
		gl::Quad _camera_quad;
		gl::Quad _fbo_quad;
		gl::GLAsset<GeometryPNF> _gripper_model;
		gl::GLAsset<GeometryPNF> _ground_model;
		gl::GLAsset<GeometryLeeds> _ground_textured_model;
		gl::GLAsset<GeometryLeeds> _textured_model;
		gl::GLAsset<GeometryPNF> _model;

		// Cameras
		InertiaCam _camera;
		Camera _gripper_camera;
		Camera _overview_camera;
		ScreenCamera _screen_camera;

		// Settings
		XMLSettings mSettings;

		// Video Cameras
		std::vector<gl::VidCam> vCameras;
		std::vector<gl::CVVidCam> vCVCameras;

		// Shaders
		gl::Shader _shader_camera;
		gl::Shader _shader_basic;
		gl::Shader _shader_light;
		gl::Shader _shader_leeds;
		gl::Shader _shader_pick;

		//FBOs
		gl::FBO 	_gripper_fbo;
		gl::FBO 	_overview_fbo;
		gl::FBO 	_picking_fbo;

		// Video Camera settings
		uint8_t _cam_contrast;
		uint8_t _cam_gain;
		uint8_t _cam_brightness;
		uint8_t _cam_exposure;
		uint8_t _cam_saturation;

		uint32_t mScreenW, mScreenH;

		// Mesh Reconstruction
		bool _scanning;
		Scanner _scanner;
	};
}

#endif
