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


/*
 * Called when the mainloop starts, just once
 */

void ObjApp::Init(){
    shader_ = Shader( s9::File("./shaders/3/basic_mesh.vert"),  s9::File("./shaders/3/quad.frag"));

    camera_= Camera( glm::vec3(0,0,10.0f), glm::vec3(0,0,0.0f));
    node_.Add(shader_).Add(camera_);
    camera_.set_field(55.0f);

    obj_mesh_ = ObjMesh(s9::File("./data/suzanne.obj")); 
    node_.Add(obj_mesh_);

    rotation_ = 0.1f;
}

void ObjApp::Update(double_t dt) {

}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void ObjApp::Display(GLFWwindow* window, double_t dt){

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    rotation_ += 1.0f;
   
    glm::mat4 Model = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));
    node_.set_matrix(Model);

    node_.Draw();
   
    CXGLERROR
}


void ObjApp::ProcessEvent(CloseWindowEvent e, GLFWwindow* window) {

}

/*
 * This is called by the wrapper function when an event is fired
 */

void ObjApp::ProcessEvent(MouseEvent e, GLFWwindow* window){
}

/*
 * Called when the window is resized. You should set cameras here
 */

void ObjApp::ProcessEvent(ResizeEvent e, GLFWwindow* window){
    cout << "Window Resized:" << e.w << "," << e.h << endl;
    camera_.Resize(e.w,e.h);
}

void ObjApp::ProcessEvent(KeyboardEvent e, GLFWwindow* window){
    cout << "Key Pressed: " << e.key << endl;
}

/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {
  
    ///\todo better command line args parsing

    ObjApp b;

#ifdef _SEBURO_OSX
    GLFWApp a(b,3,2);
#else
    GLFWApp a(b);
#endif

    // Create our initial window
    a.CreateWindow("ObjApp", 800, 600);

    // Now kick off the OpenGL side of things
    a.Run();
}