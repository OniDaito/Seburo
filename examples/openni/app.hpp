/*
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef OPENNIAPP_HPP
#define OPENNIAPP_HPP

#include "s9/common.hpp"
#include "s9/camera.hpp"
#include "s9/file.hpp"
#include "s9/geometry.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/glfw_app.hpp"
#include "s9/openni/openni.hpp"
#include "s9/node.hpp"
#include "s9/gl/drawable.hpp"
#include "s9/gl/texture.hpp"

#include <anttweakbar/AntTweakBar.h>

 
namespace s9 {

  /*
   * An application that shows how to deal with an FBO and draw to the screen
   */

  class OpenNIApp : public WindowApp, WindowResponder{
  public:

    ~OpenNIApp();

    void init();
    void display(double_t dt);
    void update(double_t dt);

    // Event handling - you can choose which to override
    void processEvent(MouseEvent e);
    void processEvent(KeyboardEvent e);
    void processEvent(ResizeEvent e);


  protected:
  
    s9::oni::OpenNIBase openni_;
    s9::oni::OpenNISkeleton skeleton_;

    Quad quad_;
    Image image_;
    gl::Shader shader_;
    Node node_depth_;
    Node node_colour_;
    glm::mat4 model_;

    byte_t *bytes_;

    gl::Texture texture_;

    Camera camera_;
    Camera ortho_camera_;    
  };
}

#endif