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

		template< class T>
		class SEBUROAPI GLFWWindowManager;
		

		/**
		 * A basic GLFW Window (a nice wrapper basically but with some functions attached)
		 */

		template<class T>
		class GLWindow  {
		public:
			GLWindow (const char * title, size_t width, size_t height) { 
				Create(title,width,height,false, nullptr,-1,-1); 
			}

			GLWindow (const char * title, size_t width, size_t height,
				 	WindowListener<GLWindow> *listener,
					void (T::*init_func)(), 
					void (T::*draw_func)(double_t dt),
					bool fullscreen, const char * monitor_name, int major, int minor )  { 

				Create(title, width, height, fullscreen, monitor_name, major, minor); 	
				listener_ = listener;

				init_func_ = std::bind(init_func, listener_);
				draw_func_ = std::bind(draw_func, listener_, std::placeholders::_1);

				init_func_();

			}


			//void SetInitFunc(	void (*init_func)() ) { init_func_ = init_func;  }
			//void SetDrawFunc(	void (*draw_func)(double_t dt) ) { 	draw_func_ = draw_func;  }
			//void SetResizeFunc(	void (*resize_func)(size_t width, size_t height) ) {  resize_func_ = resize_func;  }

			GLFWwindow* glfw_window() const { return glfw_window_; }

		protected:

			void Create(const char * title, size_t width, size_t height, bool fullscreen, const char * monitor_name, int major, int minor);

			friend class GLFWWindowManager<T>;

			void set_glfw_window(GLFWwindow *glfw_window) { glfw_window_ = glfw_window; }


			std::function<void(double_t)> draw_func_;
			std::function<void()> init_func_;

			WindowListener<GLWindow> *listener_;


			GLFWwindow* glfw_window_;
		};

		/**
		 * A static wrapper around a C++ style class for GLFW - delgates to app class
		 * Calls GLEW to setup the context
		 * Considered a template but the static, C-Like nature of GLFW made this more annoying
		 */

		template< class T>
		class SEBUROAPI GLFWWindowManager : public WindowManager<GLWindow<T> > {
			
		protected:

			friend class GLWindow<T>;

			static void ErrorCallback(int error, const char* description);

			static void Reshape(GLFWwindow* window, int w, int h);
  
			static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

			static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

			static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

			static void WindowCloseCallback(GLFWwindow* window);

			static void MousePositionCallback(GLFWwindow* window, double x, double y);

			static void MouseWheelCallback(GLFWwindow* window, double xpos, double ypos);

			const GLWindow<T> & GetWindow(GLFWwindow* window) {
				for (auto it = windows_.begin(); it != windows_.end(); ++it) {
					if ((*it)->glfw_window() == window)
			    	return **it;
				}
				// Should never reach here! ><
				assert(false);
				return **(windows_.begin());
			}

		public:

			/**
			 * Constructor for the GLFW Application
			 * \parama WindowApp - an instance of the application to call
			 * \param major - the major version of OpenGL. Set to -1 if you want to go native
			 * \param minor - the minor version of OpenGL. Set to -1 if you want to go native
			 */
			
			GLFWWindowManager();

			const GLWindow<T> & CreateWindow(const char * title,  size_t width, size_t height, 
					WindowListener<GLWindow<T> > *listener, void (WindowListener<GLWindow<T> >::*init_func)(), 
					void (WindowListener<GLWindow<T> >::*draw_func)(double_t dt) ){

				windows_.push_back( std::unique_ptr<GLWindow<T> > (new GLWindow(title, width, height, listener, init_func, draw_func, false, nullptr, -1, -1)) );
				return *(windows_.back());
			}	
 
			
			void MainLoop() {
				pp_ = this;
				
				double_t t = glfwGetTime();

				for (auto it = windows_.begin(); it != windows_.end(); ++it) {
					
					const GLWindow<T> &w = **it;

					GLFWwindow* b = w.glfw_window();
					glfwMakeContextCurrent(b);
					glfwSwapInterval( 1 ); // vsync basically

					if (w.draw_func_ != nullptr){
						w.draw_func_(dt_);
					}

					glfwSwapBuffers(b);

				}

				dt_ = glfwGetTime() - t;

				glfwPollEvents();
			}


			void Shutdown();

		protected:

			static GLFWWindowManager 	*pp_;
	
      double_t 									dt_;
      size_t 										mx_, my_;
      uint16_t 									flag_;

		};

	}


}
#endif

