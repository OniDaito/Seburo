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
#include "s9/gl/glfw_app.hpp"

#include "anttweakbar/AntTweakBar.h"

// Build a basic GTK+ Second window with its own thread
#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

#include <thread>



namespace s9 {

	#ifdef _SEBURO_LINUX

	class UXWindow : public Gtk::Window {

	public:
	  UXWindow();
	  virtual ~UXWindow();

	protected:
	  //Signal handlers:
	  void on_button_clicked();

	  //Member widgets:
	  Gtk::Button m_button;

	};

	#endif

	/*
 	 * An Basic App that draws a quad and provides a basic camera
 	 */

	class UxApp : public WindowApp, WindowResponder{
	public:
		void init();
		void display(double_t dt);
		void update(double_t dt);

		// Event handling - you can choose which to override
		void processEvent(MouseEvent e);
		void processEvent(KeyboardEvent e);
		void processEvent(ResizeEvent e);
		
	protected:
		Cuboid cuboid_;
		Node node_;
		Node top_node_;
		gl::Shader shader_;
		Camera camera_;
		Node top_node_2_;

		float rotation_;

		UXWindow window_;
		
	};

	
}

#endif
