/**
* @brief GLFW Window based solution
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 29/10/2013
*
*/

#include "app.hpp"
#include <signal.h>

using namespace std;
using namespace s9;
using namespace s9::gl;


MD5App::MD5App() {

  window_manager_.AddListener(this);
  std::function<void()> init = std::bind(&MD5App::Init, this);
  std::function<void(double_t)> draw = std::bind(&MD5App::Draw, this, std::placeholders::_1);
  const GLWindow &windowZero = window_manager_.CreateWindow("MD5 Application",800,600, init, draw );
}

/*
 * Called when the mainloop starts, just once
 */

 void MD5App::Init(){
  shader_ = Shader(s9::File("./shaders/3/skinning.glsl"));
  shader_colour_ = Shader(s9::File("./shaders/3/solid_colour.glsl"));

  show_wireframe_ = false;

  quad_ = Quad(1.0,1.0);
  rotation_ = 0;

  bone_colour_ = glm::vec4(1.0,0.0,0.0,1.0);
  orient_colour_ = glm::vec4(0.0,0.0,1.0,1.0);

  camera_ = Camera( glm::vec3(0,0,2.0f));
  md5_ = MD5Model( s9::File("./data/sintel_lite/sintel_lite.md5mesh") ); 

  node_.Add(md5_).Add(camera_).Add(shader_);

  skeleton_shape_ = SkeletonShape(md5_.skeleton());
  skeleton_shape_.Add(shader_colour_).Add(camera_);
  
  node_.Add(skeleton_shape_).Add(sphere_node_);

  Sphere s(0.08f, 10);
  sphere_colour_ = glm::vec4(0.0f,1.0f,0.0f,1.0f);
  sphere_node_.Add(s).Add(shader_colour_).Add(gl::ShaderClause<glm::vec4,1>("uColour", sphere_colour_));


  node_full_.Add(md5_).Add(camera_).Add(shader_).Add(sphere_node_);

    /*Bone * neck = md5_.skeleton().bone("neck");
    neck->applyRotation ( glm::angleAxis( 20.0f, glm::vec3(0.0,1.0,0.0)) );

    Bone * waist = md5_.skeleton().bone("waist");
    waist->applyRotation( glm::angleAxis( -45.0f, glm::vec3(1.0,0.0,0.0)) );
    waist->applyRotation( glm::angleAxis( -25.0f, glm::vec3(0.0,1.0,0.0)) );
 
    Bone * luparm = md5_.skeleton().bone("luparm");
    luparm->applyRotation( glm::angleAxis( -45.0f, glm::vec3(0.0,1.0,0.0))  );
    //luparm->applyRotation( glm::angleAxis( 15.0f, glm::vec3(0.0,0.0,1.0))  );

    Bone * lloarm = md5_.skeleton().bone("lloarm");
    lloarm->applyRotation( glm::angleAxis( -45.0f, glm::vec3(0.0,0.0,1.0))  );
    //lloarm->applyRotation( glm::angleAxis( 15.0f, glm::vec3(0.0,0.0,1.0))  );
   */
    cout << node_ << endl;

    CXGLERROR


  }

///\todo seems not to want to update member variables :(
  void MD5App::ThreadMainLoop(double_t dt) {

    Bone * upper_arm_l = md5_.skeleton().GetBone("upper_arm.L");
    upper_arm_l->ApplyRotation( glm::angleAxis( -0.005f, glm::vec3(0.0,0.0,1.0)) );

    Bone * thigh_l = md5_.skeleton().GetBone("thigh.L");
    thigh_l->ApplyRotation( glm::angleAxis( 0.005f, glm::vec3(0.0,1.0,0.0))  );

    md5_.skeleton().Update();

    //Bone * lloarm = md5_.skeleton().bone("lower_arm.L");

    //glm::vec4 sp (0.58f, 0.0f, 1.35f, 1.0f);
    //sp = lloarm->skinned_matrix() * sp;

    //glm::mat4 tm = glm::translate(glm::mat4(1.0f), glm::vec3(sp.x, sp.y, sp.z));
    //sphere_node_.set_matrix(tm);
    
  }


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */

 void MD5App::Draw(double_t dt){

  glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
  GLfloat depth = 1.0f;
  glClearBufferfv(GL_DEPTH, 0, &depth );

  if (show_wireframe_){
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
  } else {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
  }



  rotation_ += 0.5;
  glm::mat4 Model = glm::rotate(glm::mat4(), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));
  Model = glm::translate(Model, glm::vec3(0.0,-0.8,0.0));
    //Model = glm::scale(Model, glm::vec3(0.01,0.01,0.01));
  Model = glm::rotate(Model, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  node_.set_matrix(Model);
  node_full_.set_matrix(Model);

  if (show_wireframe_)
    node_.Draw();
  else
    node_full_.Draw();

  CXGLERROR
}


void MD5App::MainLoop(double_t dt) {
  window_manager_.MainLoop();
}


void MD5App::ProcessEvent( const GLWindow &window, CloseWindowEvent e) {

    // One window left and we are about to close it
  if (window_manager_.NumWindows() <= 1){
    Shutdown();
  }
}



/*
 * This is called by the wrapper function when an event is fired
 */

 void MD5App::ProcessEvent( const GLWindow &window, MouseEvent e){
 }

/*
 * Called when the window is resized. You should set cameras here
 */

 void MD5App::ProcessEvent( const GLWindow &window, ResizeEvent e){
  cout << "Window Resized:" << e.w << "," << e.h << endl;
  camera_.Resize(e.w, e.h);
}

void MD5App::ProcessEvent( const GLWindow &window, KeyboardEvent e){
  if (e.key == 87) {
    if (e.action == GLFW_PRESS){

      show_wireframe_ = !show_wireframe_;
      if (show_wireframe_){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
    }

  }
}


/*
 * Main function - uses boost to parse program arguments
 */

 int main (int argc, const char * argv[]) {

  MD5App a;

  a.Run();

  return EXIT_SUCCESS;

}