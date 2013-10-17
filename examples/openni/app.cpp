/**
* @brief GLFW Window based solution
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#include "app.hpp"


#include <boost/program_options.hpp>
#include <signal.h>

using namespace std;
using namespace s9;
using namespace s9::gl;
using namespace s9::oni;


/*
 * Called when the mainloop starts, just once
 */

void OpenNIApp::init(){
   

    //mCamera.move(glm::vec3(0,0,10.0f));

    link(*this);

    CXGLERROR

    openni_ = OpenNISkeleton(openni::ANY_DEVICE);
}

/*
 * Update loop on another thread
 */

void OpenNIApp::update(double_t dt) {
    openni_.update();
}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
    
void OpenNIApp::display(double_t dt){

    GLfloat depth = 1.0f;
    // Our matrix = the object * camera

    // Now draw to the screen

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );
    
    
    CXGLERROR
}


/*
 * This is called by the wrapper function when an event is fired
 */

void OpenNIApp::processEvent(MouseEvent e){
}

/*
 * Called when the window is resized. You should set cameras here
 * and reset the viewport
 */

void OpenNIApp::processEvent(ResizeEvent e){
    glViewport(0, 0, e.w, e.h);
}

void OpenNIApp::processEvent(KeyboardEvent e){
    cout << "Key Pressed: " << e.key << endl;
}

/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {
    
    OpenNIApp b;

#ifdef _SEBURO_OSX
    GLFWApp a(b, 800, 600, false, argc, argv, "OpenNI",3,2);
#else
    GLFWApp a(b, 800, 600, false, argc, argv, "OpenNI");
#endif

    return EXIT_SUCCESS;

}