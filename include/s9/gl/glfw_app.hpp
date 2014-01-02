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

//#ifdef _SEBURO_LINUX	
//#include <gtkmm.h>
//#endif

#include <thread>

namespace s9 {

	namespace gl {

		/**
		 * A static wrapper around a C++ style class for GLFW - delgates to app class
		 * Calls GLEW to setup the context
		 * Considered a template but the static, C-Like nature of GLFW made this more annoying
		 */

		class SEBUROAPI GLFWApp : WindowSystem{
			
		protected:

			bool running_;
			std::vector<GLFWwindow* > windows_;
			double_t dt_;
			size_t mx_, my_;
			uint16_t flag_;

			static void initGL(size_t w, size_t h, int major, int minor, int depthbits);

			static void _error_callback(int error, const char* description);

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

			GLFWApp (WindowApp &app, const size_t w = 800, const size_t h = 600, 
				bool fullscreen = false, int argc = 0, const char * argv[] = NULL, 
				const char * title = "SEBURO", const int major = -1, const int minor = -1, 
				const int depthbits = 16,  bool auto_start = true);

			void run();

			void shutdown();

			void mainLoop();

			virtual void MainLoopCallback() {};




			static GLFWwindow* createWindow(const char * title, int w, int h);
			
			std::vector<GLFWwindow* >& windows() { return windows_; }


		private:

			static GLFWApp *pp_;
			static std::string title_;

			int requested_major_;
			int requested_minor_;
			int requested_depth_bits_;
			size_t initial_width_;
			size_t initial_height_;

		};

	}


}
#endif

