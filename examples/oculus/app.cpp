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

 void OculusApp::init(){
    shader_ = Shader( s9::File("./shaders/3/quad.vert"),  s9::File("./shaders/3/quad.frag"));

   // camera_.set_pos(glm::vec3(0,0,20.0f));

    addWindowListener(this);

    cuboid_ = Cuboid(3.0,2.0,1.0);
    Cuboid v = cuboid_;

    Node n;
    v.brew();

    rotation_ = 0;
    oculus_ = oculus::OculusBase(true);

}

///\todo seems not to want to update member variables :(
void OculusApp::update(double_t dt) {    
    oculus_.update(dt);

}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */

 void OculusApp::display(double_t dt){

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    rotation_ += 1.0;
    camera_.update(dt);
    shader_.bind();

    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f));
    glm::mat4 Model = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::quat q = oculus_.orientation();

    glm::mat4 mq = glm::mat4_cast(q);
    View = mq * View;

    glm::mat4 MVP = Projection * View * Model;

    shader_.s("uMVPMatrix",MVP);

    cuboid_.draw();
    shader_.unbind();
     
}


/*
 * This is called by the wrapper function when an event is fired
 */

 void OculusApp::processEvent(MouseEvent e){
 }

/*
 * Called when the window is resized. You should set cameras here
 */

 void OculusApp::processEvent(ResizeEvent e){
    cout << "Window Resized:" << e.w << "," << e.h << endl;
    glViewport(0,0,e.w,e.h);
    camera_.resize(e.w,e.h);
}

void OculusApp::processEvent(KeyboardEvent e){
    cout << "Key Pressed: " << e.key << endl;
}

/*
 * Main function - uses boost to parse program arguments
 */

 int main (int argc, const char * argv[]) {

    OculusApp b;

#ifdef _SEBURO_OSX
    GLFWApp a(b, 800, 600, false, argc, argv, "Oculus",3,2);
#else
    GLFWApp a(b, 800, 600, false, argc, argv, "Oculus");
#endif

    return EXIT_SUCCESS;

}