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
   

    shader_.load( s9::File("./shaders/3/quad_texture.vert").path(), s9::File("./shaders/3/quad_texture.frag").path());

    addWindowListener(this);

    CXGLERROR

    openni_ = OpenNIBase(openni::ANY_DEVICE);
    skeleton_ = OpenNISkeleton(openni_);

    ortho_camera_.set_orthographic(true);

    quad_ = Quad(320,240);
    node_depth_.add(quad_);
    node_colour_.add(quad_);

    bytes_ = new byte_t[320 * 240 * 4];
    memset(bytes_, 255, 320 * 240 * 4);

    texture_ = Texture(320, 240, RGBA, UNSIGNED_BYTE, bytes_);

}

/*
 * Update loop on another thread
 */

void OpenNIApp::update(double_t dt) {
    
    skeleton_.update();
}

OpenNIApp::~OpenNIApp(){
    delete[] bytes_;
}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
    
void OpenNIApp::display(double_t dt){

    GLfloat depth = 1.0f;
    // Now draw to the screen

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );

    openni_.update(); // While thread safe, its best to put this immediately before the update_textures
    openni_.update_textures();
   
    ortho_camera_.update(dt);

    shader_.bind();

    //texture_.bind();
    model_ = glm::translate(glm::mat4(1.0f), glm::vec3(160,120,0));
    glm::mat4 MVP =  ortho_camera_.projection_matrix() * ortho_camera_.view_matrix() * model_;
    shader_.s("uMVPMatrix",MVP);
    openni_.texture_depth().bind();
    node_depth_.draw();
    openni_.texture_depth().unbind();

    model_ = glm::translate(glm::mat4(1.0f), glm::vec3(480,120,0));
    MVP =  ortho_camera_.projection_matrix() * ortho_camera_.view_matrix() * model_;
    shader_.s("uMVPMatrix",MVP);
    openni_.texture_colour().bind();
    node_colour_.draw();
    //texture_.unbind();
    openni_.texture_colour().unbind();
    shader_.unbind();


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
    ortho_camera_.resize(e.w, e.h);
   

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