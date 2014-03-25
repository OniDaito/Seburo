/*
* @brief GLFW OpenNI Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef OPENNI_APP_HPP
#define OPENNI_APP_HPP

#include "s9/common.hpp"
#include "s9/camera.hpp"
#include "s9/file.hpp"
#include "s9/geometry.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/glfw.hpp"
#include "s9/openni/openni.hpp"
#include "s9/node.hpp"
#include "s9/gl/drawable.hpp"
#include "s9/gl/texture.hpp"
#include "s9/skeleton.hpp"
#include "s9/composite_shapes.hpp"

 
namespace s9 {

  /*
   * An application that shows how to deal with an FBO and draw to the screen
   */

  class OpenNIApp : public WindowApp<GLFWwindow*> {
  public:

    ~OpenNIApp();

    void Init(Context context);
    void Display(Context context, GLFWwindow* window, double_t dt);
    void Update(double_t dt);

    // Event handling - you can choose which to override
    void ProcessEvent(MouseEvent e, GLFWwindow* window);
    void ProcessEvent(KeyboardEvent e, GLFWwindow* window);
    void ProcessEvent(ResizeEvent e, GLFWwindow* window);

  protected:
  
    s9::oni::OpenNIBase openni_;
    s9::oni::OpenNISkeleton skeleton_;

    Skeleton skeleton_base_;

    Quad quad_;
    Image image_;
    
    gl::Shader shader_;
    gl::Shader shader_colour_;

    Node node_depth_;
    Node node_colour_;
    Node skeleton_node_;
    
    glm::mat4 model_;

    byte_t *bytes_;

    glm::vec4 bone_colour_, orient_colour_;
    SkeletonShape skeleton_shape_;
   
    gl::Texture texture_;

    Camera camera_;
    Camera ortho_camera_;    
  };
}

#endif