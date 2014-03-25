/*
* @brief GLFW Application with a UX Second Window
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 02/01/2014
*
*/

#ifndef UXAPP_HPP
#define UXAPP_HPP

#include "s9/common.hpp"
#include "s9/file.hpp"
#include "s9/camera.hpp"
#include "s9/shapes.hpp"
#include "s9/node.hpp"
#include "s9/gl/drawable.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/glfw.hpp"
#include "s9/window.hpp"


// Build a basic GTK2+ Second window

#ifdef _SEBURO_LINUX
#include <gtk/gtk.h>
#endif

namespace s9 {

#ifdef _SEBURO_LINUX

	class UxApp;

	class UXWindow : public Gtk::Window {

	public:
	  UXWindow(UxApp &app);
	  virtual ~UXWindow();

	protected:
	  //Signal handlers:
	  void on_button_clicked();

	  Gtk::Button button_;

	  UxApp& app_;

	};

#endif

	/*
 	 * An Basic App that draws a quad and provides a basic camera
 	 */

	class UxApp : public WindowApp<GLFWwindow*>{
	public:
		void Init(Context context);
		void Display(Context context, GLFWwindow* window, double_t dt);
		void Update(double_t dt);

		// Event handling - you can choose which to override
		void ProcessEvent(MouseEvent e, GLFWwindow* window);
		void ProcessEvent(KeyboardEvent e, GLFWwindow* window);
		void ProcessEvent(ResizeEvent e, GLFWwindow* window);
		
	protected:
		Cuboid			cuboid_;
		Node 				node_;
		Node 				top_node_;
		gl::Shader 	shader_;
		Camera 			camera_;
		Node 				top_node_2_;

		float 			rotation_;
		
	};

	
}

#endif
