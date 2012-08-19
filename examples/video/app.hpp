/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef VIDEOAPP_HPP
#define VIDEOAPP_HPP

#include "s9/common.hpp"
#include "s9/camera.hpp"
#include "s9/gl/shapes.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/video.hpp"
#include "s9/gl/glfw_app.hpp"


#include <anttweakbar/AntTweakBar.h>

 
namespace s9 {

	/*
 	 * A Video App that grabs a camera and draws it to the screen
 	 */

	class VideoApp : public WindowApp, WindowResponder{
	public:
		void init();
		void display(double_t dt);

		// Event handling - you can choose which to override
		void processEvent(MouseEvent e);
		void processEvent(KeyboardEvent e);
		void processEvent(ResizeEvent e);
		
	protected:
		gl::Quad _test_quad;
		gl::Shader _shader;
		ScreenCamera _camera;
		gl::VidCam _video;
		
	};
}

#endif
