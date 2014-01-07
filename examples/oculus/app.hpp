/*
* @brief GLFW Oculus Rift Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 17/10/2013
*
*/

#ifndef OCULUS_APP_HPP
#define OCULUS_APP_HPP

#include "s9/common.hpp"
#include "s9/file.hpp"
#include "s9/camera.hpp"
#include "s9/shapes.hpp"
#include "s9/node.hpp"
#include "s9/gl/drawable.hpp"
#include "s9/gl/fbo.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/glfw.hpp"

#include "s9/oculus/oculus.hpp"

 
namespace s9 {

	/*
 	 * An Basic App that draws a quad and provides a basic camera
 	 */

	class OculusApp : public WindowApp<GLFWwindow*>{
	public:
		void Init();
		void Display(GLFWwindow* window, double_t dt);
		void Update(double_t dt);

		// Event handling - you can choose which to override
		void ProcessEvent(MouseEvent e, GLFWwindow* window);
		void ProcessEvent(KeyboardEvent e, GLFWwindow* window);
		void ProcessEvent(ResizeEvent e, GLFWwindow* window);
		
	protected:
		Cuboid 				cuboid_;
		Quad					quad_;

		gl::Shader 		shader_;
		gl::Shader 		shader_warp_;
		
		Camera 				camera_main_;
		Camera 				camera_left_;
		Camera 				camera_right_;
		Camera 				camera_ortho_;
		

		Node 					node_;
		Node 					node_main_;
		Node 					node_left_;
		Node 					node_right_;
		Node					node_quad_;


		gl::FBO				fbo_;

		GLuint				null_VAO_;

		oculus::OculusBase oculus_;

		glm::quat oculus_dt_;
		glm::quat oculus_prev_;


		
	};
}

#endif
