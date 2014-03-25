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


/*
 * Called when the mainloop starts, just once
 */

void FBOApp::Init(Context context){
    shader_ = Shader( s9::File("./shaders/3/quad.vert"), s9::File("./shaders/3/quad.frag"));
    shader_warp_ = Shader( s9::File("./shaders/3/quad_texture.vert"), s9::File("./shaders/3/quad_texture.frag"));

    cuboid_ = Cuboid(3.0,2.0,1.0);
    camera_= Camera( glm::vec3(0,0,10.0f));
    camera_ortho_ = Camera(glm::vec3(0.0f,0.0f,1.0f));
    camera_ortho_.set_orthographic(true);

    Spike s (4,1,1.0f,2.0f);

    node_.Add(cuboid_).Add(camera_).Add(shader_);
   
    Node spike_node(s);
    spike_node.set_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,2.0f)));

    node_.Add(spike_node);

    rotation_ = 0;

    fbo_ = FBO(800,600);
    quad_ = Quad(640,480);
    
    node_quad_.Add(quad_).Add(shader_warp_).Add(camera_ortho_).Add(fbo_.colour());

}

///\todo seems not to want to update member variables :(
void FBOApp::Update(double_t dt) {
}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void FBOApp::Display(Context context, GLFWwindow* window, double_t dt){
    GLfloat depth = 1.0f;

    // Draw to the FBO
    fbo_.Bind();
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.2f, 0.5f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );
    rotation_ += 1.0;
    glm::mat4 Model = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));
    node_.set_matrix(Model);
    node_.Draw();
    fbo_.Unbind();

    // Draw to main screen
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );

    node_quad_.Draw();
   
}


/*
 * This is called by the wrapper function when an event is fired
 */

void FBOApp::ProcessEvent(MouseEvent e, GLFWwindow* window){
}

/*
 * Called when the window is resized. You should set cameras here
 */

void FBOApp::ProcessEvent(ResizeEvent e, GLFWwindow* window){
    cout << "Window Resized:" << e.w << "," << e.h << endl;
    camera_.Resize(e.w, e.h);
    camera_ortho_.Resize(e.w, e.h);
    fbo_.Resize(e.w, e.h);

    node_quad_.set_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(e.w / 2.0f, e.h/2.0f, 0.0f)));
}

void FBOApp::ProcessEvent(KeyboardEvent e, GLFWwindow* window){
    cout << "Key Pressed: " << e.key << endl;
}

/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {
  
    FBOApp b;

#ifdef _SEBURO_OSX
    GLFWApp a(b,3,2);
#else
    GLFWApp a(b);
#endif

    a.CreateWindow("FBO", 800, 600);
    a.Run();

    return EXIT_SUCCESS;

}