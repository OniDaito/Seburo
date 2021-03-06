/*
* @brief GLFW MD5 Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 29/10/2013
*
*/

#ifndef MD5_APP_HPP
#define MD5_APP_HPP

#include "s9/application.hpp"
#include "s9/common.hpp"
#include "s9/file.hpp"
#include "s9/camera.hpp"
#include "s9/shapes.hpp"
#include "s9/node.hpp"
#include "s9/gl/drawable.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/glfw.hpp"
#include "s9/image.hpp"
#include "s9/gl/texture.hpp"
#include "s9/md5.hpp"
#include "s9/composite_shapes.hpp"

 
namespace s9 {

	/*
 	 * Example of texturing a basic Quad
 	 */

	class MD5App : public Application, public WindowListener<gl::GLWindow> {
	public:

		MD5App();

		void Init();
		void Draw(double_t dt);
		void MainLoop(double_t dt);

		 void ThreadMainLoop(double_t dt);

		// Event handling - you can choose which to override
		void ProcessEvent(const gl::GLWindow & window, MouseEvent e);
		void ProcessEvent(const gl::GLWindow & window, KeyboardEvent e);
		void ProcessEvent(const gl::GLWindow & window, ResizeEvent e);
		void ProcessEvent(const gl::GLWindow & window, CloseWindowEvent e);

		
	protected:

		gl::GLFWWindowManager window_manager_;

		Quad quad_;
		Image image_;
		gl::Texture texture_;
		gl::Shader shader_;
		gl::Shader shader_colour_;
		

		Camera camera_;
		Node node_;
		Node node2_;
		Node node_full_;
		Node cylinder_node_;
		Node sphere_node_;
		MD5Model md5_;
		glm::vec4 bone_colour_, orient_colour_;
		SkeletonShape skeleton_shape_;

		bool show_wireframe_;

		glm::vec4 sphere_colour_;
		float rotation_;
		
	};
}

#endif
