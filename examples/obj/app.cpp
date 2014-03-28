/**
* @brief GLFW Window based solution
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#include "app.hpp"
#include <signal.h>

using namespace std;
using namespace s9;
using namespace s9::gl;


ObjApp::ObjApp() {

  window_manager_.AddListener(this);
  std::function<void()> init = std::bind(&ObjApp::Init, this);
  std::function<void(double_t)> draw = std::bind(&ObjApp::Draw, this, std::placeholders::_1);
  const GLWindow &windowZero = window_manager_.CreateWindow("OBJ Application",800,600, init, draw );
}



/*
 * Called when the mainloop starts, just once
 */

 void ObjApp::Init(){
  shader_ = Shader( s9::File("./shaders/3/basic_mesh.vert"),  s9::File("./shaders/3/textured_mesh.frag"));

  camera_= Camera( glm::vec3(0,0,10.0f), glm::vec3(0,0,0.0f));
  node_.Add(shader_).Add(camera_);
  camera_.set_field(55.0f);

  obj_mesh_ = ObjMesh(s9::File("./data/test.obj")); 
  node_.Add(obj_mesh_);

  rotation_ = 0.1f;

  glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
 
 void ObjApp::Draw(double_t dt){

  glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
  GLfloat depth = 1.0f;
  glClearBufferfv(GL_DEPTH, 0, &depth );

  rotation_ += 1.0f;
  
  glm::mat4 Model = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(1.0f, 0.0f, 0.0f));
  Model = glm::rotate(Model, rotation_, glm::vec3(0.0f, 1.0f, 0.0f));
  Model = glm::rotate(Model, rotation_, glm::vec3(0.0f, 0.0f, 1.0f));
    //Model = glm::scale(Model, glm::vec3(0.025f, 0.025f, 0.025f));
  node_.set_matrix(Model);

  node_.Draw();
  
  CXGLERROR
}



void ObjApp::MainLoop(double_t dt) {
  window_manager_.MainLoop();
}


void ObjApp::ProcessEvent( const GLWindow &window, CloseWindowEvent e) {
  
    // One window left and we are about to close it
  if (window_manager_.NumWindows() <= 1){
    Shutdown();
  }
}



/*
 * This is called by the wrapper function when an event is fired
 */

 void ObjApp::ProcessEvent( const GLWindow &window, MouseEvent e){
 }

/*
 * Called when the window is resized. You should set cameras here
 */

 void ObjApp::ProcessEvent( const GLWindow &window, ResizeEvent e){
  cout << "Window Resized:" << e.w << "," << e.h << endl;
  camera_.Resize(e.w, e.h);
}

void ObjApp::ProcessEvent( const GLWindow &window, KeyboardEvent e){
  cout << "Key Pressed: " << e.key << endl;
}



/*
 * Main function - uses boost to parse program arguments
 */

 int main (int argc, const char * argv[]) {
  
    ///\todo better command line args parsing
  ObjApp a;

    // Now kick off the OpenGL side of things
  a.Run();

  return EXIT_SUCCESS;
}