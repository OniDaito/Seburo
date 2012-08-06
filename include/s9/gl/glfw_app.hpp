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

#include <GL/glfw3.h>
#include <anttweakbar/AntTweakBar.h>

#ifdef _GEAR_X11_GLX	
#include <gtkmm.h>
#endif

namespace s9 {

	namespace gl {

		/*
		 * A static wrapper around a C++ style class for GLFW - delgates to app class
		 * Calls GLEW to setup the context
		 * Considered a template but the static, C-Like nature of GLFW made this more annoying
		 */

		 ///\todo we can even subclass this and add in basics for certain app types :P
		 ///\todo this may need to be a service class - see the Modern C++ book


		class GLFWApp {

		protected:

			bool mRunning;
			std::vector<GLFWwindow> vWindows;
			double_t mDX;
			size_t mMX, mMY;
			uint16_t mFlag;

			/*
			 * Main loop calls the display function and checks for events
			 */

			static void mainLoop();

			/*
			 * GLFW Callback for resizing a window
			 */

			static void _reshape(GLFWwindow window, int w, int h);

			/*
			 * GLFW display
			 */

			static void _display(GLFWwindow window);
			
			/*
			 * GLFW Callback for the keyboard
			 */


			static void _keyCallback(GLFWwindow window, int key, int action);

			/*
			 * GLFW Mouse button callback - sends a full event with current position as well
			 */

			static void _mouseButtonCallback(GLFWwindow window, int button, int action);



			static int _window_close_callback(GLFWwindow window);

			/*
			 * GLFW Callback for resizing a window
			 */

			static void _mousePositionCallback(GLFWwindow window, int x, int y);

			static void _mouseWheelCallback(GLFWwindow window, double xpos, double ypos);

		public:
			GLFWApp(VisualApp *app, int argc, const char * argv[], const char * title);

			static GLFWwindow createWindow(const char * title, size_t w, size_t h);
			
			
			/*
			 * Perform OpenGL initialisation using GLEW
			 */

			static void init(int major, int minor);
			
		private:

			static VisualApp *pApp;
			static GLFWApp *pThis;
			static std::string mTitle;

		};

	}

}

#endif

