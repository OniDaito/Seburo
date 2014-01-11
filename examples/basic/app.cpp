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

void BasicApp::Init(){
    shader_ = Shader( s9::File("./shaders/3/quad.vert"),  s9::File("./shaders/3/quad.frag"));

    camera_= Camera( glm::vec3(0,0,0.0f), glm::vec3(0,0,-1.0f));
    top_node_.add(shader_).add(camera_);
    camera_.set_field(55.0f);

    cuboid_ = Cuboid(0.1f,0.1f,0.1f);
    Node cuboid_node(cuboid_);
    cuboid_node.set_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,-1.0f)));
    top_node_.add(cuboid_node);

  

    Spike s (4,1,0.1f,0.2f);
    Node spike_node(s);
    spike_node.set_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f,0.0f,0.0f)));
    top_node_.add(spike_node);

    Node spike_node2(s);
    spike_node2.set_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f,0.0f,0.0f)));
    top_node_.add(spike_node2);

    rotation_ = 0.1f;
}

void BasicApp::Update(double_t dt) {

}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void BasicApp::Display(GLFWwindow* window, double_t dt){

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    //rotation_ += 0.1f;
   
    glm::mat4 Model = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));
    
    glm::vec4 look = Model * glm::vec4( camera_.look().x,  camera_.look().y,  camera_.look().z, 1.0f);
    camera_.set_look(glm::vec3( look.x,look.y,look.z ));


    //node_.set_matrix(Model);

    top_node_.draw();
   
    CXGLERROR
}


void BasicApp::ProcessEvent(CloseWindowEvent e, GLFWwindow* window) {

}

/*
 * This is called by the wrapper function when an event is fired
 */

void BasicApp::ProcessEvent(MouseEvent e, GLFWwindow* window){
}

/*
 * Called when the window is resized. You should set cameras here
 */

void BasicApp::ProcessEvent(ResizeEvent e, GLFWwindow* window){
    cout << "Window Resized:" << e.w << "," << e.h << endl;
    glViewport(0,0,e.w,e.h);
    camera_.resize(e.w,e.h);
}

void BasicApp::ProcessEvent(KeyboardEvent e, GLFWwindow* window){
    cout << "Key Pressed: " << e.key << endl;
}

/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {
  
    ///\todo better command line args parsing

    BasicApp b;

#ifdef _SEBURO_OSX
    GLFWApp a(b,3,2);
#else
    GLFWApp a(b);
#endif

    // Create our initial window
    a.CreateWindow("Basic", 800, 600);

    // Now kick off the OpenGL side of things
    a.Run();
}