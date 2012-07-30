/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef VIDEOAPP_HPP
#define VIDEOAPP_HPP

#include "s9gear.hpp"
#include "common.hpp"
#include "gl/shapes.hpp"
#include "gl/shader.hpp"
#include "gl/video.hpp"
#include "gl/glfw_app.hpp"


#include <anttweakbar/AntTweakBar.h>

 
namespace s9 {

	/*
 	 * A Video App that grabs a camera and draws it to the screen
 	 */

	class VideoApp : public VisualApp{
	public:
		void init();
		void display(double_t dt);

		// Event handling - you can choose which to override
		void fireEvent(MouseEvent e);
		void fireEvent(KeyboardEvent e);
		void fireEvent(ResizeEvent e);
		
	protected:
		gl::Quad mTestQuad;
		gl::Shader mShader;
		ScreenCamera mCamera;
		gl::VidCam mVideo;
		
	};
}

#endif
