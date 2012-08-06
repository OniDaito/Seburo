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

void PickingApp::init(){
    mTestQuad = gl::Quad(1.0,1.0);
    mShader.load("../../../shaders/quad.vert", "../../../shaders/quad.frag");
    mPickingShader.load("../../../shaders/picker.vert", "../../../shaders/picker.frag");

    mPickingFBO = gl::FBO(800,600);
    
    mTestQuad.move(glm::vec3(-0.5,-0.5,0.0));
    mCamera.setRatio(800.0 / 600.0);
    mCamera.move(glm::vec3(0,0,10.0f));

    glEnable(GL_TEXTURE_RECTANGLE);

}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void PickingApp::display(double_t dt){
    

    GLfloat depth = 1.0f;
    // Our matrix = the object * camera
    glm::mat4 mvp = mCamera.getMatrix() * mTestQuad.getMatrix();

    // Bind to an FBO
    mPickingFBO.bind();
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );

    mPickingShader.bind();
    mPickingShader.s("uMVPMatrix",mvp);

    mPickingShader.s("uColour",mTestQuad.getColour());
    mTestQuad.draw();
    mPickingShader.unbind();

    mPickingFBO.unbind();

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

void PickingApp::fireEvent(MouseEvent e){
    mCamera.passEvent(e);


   glm::vec4 c;
   if (mPickingFBO && e.mFlag & MOUSE_LEFT_DOWN) {
        mPickingFBO.bind();
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        CXGLERROR

        GLfloat *data = (GLfloat*) new GLfloat[4];
        glReadPixels(e.mX, e.mY, 1, 1, GL_RGBA, GL_FLOAT, data);
        CXGLERROR
        c = glm::vec4 ( data[0], data[1], data[2], data[3]);

        if (c == mTestQuad.getColour())
            cout << "Selected the quad" << endl;

        CXGLERROR
        mPickingFBO.unbind();
    }
}

/*
 * Called when the window is resized. You should set cameras here
 * and reset the viewport
 */

void PickingApp::fireEvent(ResizeEvent e){
    glViewport(0,0,e.mW,e.mH);
    mCamera.setRatio( static_cast<float_t>(e.mW) /  static_cast<float_t>(e.mH));
    
    if (mPickingFBO) {
        mPickingFBO.resize(e.mW,e.mH);
    }
}

void PickingApp::fireEvent(KeyboardEvent e){
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
  
    PickingApp b;

  	GLFWApp a(&b,argc,argv,"Picking App");
  	a.init(4,0); 

    return EXIT_SUCCESS;

}