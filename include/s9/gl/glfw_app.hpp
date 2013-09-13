/**
* @brief GLFW App 
* @file shapes.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 25/07/2012
*
*/

#ifndef GLFW_APP_HPP
#define GLFW_APP_HPP

#include "../common.hpp"
#include "common.hpp"
#include "../visualapp.hpp"
#include "utils.hpp"

#include <GLFW/glfw3.h>
#include <anttweakbar/AntTweakBar.h>

#ifdef _SEBURO_X11_GLX	
#include <gtkmm.h>
#endif

#include <boost/thread.hpp>

namespace s9 {

	namespace gl {

		/**
		 * A static wrapper around a C++ style class for GLFW - delgates to app class
		 * Calls GLEW to setup the context
		 * Considered a template but the static, C-Like nature of GLFW made this more annoying
		 */

		class SEBUROAPI GLFWApp : WindowSystem{

		protected:

			bool mRunning;
			std::vector<GLFWwindow* > vWindows;
			double_t _dt;
			size_t mMX, mMY;
			uint16_t mFlag;

			static void initGL(const int w, const int h,
				const int major, const int minor, const int depthbits);

			static void _error_callback(int error, const char* description);

			static void _shutdown();

			static void mainLoop();

			static void _update();

			static void _reshape(GLFWwindow* window, int w, int h);

			static void _display(GLFWwindow* window);
  
      static void _display();

			static void _keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

			static void _mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

			static void _scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

			static void _window_close_callback(GLFWwindow* window);

			static void _mousePositionCallback(GLFWwindow* window, double x, double y);

			static void _mouseWheelCallback(GLFWwindow* window, double xpos, double ypos);

		public:

			GLFWApp (WindowApp &app, const int w = 800, const int h = 600, 
				bool fullscreen = false, int argc = 0, const char * argv[] = NULL, 
				const char * title = "OpenGLCourse", const int major = 3, const int minor = 0, 
				const int depthbits = 16 );

			static GLFWwindow* createWindow(const char * title, int w, int h);
			
		private:

			static GLFWApp *pThis;
			static std::string mTitle;
			boost::thread *_update_thread;

		};

	}

}

#endif

