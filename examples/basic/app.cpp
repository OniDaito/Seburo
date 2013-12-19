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

void BasicApp::init(){
    shader_ = Shader( s9::File("./shaders/3/quad.vert"),  s9::File("./shaders/3/quad.frag"));

    addWindowListener(this);

    cuboid_ = Cuboid(3.0,2.0,1.0);

    camera_= Camera( glm::vec3(0,0,10.0f));

    Spike s (4,1,1.0f,2.0f);

    node_.add(cuboid_).add(shader_).add(camera_);

    top_node_2_.add(s);
    top_node_2_.add(camera_).add(shader_);

    Node spike_node(s);

    spike_node.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,2.5f,0.0f)));

    node_.add(spike_node);

    top_node_.add(node_);
    top_node_.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f,0.0f,0.0f)));

    rotation_ = 0;
}

///\todo seems not to want to update member variables :(
void BasicApp::update(double_t dt) {

}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void BasicApp::display(double_t dt){

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    rotation_ += 1.0;
   
    glm::mat4 Model = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));
    node_.setMatrix(Model);

    top_node_.draw();

    top_node_2_.draw();
   
    CXGLERROR
}


/*
 * This is called by the wrapper function when an event is fired
 */

void BasicApp::processEvent(MouseEvent e){
}

/*
 * Called when the window is resized. You should set cameras here
 */

void BasicApp::processEvent(ResizeEvent e){
    cout << "Window Resized:" << e.w << "," << e.h << endl;
    glViewport(0,0,e.w,e.h);
    camera_.resize(e.w,e.h);
}

void BasicApp::processEvent(KeyboardEvent e){
    cout << "Key Pressed: " << e.key << endl;
}

/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {
  
    ///\todo better command line args parsing

    BasicApp b;

#ifdef _SEBURO_OSX
    GLFWApp a(b, 800, 600, false, argc, argv, "Basic",3,2);
#else
    GLFWApp a(b, 800, 600, false, argc, argv, "Basic");
#endif

    return EXIT_SUCCESS;

}