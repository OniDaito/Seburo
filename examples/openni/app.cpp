#/**
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
using namespace s9::oni;


/*
 * Called when the mainloop starts, just once
 */

void OpenNIApp::Init(){

    shader_ = Shader( s9::File("./shaders/3/quad_texture.vert"), s9::File("./shaders/3/quad_texture.frag"));
    shader_colour_ = Shader(s9::File("./shaders/3/solid_colour.glsl"));

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
    node_depth_.Add(quad_).Add(shader_).Add(ortho_camera_);
    node_colour_.Add(quad_).Add(shader_).Add(ortho_camera_);


    skeleton_base_ = Skeleton(OPENNI_SKELETON);
    skeleton_shape_ = SkeletonShape(skeleton_base_);

    skeleton_base_.Update();

    skeleton_node_.Add(shader_colour_).Add(camera_).Add(skeleton_shape_);


}

/*
 * Update loop on another thread
 */

  void OpenNIApp::Update(double_t dt) {
	
  }

  OpenNIApp::~OpenNIApp(){
  }


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */

 void OpenNIApp::Display(GLFWwindow* window, double_t dt){

    // Moved here as GC Seems to be upset

    if (openni_.ready()) {
        skeleton_.Update();
        OpenNISkeleton::User user = skeleton_.GetUserByID(1);
        if (user.IsTracked()){
            
            skeleton_base_.CopyBoneRotations(user.skeleton());
            
            cout << skeleton_base_ << endl;
        } 
    }
    skeleton_base_.Update();



    GLfloat depth = 1.0f;
    // Now draw to the screen

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.5f, 0.6f, 0.6f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );

    openni_.Update(); // While thread safe, its best to put this immediately before the update_textures
    openni_.UpdateTextures();

    ortho_camera_.Update(dt);
    camera_.Update(dt);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (openni_.depth_ready()) {
        model_ = glm::translate(glm::mat4(1.0f), glm::vec3(160,120,0));
        node_depth_.set_matrix(model_);
        openni_.texture_depth().Bind();
        node_depth_.Draw();
        openni_.texture_depth().Unbind();
    }

    if (openni_.colour_ready()) {
        model_ = glm::translate(glm::mat4(1.0f), glm::vec3(480,120,0));
        node_colour_.set_matrix(model_);
        openni_.texture_colour().Bind();
        node_colour_.Draw();
        openni_.texture_colour().Unbind();
    }

    //glm::mat4 skel_mat = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f,0.1f,0.1f));
    //skeleton_node_.set_matrix(skel_mat);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    skeleton_node_.Draw();

    CXGLERROR
}


/*
 * This is called by the wrapper function when an event is fired
 */

 void OpenNIApp::ProcessEvent(MouseEvent e, GLFWwindow* window){
 }

/*
 * Called when the window is resized. You should set cameras here
 * and reset the viewport
 */

 void OpenNIApp::ProcessEvent(ResizeEvent e, GLFWwindow* window){
    ortho_camera_.Resize(e.w, e.h);
    camera_.Resize(e.w, e.h);
}

void OpenNIApp::ProcessEvent(KeyboardEvent e, GLFWwindow* window){
    cout << "Key Pressed: " << e.key << endl;
}

/*
 * Main function - uses boost to parse program arguments
 */

 int main (int argc, const char * argv[]) {

    OpenNIApp b;

#ifdef _SEBURO_OSX
    GLFWApp a(b,3,2);
#else
    GLFWApp a(b);
#endif

    a.Run();

    return EXIT_SUCCESS;

}
