/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 20/08/2012
*
*/

#ifndef TEMPLATE_APP_HPP
#define TEMPLATE_APP_HPP

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

using namespace s9;

 
namespace TemplateNamespace {

	/*
 	 * A template application for copying
 	 */

	class TemplateApp : public WindowApp, WindowResponder{
	public:
		void init();
		void display(double_t dt);
 		void update(double_t dt);
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
		static void TW_CALL _setValue(const void *value, void *clientData);
		static void TW_CALL _getValue(void *value, void *clientData) {  };

		static void TW_CALL _buttonCallback(void * obj);
		
		struct SharedObj{
			// stuff
			gl::Quad _test_quad;
			OrbitCamera _camera;
			gl::Shader _shader_basic;
			XMLSettings _settings;
		};	

		boost::shared_ptr<SharedObj> _obj;
		TwBar *_bar; 

	};
}

#endif
