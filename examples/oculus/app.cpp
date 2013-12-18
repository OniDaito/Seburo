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

 void OculusApp::init(){
    shader_ = Shader( s9::File("./shaders/3/quad.vert"),  s9::File("./shaders/3/quad.frag"));
    shader_warp_ = Shader( s9::File("./shaders/1.5/barrel.vert"), 
        s9::File("./shaders/1.5/barrel.frag"),
        s9::File("./shaders/1.5/barrel.geom"));

    addWindowListener(this);

    cuboid_ = Cuboid(1.0,1.0,1.0);
    Cuboid v = cuboid_;
    quad_ = Quad(1.0,1.0);

    Spike s (4,1,1.0f,2.0f);
    Node spike_node(s);
    spike_node.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f,0.0f,2.0f)));

    Node cube_node_01(cuboid_);
    cube_node_01.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f,0.0f,5.0f)));
    
    camera_main_ = Camera(glm::vec3(0.0f,0.0f,5.0f));
    camera_left_ = Camera(glm::vec3(0.0f,0.0f,0.0f));
    camera_right_ = Camera(glm::vec3(0.0f,0.0f,0.0f));

    camera_left_.set_update_on_node_draw(false);
    camera_right_.set_update_on_node_draw(false);

    camera_main_.set_update_on_node_draw(false);
    camera_main_.resize(1280,800);

    camera_ortho_ = Camera(glm::vec3(0.0f,0.0f,1.0f));
    camera_ortho_.set_orthographic(true);

    oculus_ = oculus::OculusBase(true);

    node_.add(cuboid_).add(spike_node).add(cube_node_01).add(shader_);

    node_main_.add(node_).add(camera_main_);
   
    node_left_.add(camera_left_).add(node_);
    node_right_.add(camera_right_).add(node_);


}

///\todo seems not to want to update member variables :(
void OculusApp::update(double_t dt) {    
  
    oculus_.update(dt);
}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */

 void OculusApp::display(double_t dt){


    if (!fbo_ && oculus_.connected()){
        glm::vec2 s = glm::vec2( oculus_.screen_resolution().x * oculus_.distortion_scale(), oculus_.screen_resolution().y * oculus_.distortion_scale());
        glm::vec2 f = oculus_.screen_size() * oculus_.distortion_scale();
        fbo_ = FBO(static_cast<size_t>(s.x), static_cast<size_t>(s.y)); 
        node_quad_.add(fbo_.colour());

        float aspect = static_cast<float>(s.x) * 0.5f / static_cast<float>(s.y);
        float half_screen = f.y * 0.5f;
        float fov = 2.0f * atan( half_screen / oculus_.eye_to_screen_distance());

        camera_left_.resize(static_cast<size_t>(s.x / 2.0f), static_cast<size_t>(s.y ));
        camera_right_.resize(static_cast<size_t>(s.x / 2.0f), static_cast<size_t>(s.y ),static_cast<size_t>(s.x / 2.0f) );
        camera_main_.resize(static_cast<size_t>(s.x ), static_cast<size_t>(s.y ));

        // Is this the absoulte seperation? Should we be dividing by two?
        // Make sure - is on the right side

        float view_center = f.x * 0.25f;
        float eye_shift = view_center - (oculus_.lens_separation_distance() * 0.5f);
        float projection_offset = (4.0f * eye_shift) / f.x;

        // Concerned with the FOV. I'm halving it twice. Once for sure but twice?
        glm::mat4 perspective = glm::perspective( radToDeg(fov) * 0.5f, aspect, 0.3f, 1000.0f);


        glm::mat4 left_proj = glm::translate(perspective, glm::vec3(projection_offset,0.0f,0.0f));
        glm::mat4 right_proj = glm::translate(perspective, glm::vec3(-projection_offset,0.0f,0.0f));

        camera_left_.set_projection_matrix(left_proj);
        camera_right_.set_projection_matrix(right_proj);

        float dip = oculus_.interpupillary_distance() * 0.5f;

        left_inter_ = glm::translate(glm::mat4(1.0f), glm::vec3(dip,0.0f,0.0f));
        right_inter_ = glm::translate(glm::mat4(1.0f), glm::vec3(-dip,0.0f,0.0f));

        camera_ortho_.resize(1280, 800);

        glGenVertexArrays(1, &(null_VAO_));
        
    }


    GLfloat depth = 1.0f;

    // Draw to the FBO
    if (fbo_){
        fbo_.bind();
        glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.5f, 0.9f, 0.95f, 1.0f)[0]);
        glClearBufferfv(GL_DEPTH, 0, &depth );

        glm::quat q = glm::inverse(oculus_.orientation());

        // Update main camera
        oculus_dt_ = glm::inverse(oculus_prev_) * q;
        camera_main_.rotate(oculus_dt_);
        camera_main_.update();

        // Now set the view matrices given the oculus values
        camera_left_.set_view_matrix( camera_main_.view_matrix() * left_inter_ );
        camera_right_.set_view_matrix(  camera_main_.view_matrix() * right_inter_ );

        oculus_prev_ = q;

        // Draw twice (sadly)
        node_left_.draw();
        node_right_.draw();

        fbo_.unbind();
        CXGLERROR

    }
   

    // Draw to main screen - this cheats and uses a geometry shader
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );

    glBindVertexArray(null_VAO_);

    glViewport(0,0, camera_ortho_.width(), camera_ortho_.height());

    shader_warp_.bind();
    fbo_.colour().bind();

    shader_warp_.s("uDistortionOffset", oculus_.distortion_xcenter_offset()); // Can change with future headsets apparently
    shader_warp_.s("uDistortionScale", 1.0f/oculus_.distortion_scale());
    shader_warp_.s("uChromAbParam", oculus_.chromatic_abberation());
    shader_warp_.s("uHmdWarpParam",oculus_.distortion_parameters() );

    glDrawArrays(GL_POINTS, 0, 1);

    fbo_.colour().unbind();
    shader_warp_.unbind();

    glBindVertexArray(0);

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

    glm::mat4 quad_matrix = glm::translate(glm::mat4(1.0f),glm::vec3(e.w/2.0f, e.h/2.0f, 0.0f)) ;
    quad_matrix =  glm::scale(quad_matrix, glm::vec3(e.w, e.h, 1.0f));        
    node_quad_.setMatrix(quad_matrix);

    
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
    GLFWApp a(b, 1280, 800, false, argc, argv, "Oculus",3,2);
#else
    GLFWApp a(b, 1280, 800, false, argc, argv, "Oculus");
#endif

    return EXIT_SUCCESS;

}