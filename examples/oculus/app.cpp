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
    shader_warp_ = Shader( s9::File("./shaders/glsl/oculus_warp.glsl"));

    addWindowListener(this);

    cuboid_ = Cuboid(3.0,2.0,1.0);
    Cuboid v = cuboid_;
    quad_ = Quad(640,480);

    camera_ = Camera(glm::vec3(0.0f,0.0f,10.0f));
    camera_ortho_ = Camera(glm::vec3(0.0f,0.0f,1.0f));
    camera_ortho_.set_orthographic(true);

    oculus_ = oculus::OculusBase(true);

    fbo_ = FBO(800,600);
    
    node_.add(cuboid_).add(shader_).add(camera_);
    node_quad_.add(quad_).add(shader_warp_).add(camera_ortho_).add(fbo_.colour());



}

///\todo seems not to want to update member variables :(
void OculusApp::update(double_t dt) {    
    oculus_.update(dt);


}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */

 void OculusApp::display(double_t dt){
    GLfloat depth = 1.0f;

    // Draw to the FBO
    fbo_.bind();
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );

    glm::quat q = oculus_.orientation();
    glm::mat4 mq = glm::mat4_cast(q);


    oculus_dt_ = glm::inverse(oculus_prev_) * q;
    camera_.rotate(oculus_dt_);
    camera_.update(dt);
    oculus_prev_ = q;

    node_.draw();

    fbo_.unbind();

    // Draw to main screen
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );

    node_quad_.draw();
     
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
    camera_ortho_.resize(e.w, e.h);
    fbo_.resize(e.w, e.h);

    node_quad_.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(e.w/2.0f, e.h/2.0f, 0.0f)));
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