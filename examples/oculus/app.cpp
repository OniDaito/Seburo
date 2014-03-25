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
 * 1 GL Unit = 1m
 */

 void OculusApp::Init(Context context){
    shader_ = Shader( s9::File("./shaders/3/quad.vert"),  s9::File("./shaders/3/quad.frag"));
    shader_warp_ = Shader( s9::File("./shaders/1.5/barrel.vert"), 
        s9::File("./shaders/1.5/barrel.frag"),
        s9::File("./shaders/1.5/barrel.geom"));

    cuboid_ = Cuboid(0.3,0.3,0.1);
    Cuboid v = cuboid_;
    quad_ = Quad(1.0,1.0);

    Spike s (4,1,1.0f,2.0f);
    Node spike_node(s);
    spike_node.set_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(1.5f,0.0f,1.0f)));

    Node cube_node_01(cuboid_);
    cube_node_01.set_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f,0.0f,2.0f)));
    
    camera_main_ = Camera(glm::vec3(0.0f,0.0f,0.9f));
    camera_left_ = Camera(glm::vec3(0.0f,0.0f,0.0f));
    camera_right_ = Camera(glm::vec3(0.0f,0.0f,0.0f));

    camera_left_.set_update_on_node_draw(false);
    camera_right_.set_update_on_node_draw(false);

    camera_main_.set_update_on_node_draw(false);
    camera_main_.Resize(1280,800);

    oculus_ = oculus::OculusBase(0.01f,100.0f);

    node_.Add(cuboid_).Add(spike_node).Add(cube_node_01).Add(shader_);

    node_main_.Add(node_).Add(camera_main_);
   
    node_left_.Add(camera_left_).Add(node_);
    node_right_.Add(camera_right_).Add(node_);


}

///\todo seems not to want to update member variables :(
void OculusApp::Update(double_t dt) {    
    oculus_.Update(dt);
}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */

 void OculusApp::Display(Context context, GLFWwindow* window, double_t dt){

    // Create the FBO and setup the cameras
    if (!fbo_ && oculus_.Connected()){
      
        glm::vec2 s = oculus_.fbo_size();
        fbo_ = FBO(static_cast<size_t>(s.x), static_cast<size_t>(s.y)); 
        node_quad_.Add(fbo_.colour());

        camera_left_.Resize(static_cast<size_t>(s.x / 2.0f), static_cast<size_t>(s.y ));
        camera_right_.Resize(static_cast<size_t>(s.x / 2.0f), static_cast<size_t>(s.y ),static_cast<size_t>(s.x / 2.0f) );

        camera_main_.Resize(static_cast<size_t>(s.x ), static_cast<size_t>(s.y ));

        camera_left_.set_projection_matrix(oculus_.left_projection());
        camera_right_.set_projection_matrix(oculus_.right_projection());

        glGenVertexArrays(1, &(null_VAO_));
        
    }


    GLfloat depth = 1.0f;

    // Draw to the FBO
    if (fbo_){
        fbo_.Bind();
        glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.5f, 0.9f, 0.95f, 1.0f)[0]);
        glClearBufferfv(GL_DEPTH, 0, &depth );

        glm::quat q = glm::inverse(oculus_.orientation());

        // Update main camera
        oculus_dt_ = glm::inverse(oculus_prev_) * q;
        camera_main_.Rotate(oculus_dt_);
        camera_main_.Update();

        // Now set the view matrices given the oculus values
        camera_left_.set_view_matrix( camera_main_.view_matrix() * oculus_.left_inter() );
        camera_right_.set_view_matrix(  camera_main_.view_matrix() * oculus_.right_inter() );

        oculus_prev_ = q;

        // Draw twice (sadly)
        node_left_.Draw();
        node_right_.Draw();

        fbo_.Unbind();
        CXGLERROR

        // Draw to main screen - this cheats and uses a geometry shader
        glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
        glClearBufferfv(GL_DEPTH, 0, &depth );

        glBindVertexArray(null_VAO_);

        glViewport(0,0, oculus_.screen_resolution().x, oculus_.screen_resolution().y);

        shader_warp_.Bind();
        fbo_.colour().Bind();

        shader_warp_.s("uDistortionOffset", oculus_.distortion_xcenter_offset() ); // Can change with future headsets apparently
        shader_warp_.s("uDistortionScale", 1.0f/oculus_.distortion_scale());
        shader_warp_.s("uChromAbParam", oculus_.chromatic_abberation());
        shader_warp_.s("uHmdWarpParam",oculus_.distortion_parameters());

        glDrawArrays(GL_POINTS, 0, 1);

        fbo_.colour().Unbind();
        shader_warp_.Unbind();

        glBindVertexArray(0);

    }
   


}


/*
 * This is called by the wrapper function when an event is fired
 */

 void OculusApp::ProcessEvent(MouseEvent e, GLFWwindow* window){
 }

/*
 * Called when the window is Resized. You should set cameras here
 */

 void OculusApp::ProcessEvent(ResizeEvent e, GLFWwindow* window){
    cout << "Window Resized:" << e.w << "," << e.h << endl;

    glm::mat4 quad_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(e.w/2.0f, e.h/2.0f, 0.0f)) ;
    quad_matrix =  glm::scale(quad_matrix, glm::vec3(e.w, e.h, 1.0f));        
    node_quad_.set_matrix(quad_matrix);

    
}

void OculusApp::ProcessEvent(KeyboardEvent e, GLFWwindow* window){
    cout << "Key Pressed: " << e.key << endl;
}

/*
 * Main function - uses boost to parse program arguments
 */

 int main (int argc, const char * argv[]) {

    OculusApp b;

#ifdef _SEBURO_OSX
    GLFWApp a(b, 3, 2);
#else
    GLFWApp a(b);
#endif

    // Change HDMI-0 to whatever is listed in the output for the GLFW Monitor Screens
    a.CreateWindowFullScreen("Oculus", 1280, 800, "HDMI-0");

    //a.CreateWindow("Oculus",1280,800);
    a.Run();

    return EXIT_SUCCESS;

}