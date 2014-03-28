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

#ifdef USE_ANTTWEAKBAR
#include <AntTweakBar/AntTweakBar.h>
#endif

namespace s9 {

	namespace gl {

		class SEBUROAPI GLFWWindowManager;
		


		/**
		 * A basic GLFW Window (a nice wrapper basically but with some functions attached)
		 */

		class GLWindow  {
		public:
			GLWindow (const char * title, size_t width, size_t height) { 
				Create(title,width,height,false, nullptr,-1,-1,nullptr); 
			}

			GLWindow (const char * title, size_t width, size_t height,
				 	std::function<void()> init_func,
					std::function<void(double_t)> draw_func,
					bool fullscreen, const char * monitor_name, int major, int minor, GLFWwindow *share )  { 

				Create(title, width, height, fullscreen, monitor_name, major, minor, share ); 

				init_func_ = init_func;
				draw_func_ = draw_func;

			
				init_func_();

			}


			//void SetInitFunc(	void (*init_func)() ) { init_func_ = init_func;  }
			//void SetDrawFunc(	void (*draw_func)(double_t dt) ) { 	draw_func_ = draw_func;  }
			//void SetResizeFunc(	void (*resize_func)(size_t width, size_t height) ) {  resize_func_ = resize_func;  }

			GLFWwindow* glfw_window() const { return glfw_window_; }
			std::string window_name() const { return window_name_; }

		protected:

			void Create(const char * title, size_t width, size_t height, bool fullscreen, const char * monitor_name, int major, int minor, GLFWwindow *share);

			friend class GLFWWindowManager;

			void set_glfw_window(GLFWwindow *glfw_window) { glfw_window_ = glfw_window; }

			std::function<void(double_t)> draw_func_;
			std::function<void()> init_func_;

			std::string window_name_;
			GLFWwindow* glfw_window_;
		};

		/**
		 * A static wrapper around a C++ style class for GLFW - delgates to app class
		 * Calls GLEW to setup the context
		 * Considered a template but the static, C-Like nature of GLFW made this more annoying
		 */

		class SEBUROAPI GLFWWindowManager : public WindowManager<GLWindow > {
			
		protected:

			friend class GLWindow;

			static void ErrorCallback(int error, const char* description);

			static void Reshape(GLFWwindow* window, int w, int h);
  
			static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

			static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

			static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

			static void WindowCloseCallback(GLFWwindow* window);

			static void MousePositionCallback(GLFWwindow* window, double x, double y);

			static void MouseWheelCallback(GLFWwindow* window, double xpos, double ypos);

			const GLWindow & GetWindow(GLFWwindow* window) {
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

			const GLWindow & CreateWindow(const char * title,  size_t width, size_t height, 
					std::function<void()> init_func,
					std::function<void(double_t)> draw_func){

				///\todo Default is to create Shared Contexts
				GLFWwindow* share = nullptr;
				if (windows_.size() > 0){
#ifdef DEBUG
					std::cout << "SEBURO - Sharing Context." << std::endl;
#endif
					share = windows_.front()->glfw_window();
				}

				windows_.push_back( std::unique_ptr<GLWindow> (new GLWindow(title, width, height, init_func, draw_func, false, nullptr, -1, -1, share)) );
				
				Reshape(windows_.back()->glfw_window(), width,height);

				return *(windows_.back());
			}	

			/// This is the full fat create window

			const GLWindow & CreateWindow(const char * title,  size_t width, size_t height, 
					std::function<void()> init_func,
					std::function<void(double_t)> draw_func,
					bool fullscreen, const char * monitor_name, int major, int minor){

				///\todo Default is to create Shared Contexts
				GLFWwindow* share = nullptr;
				if (windows_.size() > 0){
#ifdef DEBUG
					std::cout << "SEBURO - Sharing Context." << std::endl;
#endif
					share = windows_.front()->glfw_window();
				}

				windows_.push_back( std::unique_ptr<GLWindow> (new GLWindow(title, width, height, init_func, draw_func, fullscreen, monitor_name, major, minor, share)) );
				
				Reshape(windows_.back()->glfw_window(), width,height);

				return *(windows_.back());
			}
 
			
			void MainLoop() {
				pp_ = this;
				
				double_t t = glfwGetTime();

				// Seems odd but it keeps the contexts running at 60fps! :D
				if (windows_.size() == 1)
					glfwSwapInterval( 1 );
				else
					glfwSwapInterval( 0 );

				for (auto it = windows_.begin(); it != windows_.end(); ++it) {
					
					const GLWindow &w = **it;

					GLFWwindow* b = w.glfw_window();
					glfwMakeContextCurrent(b);
				

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

