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
#include "states.hpp"

using namespace s9;

 
namespace leeds {


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

		// Callbacks for Cameras
		static void TW_CALL _setContrast(const void *value, void *clientData);
		static void TW_CALL _getContrast(void *value, void *clientData) { *(uint8_t *)value =  static_cast<SharedObj*>(clientData)->_cam_contrast; };

		static void TW_CALL _setBrightness(const void *value, void *clientData);
		static void TW_CALL _getBrightness(void *value, void *clientData) { *(uint8_t *)value =  static_cast<SharedObj*>(clientData)->_cam_brightness; };

		static void TW_CALL _setGain(const void *value, void *clientData);
		static void TW_CALL _getGain(void *value, void *clientData) { *(uint8_t *)value =  static_cast<SharedObj*>(clientData)->_cam_gain; };

		static void TW_CALL _setExposure(const void *value, void *clientData);
		static void TW_CALL _getExposure(void *value, void *clientData) { *(uint8_t *)value =  static_cast<SharedObj*>(clientData)->_cam_exposure; };

		static void TW_CALL _setSaturation(const void *value, void *clientData);
		static void TW_CALL _getSaturation(void *value, void *clientData) { *(uint8_t *)value =  static_cast<SharedObj*>(clientData)->_cam_saturation; };


		static void TW_CALL _generateTexturedCallback(void * obj);
		static void TW_CALL _scanningCallback(void * obj);


		TwBar *pBar; 

		SharedObjPtr _obj;
		StatePtr _state;
		StatePtr _state_mesh;
		StatePtr _state_textured;
		StatePtr _state_scanning;

	};
}

#endif
