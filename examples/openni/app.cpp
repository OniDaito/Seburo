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

    shader_ = Shader( s9::File("./shaders/3/quad_texture.vert"), s9::File("./shaders/3/quad_texture.frag"));
    shader_colour_ = Shader(s9::File("./shaders/3/solid_colour.glsl"));

    addWindowListener(this);

    CXGLERROR

    openni_ = OpenNIBase(openni::ANY_DEVICE);
    skeleton_ = OpenNISkeleton(openni_);

    bone_colour_ = glm::vec4(1.0,0.0,0.0,1.0);
    orient_colour_ = glm::vec4(0.0,0.0,1.0,1.0);

    ortho_camera_ = Camera(glm::vec3(0.0f,0.0f,0.1f));
    ortho_camera_.set_near(0.01f);
    ortho_camera_.set_far(1.0f);
    ortho_camera_.set_orthographic(true);

    camera_ = Camera(glm::vec3(0.0f,0.0f,10.0f));
    camera_.set_orthographic(false);

    quad_ = Quad(320,240);
    node_depth_.add(quad_).add(shader_).add(ortho_camera_);
    node_colour_.add(quad_).add(shader_).add(ortho_camera_);


    skeleton_base_ = Skeleton(OPENNI_SKELETON);
    skeleton_shape_ = SkeletonShape(skeleton_base_);
    skeleton_shape_.set_geometry_cast(WIREFRAME);

    skeleton_base_.update();

    skeleton_node_.add(shader_colour_).add(camera_).add(skeleton_shape_);

}

/*
 * Update loop on another thread
 */

 void OpenNIApp::update(double_t dt) {

    if (openni_.ready()) {
        skeleton_.update();
        OpenNISkeleton::User user = skeleton_.user(1);
        if (user.isTracked()){
            
            skeleton_base_.copyBoneRotations(user.skeleton());
            
            cout << skeleton_base_ << endl;
        } 
    }
    skeleton_base_.update();
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

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.5f, 0.6f, 0.6f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );

    openni_.update(); // While thread safe, its best to put this immediately before the update_textures
    openni_.update_textures();

    ortho_camera_.update(dt);
    camera_.update(dt);


    model_ = glm::translate(glm::mat4(1.0f), glm::vec3(160,120,0));
    node_depth_.setMatrix(model_);
    openni_.texture_depth().bind();
    node_depth_.draw();
    openni_.texture_depth().unbind();

    model_ = glm::translate(glm::mat4(1.0f), glm::vec3(480,120,0));
    node_colour_.setMatrix(model_);
    openni_.texture_colour().bind();
    node_colour_.draw();
    openni_.texture_colour().unbind();

    //glm::mat4 skel_mat = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f,0.1f,0.1f));
    //skeleton_node_.setMatrix(skel_mat);
    skeleton_node_.draw();

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
    camera_.resize(e.w, e.h);


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