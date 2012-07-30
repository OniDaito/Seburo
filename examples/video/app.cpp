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

void VideoApp::init(){
    mTestQuad = gl::Quad(640.0,320.0);
    mShader.load("../../../shaders/quad_texture.vert", "../../../shaders/quad_texture.frag");

  //  mTestQuad.move(glm::vec3(-0.5,-0.5,0.0));

#ifdef _GEAR_X11_GLX
    mVideo = VidCam("/dev/video0",640,320,30);
#endif
}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void VideoApp::display(double_t dt){
    
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    mShader.bind();

    // Our matrix = the object * camera
    glm::mat4 mvp = mCamera.getMatrix() * mTestQuad.getMatrix();

    mShader.s("uMVPMatrix",mvp);
    mVideo.bind();

    mTestQuad.draw();
    mVideo.unbind();

    mShader.unbind();
    mVideo.update();

    CXGLERROR
}


/*
 * This is called by the wrapper function when an event is fired
 */

void VideoApp::fireEvent(MouseEvent e){
}

/*
 * Called when the window is resized. You should set cameras here
 */

void VideoApp::fireEvent(ResizeEvent e){
    cout << "Window Resized:" << e.mW << "," << e.mH << endl;
    glViewport(0,0,e.mW,e.mH);
    mCamera.setRatio( static_cast<float_t>(e.mW) / e.mH);
}

void VideoApp::fireEvent(KeyboardEvent e){
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
  
    VideoApp b;

  	GLFWApp a(&b);
  	a.init(4,0); 

    return EXIT_SUCCESS;

}