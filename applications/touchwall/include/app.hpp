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
#include "s9/gl/asset.hpp"
#include "s9/gl/glfw_app.hpp"
#include "s9/gl/fbo.hpp"
#include "s9/cvprocess.hpp"


#include <anttweakbar/AntTweakBar.h>

#ifdef _SEBURO_X11_GLX	
#include "s9/linux/gtk_functions.hpp"
#include "s9/linux/defines.hpp"
#endif

using namespace s9;

 
namespace touchwall {

	/*
 	 * A touchwall application for copying
 	 */

	class TouchwallApp : public WindowApp, WindowResponder{
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
			gl::Quad			_test_quad;
			gl::Quad			_video_quad;
			OrbitCamera 	_camera;
			ScreenCamera 	_camera_screen;
			gl::Shader 		_shader_basic;
			gl::Shader 		_shader_video;
			gl::VidCam 		_video;
			compvis::Process _process;

			gl::Texture 	_tex_grey0;
			gl::Texture 	_tex_grey1;
			gl::Texture 	_tex_grey2;
			gl::Texture 	_tex_grey3;
			gl::Texture 	_tex_final;
			
			cv::Mat				_cv_result;
			XMLSettings 	_settings;
		};	

		boost::shared_ptr<SharedObj> _obj;
		TwBar *_bar; 

	};
}

#endif
