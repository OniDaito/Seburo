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

void ModelApp::init(){
 
#ifdef _SEBURO_LINUX
    mShader.load( s9::File("./shaders/4/basic_lighting.vert").path(),  s9::File("./shaders/4/basic_lighting.frag").path());
#elif _SEBURO_OSX
    mShader.load( s9::File("./shaders/1.5/basic_lighting.vert").path(),  s9::File("./shaders/1.5/basic_lighting.frag").path());
#endif

    mGeometry = gl::Asset<GeometryPNF>( AssetImporter::load( s9::File("./data/bunny.ply").path()));
    mGeometry.setScale(glm::vec3(30.0,30.0,30.0));
   
    mCamera.move(glm::vec3(0,0,20.0f));
    
    link(mCamera);
    link(*this);

    glEnable(GL_DEPTH_TEST);
}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void ModelApp::display(double_t dt){
    
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    mShader.bind();

    // Our matrix = the object * camera
    glm::mat4 mvp = mCamera.getMatrix() * mGeometry.getMatrix();
    glm::mat4 mv = mCamera.getViewMatrix() * mGeometry.getMatrix();
    glm::mat4 mn =  glm::transpose(glm::inverse(mv));

    mShader.s("uMVPMatrix",mvp).s("uShininess",128.0f).s("uMVMatrix",mv)
        .s("uNMatrix",mn).s("uLight0",glm::vec3(5.0,5.0,5.0));

    mGeometry.draw();
    
    mShader.unbind();

    mCamera.update(dt);

    CXGLERROR
}


/*
 * This is called by the wrapper function when an event is fired
 */

void ModelApp::processEvent(MouseEvent e){
}

/*
 * Called when the window is resized. You should set cameras here
 */

void ModelApp::processEvent(ResizeEvent e){
    cout << "Window Resized" << endl;
    glViewport(0,0,e.mW,e.mH);
    mCamera.setRatio( static_cast<float>(e.mW) / e.mH);
}

void ModelApp::processEvent(KeyboardEvent e){
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
    ("help", "Seburo Basic Application - No Options")
    ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }
#endif
  
    ModelApp b;

    GLFWApp a(b, 800, 600, false, argc, argv, "Bunny",2,1);

    return EXIT_SUCCESS;

}