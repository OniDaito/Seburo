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
using namespace boost;
using namespace boost::assign;
using namespace s9;
using namespace s9::gl;

namespace po = boost::program_options;


/*
 * Called when the mainloop starts, just once
 */

void BasicApp::init(){
    mTestQuad = gl::Quad(1.0,1.0);
    mShader.load("../../../shaders/quad.vert", "../../../shaders/quad.frag");

    mTestQuad.move(glm::vec3(-0.5,-0.5,0.0));
    mCamera.move(glm::vec3(0,0,20.0f));
    mPrevT = 0;

}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void BasicApp::display(double_t dt){
    
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    mShader.bind();

    // Our matrix = the object * camera
    glm::mat4 mvp = mCamera.getMatrix() * mTestQuad.getMatrix();

    mShader.s("uMVPMatrix",mvp);

    mTestQuad.draw();
    mShader.unbind();

    mCamera.update(dt);

    CXGLERROR
}


/*
 * This is called by the wrapper function when an event is fired
 */

void BasicApp::fireEvent(MouseEvent &e){
    mCamera.passEvent(e);
}

/*
 * Called when the window is resized. You should set cameras here
 */

void BasicApp::fireEvent(ResizeEvent &e){
    cout << "Window Resized" << endl;
    mCamera.setRatio( static_cast<float_t>(e.mW) / e.mH);
}

void BasicApp::fireEvent(KeyboardEvent &e){
    cout << "Key Pressed: " << e.mKey << endl;
}

/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {
  
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "S9Gear Basic Application - No Options")
    ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }
  
    BasicApp b;

  	GLFWApp a(&b);
  	a.init(4,0); 

    return EXIT_SUCCESS;

}