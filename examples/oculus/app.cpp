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

 void OculusApp::init(){
    shader_ = Shader( s9::File("./shaders/3/quad.vert"),  s9::File("./shaders/3/quad.frag"));
    shader_warp_ = Shader( s9::File("./shaders/glsl/oculus_warp.glsl"));

    addWindowListener(this);

    cuboid_ = Cuboid(3.0,2.0,1.0);
    Cuboid v = cuboid_;
    quad_ = Quad(640,480);

    camera_left_ = Camera(glm::vec3(0.0f,0.0f,10.0f));
    camera_right_ = Camera(glm::vec3(0.0f,0.0f,10.0f));
    
    camera_ortho_ = Camera(glm::vec3(0.0f,0.0f,1.0f));
    camera_ortho_.set_orthographic(true);

    oculus_ = oculus::OculusBase(true);

    
    node_.add(cuboid_).add(shader_).add(camera_left_);
    node_quad_.add(quad_).add(shader_warp_).add(camera_ortho_);

    node_left_.add(camera_left_).add(node_);
    node_right_.add(camera_right_).add(node_);

    // Oculus Shader values
    node_quad_.add( ShaderClause<glm::vec2,1>("uLensCenter", oculus_lens_center_) );
    node_quad_.add( ShaderClause<glm::vec2,1>("uScreenCenter", oculus_screen_center_) );
    node_quad_.add( ShaderClause<glm::vec2,1>("uScale", oculus_scale_) );
    node_quad_.add( ShaderClause<glm::vec2,1>("uScaleIn", oculus_scale_in_) );
    node_quad_.add( ShaderClause<glm::vec4,1>("uHmdWarpParam", oculus_hmd_warp_param_) );
}

///\todo seems not to want to update member variables :(
void OculusApp::update(double_t dt) {    
  
    oculus_.update(dt);
    ///\todo These probably only need to be set once. We'll get a cache at some point for shader values
    //oculus_lens_center_ = oculus_.
    //oculus_screen_center_ = oculus_.screen_center();
    //oculus_scale_ = 
    //oculus_scale_in_ = 
    //oculus_hmd_warp_param_ = oculus_.distortion_parameters();

}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */

 void OculusApp::display(double_t dt){


    if (!fbo_ && oculus_.connected()){
        glm::ivec2 s = oculus_.screen_resolution();
        glm::vec2 f = oculus_.screen_size();
        fbo_ = FBO(static_cast<size_t>(s.x),static_cast<size_t>(s.y)); // Match the oculus rift
        node_.add(fbo_.colour());

        float aspect = s.x / (2.0 * s.y);
        float fov = 2.0 * atan( s.y / (2.0 * oculus_.eye_to_screen_distance()));

        glm::mat4 perspective = glm::perspective( fov, aspect, 0.1f, 100.0f);

        camera_left_.resize(static_cast<size_t>(s.x / 2), static_cast<size_t>(s.y));
        camera_right_.resize(static_cast<size_t>(s.x / 2), static_cast<size_t>(s.y));

        // Is this the absoulte seperation? Should we be dividing by two?
        // Make sure - is on the right side

        float offset_metres = f.x /4.0 -  oculus_.lens_separation_distance() / 2.0; // in metres
        float offset = 4.0 * offset / f.x;

        glm::mat4 left_offset = glm::translate(glm::mat4(1.0f), glm::vec3(offset,0.0f,0.0f));
        glm::mat4 right_offset = glm::translate(glm::mat4(1.0f), glm::vec3(-offset,0.0f,0.0f));

        float dip = oculus_.interpupillary_distance() / 2.0;

        glm::mat4 left_inter = glm::translate(glm::mat4(1.0f), glm::vec3(dip,0.0f,0.0f));
        glm::mat4 right_inter = glm::translate(glm::mat4(1.0f), glm::vec3(-dip,0.0f,0.0f));

        camera_left_.set_projection_matrix(left_inter * left_offset * perspective);
        camera_right_.set_projection_matrix(right_inter * right_offset * perspective);

    }


    GLfloat depth = 1.0f;

    // Draw to the FBO
    if (fbo_){
        fbo_.bind();
        glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.6f, 0.6f, 0.6f, 1.0f)[0]);
        glClearBufferfv(GL_DEPTH, 0, &depth );

        glm::quat q = oculus_.orientation();
        glm::mat4 mq = glm::mat4_cast(q);

        // Update cameras for both eyes
        oculus_dt_ = glm::inverse(oculus_prev_) * q;
        //camera_left_.rotate(oculus_dt_);
        //camera_right_.rotate(oculus_dt_);
        oculus_prev_ = q;

        // Draw twice (sadly)
        node_left_.draw();
        //node_right_.draw();

        fbo_.unbind();
    }
   

    // Draw to main screen
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );

    node_quad_.draw();

}


/*
 * This is called by the wrapper function when an event is fired
 */

 void OculusApp::processEvent(MouseEvent e){
 }

/*
 * Called when the window is resized. You should set cameras here
 */

 void OculusApp::processEvent(ResizeEvent e){
    cout << "Window Resized:" << e.w << "," << e.h << endl;
    camera_ortho_.resize(e.w, e.h);

    node_quad_.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(e.w/2.0f, e.h/2.0f, 0.0f)));
}

void OculusApp::processEvent(KeyboardEvent e){
    cout << "Key Pressed: " << e.key << endl;
}

/*
 * Main function - uses boost to parse program arguments
 */

 int main (int argc, const char * argv[]) {

    OculusApp b;

#ifdef _SEBURO_OSX
    GLFWApp a(b, 800, 600, false, argc, argv, "Oculus",3,2);
#else
    GLFWApp a(b, 800, 600, false, argc, argv, "Oculus");
#endif

    return EXIT_SUCCESS;

}