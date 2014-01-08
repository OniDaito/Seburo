/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef BASICAPP_HPP
#define BASICAPP_HPP

#include "s9/common.hpp"
#include "s9/file.hpp"
#include "s9/camera.hpp"
#include "s9/shapes.hpp"
#include "s9/node.hpp"
#include "s9/gl/drawable.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/glfw.hpp"

 
namespace s9 {

	/*
 	 * An Basic App that draws a quad and provides a basic camera
 	 * A little multi-inheritence but one is only an interface so its cool
 	 */

	class BasicApp : public WindowApp<GLFWwindow*> {
	public:
		void Init();
		void Display(GLFWwindow*  window, double_t dt);
		void Update(double_t dt);
		

		// Event handling - you can choose which to override
		void ProcessEvent(MouseEvent e, GLFWwindow* window);
		void ProcessEvent(KeyboardEvent e, GLFWwindow* window);
		void ProcessEvent(ResizeEvent e, GLFWwindow* window );
		void ProcessEvent(CloseWindowEvent e, GLFWwindow* window );
		
	protected:
		Cuboid cuboid_;
		Node node_;
		Node top_node_;
		gl::Shader shader_;
		Camera camera_;
		Node top_node_2_;

		float rotation_;
		
	};
}

#endif
