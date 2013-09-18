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
    _test_quad = gl::Quad(640.0,480.0);
    _shader.load("../../../shaders/3/quad_texture.vert", "../../../shaders/3/quad_texture.frag");

#ifdef _SEBURO_LINUX
    _video = VidCam("/dev/video0",640, 480,30);
#endif

    link(_camera);
    link(*this);

    glEnable(GL_TEXTURE_RECTANGLE);
}

/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void VideoApp::display(double_t dt){
    
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    _shader.bind();

    // Our matrix = the object * camera
    glm::mat4 mvp = _camera.getMatrix() * _test_quad.getMatrix();

    _shader.s("uMVPMatrix",mvp);
    _shader.s("uBaseTex",0);
    _video.bind();

    _test_quad.draw();
    _video.unbind();

    _shader.unbind();
    _video.update();

    CXGLERROR
}


/*
 * This is called by the wrapper function when an event is fired
 */

void VideoApp::processEvent(MouseEvent e){
}

/*
 * Called when the window is resized. You should set cameras here
 */

void VideoApp::processEvent(ResizeEvent e){
    cout << "Window Resized:" << e.mW << "," << e.mH << endl;
    glViewport(0,0,e.mW,e.mH);
}

void VideoApp::processEvent(KeyboardEvent e){
    cout << "Key Pressed: " << e.mKey << endl;
}

/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {
  
#ifdef _SEBURO_LINUX
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "Seburo Video Application - No Options")
    ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }
#endif

    
    VideoApp b;
    
#ifdef _SEBURO_OSX
    GLFWApp a(b, 800, 600, false, argc, argv, "Video",3,2);
#else
    GLFWApp a(b, 800, 600, false, argc, argv, "Video");
#endif

    return EXIT_SUCCESS;

}