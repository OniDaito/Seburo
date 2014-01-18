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
#include "../window.hpp"
#include "utils.hpp"

#include <GLFW/glfw3.h>

#include <thread>
#include <ctime>
#include <chrono>

#ifdef _SEBURO_LINUX	
#include <gtkmm.h>
#endif

namespace s9 {

	namespace gl {


		/**
		 * A static wrapper around a C++ style class for GLFW - delgates to app class
		 * Calls GLEW to setup the context
		 * Considered a template but the static, C-Like nature of GLFW made this more annoying
		 */

		class SEBUROAPI GLFWApp : public WindowSystem<GLFWwindow*> {
			
		protected:

			static void InitGL(const int major = -1, const int minor = -1, const int depthbits = 16);

			static void ErrorCallback(int error, const char* description);

			static void Reshape(GLFWwindow* window, int w, int h);

			static void Display(GLFWwindow* window);
  
			static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

			static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

			static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

			static void WindowCloseCallback(GLFWwindow* window);

			static void MousePositionCallback(GLFWwindow* window, double x, double y);

			static void MouseWheelCallback(GLFWwindow* window, double xpos, double ypos);

			static void UpdateThread();

			void InitGLEW(GLFWwindow* window);

			bool MainLoop();

		public:

			/**
			 * Constructor for the GLFW Application
			 * \parama WindowApp - an instance of the application to call
			 * \param major - the major version of OpenGL. Set to -1 if you want to go native
			 * \param minor - the minor version of OpenGL. Set to -1 if you want to go native
			 */
			
			GLFWApp ( WindowApp<GLFWwindow*> &app, const int major = -1, const int minor = -1);

			virtual void CloseWindow(GLFWwindow* window);	
			void Run();

			GLFWwindow* CreateWindow(const char * title, size_t width, size_t height);	
			GLFWwindow* CreateWindowFullScreen(const char * title, size_t width, size_t height, const char * monitor_name);	

			std::vector<GLFWwindow* >& windows() { return windows_; }

			void AddWindowListener(WindowListener<GLFWwindow*> *listener) { eventor_.AddWindowListener(listener); }

			void Shutdown();

		protected:

			static GLFWApp 						*pp_;
			std::thread 							*update_thread_;
			std::chrono::time_point< std::chrono::high_resolution_clock> 	thread_start_;

			WindowEventor<GLFWwindow*> eventor_;
			WindowApp<GLFWwindow*> 		&app_;

    	bool running_;
      std::vector<GLFWwindow* > windows_;
      double_t 									dt_;
      size_t 										mx_, my_;
      uint16_t 									flag_;

		};

		/**
		 * This class represents the above, but with a UX window as well.
		 * Under Linux, this will be GTKMM. Windows and OSX are yet to be decided
		 */

		class WithUXApp : public GLFWApp {
		
		public:
			
			WithUXApp(WindowApp<GLFWwindow*> &app, int argc, const char * argv[], const int major = -1, const int minor = -1);

#ifdef _SEBURO_LINUX
			void Run(Gtk::Window &window);
#endif

			void Shutdown();

		protected:

#ifdef _SEBURO_LINUX
			Glib::RefPtr<Gtk::Application> gtk_app_;
			sigc::connection idle_connection_;
#endif

		};

	}


}
#endif

