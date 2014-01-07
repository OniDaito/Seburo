/*
* @brief GLFW FBO Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/12/2013
*
*/

#ifndef FBO_APP_HPP
#define FBO_APP_HPP

#include "s9/common.hpp"
#include "s9/file.hpp"
#include "s9/camera.hpp"
#include "s9/window.hpp"
#include "s9/shapes.hpp"
#include "s9/node.hpp"
#include "s9/gl/fbo.hpp"
#include "s9/gl/drawable.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/glfw.hpp"


 
namespace s9 {

	/*
 	 * An Basic App that draws a quad and provides a basic camera
 	 */

	class FBOApp : public WindowApp<GLFWwindow*> {
	public:
		void Init();
		void Display(GLFWwindow* window, double_t dt);
		void Update(double_t dt);

		// Event handling - you can choose which to override
		void ProcessEvent(MouseEvent e, GLFWwindow* window);
		void ProcessEvent(KeyboardEvent e, GLFWwindow* window);
		void ProcessEvent(ResizeEvent e, GLFWwindow* window);
		
	protected:
		Cuboid 			cuboid_;
		Node				node_quad_;
		Node 				node_;
		Quad				quad_;
		gl::Shader 	shader_;
		gl::Shader 	shader_warp_;
		Camera 			camera_;
		Camera 			camera_ortho_;
		gl::FBO 		fbo_;

		float rotation_;
		
	};
}

#endif
