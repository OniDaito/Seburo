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

namespace po = boost::program_options;


/*
 * Called when the mainloop starts, just once
 */

void FBOApp::init(){
    mTestQuad = gl::Quad(1.0,1.0);

#ifdef _SEBURO_OSX
    mShader.load(s9::File("./shaders/3/quad.vert").path(), s9::File("./shaders/3/quad.frag").path());
    mFBOShader.load(s9::File("./shaders/3/quad_texture.vert").path(), s9::File("./shaders/3/quad_texture.frag").path());
    CXGLERROR
#else
    mShader.load(s9::File("./shaders/4/quad.vert").path(), s9::File("./shaders/4/quad.frag").path());
    mFBOShader.load(s9::File("./shaders/4/quad_texture.vert").path(), s9::File("./shaders/4/quad_texture.frag").path());
#endif

    mFBO = gl::FBO(640,480);
    mHudQuad = gl::Quad(640.0,480.0);
    mHudQuad.setScale(glm::vec3(0.5,0.5,0.5));

    mTestQuad.move(glm::vec3(-0.5,-0.5,0.0));
    mCamera.move(glm::vec3(0,0,10.0f));

    link(mCamera);
    link(*this);
    link(mScreenCamera);

    CXGLERROR
}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void FBOApp::display(double_t dt){
    
    GLfloat depth = 1.0f;
    // Our matrix = the object * camera
    glm::mat4 mvp = mCamera.getMatrix() * mTestQuad.getMatrix();

    // Bind to an FBO
    mFBO.bind();
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );

    mShader.bind();
    mShader.s("uMVPMatrix",mvp);

    mTestQuad.draw();
    mShader.unbind();

    mFBO.unbind();

    // Now draw to the screen

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );
    

    mShader.bind();
    mShader.s("uMVPMatrix",mvp);
    mTestQuad.draw();
    mShader.unbind();

    mCamera.update(dt);

    // Now switch to the HUD and draw a quad aligned to the FBO Size
    mFBOShader.bind();
    mvp = mScreenCamera.getMatrix() * mHudQuad.getMatrix();
    mFBOShader.s("uMVPMatrix",mvp);

    mFBO.bindColour();

    mHudQuad.draw();

    mFBO.unbindColour();
    mFBOShader.unbind();


    CXGLERROR
}


/*
 * This is called by the wrapper function when an event is fired
 */

void FBOApp::processEvent(MouseEvent e){
}

/*
 * Called when the window is resized. You should set cameras here
 * and reset the viewport
 */

void FBOApp::processEvent(ResizeEvent e){
    glViewport(0,0,e.mW,e.mH);
}

void FBOApp::processEvent(KeyboardEvent e){
    cout << "Key Pressed: " << e.mKey << endl;
}

/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {
  
    FBOApp b;

#ifdef _SEBURO_OSX
    GLFWApp a(b, 800, 600, false, argc, argv, "FBO",3,2);
#else
    GLFWApp a(b, 800, 600, false, argc, argv, "FBO");
#endif

    return EXIT_SUCCESS;

}