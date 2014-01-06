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
#include "../window_app.hpp"
#include "utils.hpp"

#include <GLFW/glfw3.h>
#include <anttweakbar/AntTweakBar.h>

#include <thread>

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

		class SEBUROAPI GLFWApp : public WindowSystem {
			
		protected:
			bool running_;
			std::vector<GLFWwindow* > windows_;
			double_t dt_;
			size_t mx_, my_;
			uint16_t flag_;

			static void initGL();

			static void ErrorCallback(int error, const char* description);

			static void Update();

			static void Reshape(GLFWwindow* window, int w, int h);

			static void Display(GLFWwindow* window);
  
      static void Display();

			static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

			static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

			static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

			static void WindowCloseCallback(GLFWwindow* window);

			static void MousePositionCallback(GLFWwindow* window, double x, double y);

			static void MouseWheelCallback(GLFWwindow* window, double xpos, double ypos);

			bool MainLoop();

		public:

			/**
			 * Constructor for the GLFW Application
			 * \param app - an instance of a WindowApp
			 * \param w - the initial width. Set to 0 to go fullscreen at native res.
			 * \param h - the initial height. Set to 0 to go fullscreen at native res.
			 * \param title - the title for the Window itself. Defaults to 'SEBURO'
			 * \param major - the major version of OpenGL. Set to -1 if you want to go native
			 * \param minor - the minor version of OpenGL. Set to -1 if you want to go native
			 * \param depthbit - the size of the depth buffer
			 * \param monitor name - if going fullscreen, provide the name of the monitor
			 */
			
			GLFWApp (WindowApp &app, const size_t w, const size_t h, 
				int argc = 0, const char * argv[] = NULL, 
				const char * title = "SEBURO", const int major = -1, const int minor = -1,  
				const int depthbits = 16,
				const char* monitor_name = nullptr);

			virtual void Shutdown();

			virtual void MainLoopCallback() {};

			static GLFWwindow* CreateWindow(const char * title, int w, int h, const char * monitor_name = nullptr);
			
			std::vector<GLFWwindow* >& windows() { return windows_; }

			virtual int Run();

		protected:

			static GLFWApp *pp_;
			static std::string title_;

			std::thread *update_thread_;

			int requested_major_;
			int requested_minor_;
			int requested_depth_bits_;
			bool fullscreen_;
			size_t initial_width_;
			size_t initial_height_;
			const char * monitor_name_;

		};

#ifdef _SEBURO_LINUX

		/// A class for Linux that allows a second GTK Window and uses GTK as the timing mechanism
		class SEBUROAPI GLFWGTKApp : public GLFWApp {
		public:
			GLFWGTKApp (WindowApp &app, const size_t w = 800, const size_t h = 600, 
				int argc = 0, const char * argv[] = NULL, 
				const char * title = "SEBURO", const int major = -1, const int minor = -1,  
				const int depthbits = 16,
				const char* monitor_name = nullptr);

			virtual int Run(Gtk::Window &window);
			virtual void Shutdown();

		protected:

			Glib::RefPtr<Gtk::Application> gtk_app_;
			sigc::connection idle_connection_;
		};

#endif

	}


}
#endif

