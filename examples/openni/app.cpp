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
    mTestQuad = gl::Quad(1.0,1.0);

#ifdef _SEBURO_OSX
    mShader.load(s9::File("./shaders/3/quad.vert").path(), s9::File("./shaders/3/quad.frag").path());
    mFBOShader.load(s9::File("./shaders/3/quad_texture.vert").path(), s9::File("./shaders/3/quad_texture.frag").path());
    CXGLERROR
#else
    mShader.load(s9::File("./shaders/4/quad.vert").path(), s9::File("./shaders/4/quad.frag").path());
    mFBOShader.load(s9::File("./shaders/4/quad_texture.vert").path(), s9::File("./shaders/4/quad_texture.frag").path());
#endif

    mHudQuad = gl::Quad(640.0,480.0);
    mHudQuad.setScale(glm::vec3(0.5,0.5,0.5));

    mTestQuad.move(glm::vec3(-0.5,-0.5,0.0));
    mCamera.move(glm::vec3(0,0,10.0f));

    link(mCamera);
    link(*this);
    link(mScreenCamera);

    CXGLERROR

    mOpenNI = OpenNIBase(openni::ANY_DEVICE);
}

/*
 * Update loop on another thread
 */

void OpenNIApp::update(double_t dt) {
    
}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
    
void OpenNIApp::display(double_t dt){

    mOpenNI.update();

    GLfloat depth = 1.0f;
    // Our matrix = the object * camera
    glm::mat4 mvp = mCamera.getMatrix() * mTestQuad.getMatrix();

    // Now draw to the screen

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );
    
    mShader.bind();
    mShader.s("uMVPMatrix",mvp);
    mTestQuad.draw();
    mShader.unbind();

    mCamera.update(dt);
    
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
    glViewport(0,0,e.mW,e.mH);
}

void OpenNIApp::processEvent(KeyboardEvent e){
    cout << "Key Pressed: " << e.mKey << endl;
}

/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {
    
    OpenNIApp b;

#ifdef _SEBURO_OSX
    GLFWApp a(b, 800, 600, false, argc, argv, "FBO",3,2);
#else
    GLFWApp a(b, 800, 600, false, argc, argv, "FBO");
#endif

    return EXIT_SUCCESS;

}