/**
* @brief GLFW Window based solution
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 29/10/2013
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

void MD5App::Init(){
    shader_ = Shader(s9::File("./shaders/3/skinning.glsl"));
    shader_colour_ = Shader(s9::File("./shaders/3/solid_colour.glsl"));

    show_wireframe_ = true;

    quad_ = Quad(1.0,1.0);
    rotation_ = 0;

    bone_colour_ = glm::vec4(1.0,0.0,0.0,1.0);
    orient_colour_ = glm::vec4(0.0,0.0,1.0,1.0);
   
    camera_ = Camera( glm::vec3(0,0,2.0f));
    md5_ = MD5Model( s9::File("./data/sintel_lite/sintel_lite.md5mesh") ); 
    md5_.set_geometry_cast(WIREFRAME);

    node_.add(md5_).add(camera_).add(shader_);
 
    skeleton_shape_ = SkeletonShape(md5_.skeleton());
    skeleton_shape_.set_geometry_cast(WIREFRAME);
    skeleton_shape_.add(shader_colour_).add(camera_);
  
    node_.add(skeleton_shape_);


    node_full_.add(md5_).add(camera_).add(shader_);

    /*Bone * neck = md5_.skeleton().bone("neck");
    neck->applyRotation ( glm::angleAxis( 20.0f, glm::vec3(0.0,1.0,0.0)) );

    Bone * waist = md5_.skeleton().bone("waist");
    waist->applyRotation( glm::angleAxis( -45.0f, glm::vec3(1.0,0.0,0.0)) );
    waist->applyRotation( glm::angleAxis( -25.0f, glm::vec3(0.0,1.0,0.0)) );
 
    Bone * luparm = md5_.skeleton().bone("luparm");
    luparm->applyRotation( glm::angleAxis( -45.0f, glm::vec3(0.0,1.0,0.0))  );
    //luparm->applyRotation( glm::angleAxis( 15.0f, glm::vec3(0.0,0.0,1.0))  );

    Bone * lloarm = md5_.skeleton().bone("lloarm");
    lloarm->applyRotation( glm::angleAxis( -45.0f, glm::vec3(0.0,0.0,1.0))  );
    //lloarm->applyRotation( glm::angleAxis( 15.0f, glm::vec3(0.0,0.0,1.0))  );
   */
    cout << node_ << endl;

    CXGLERROR


}

///\todo seems not to want to update member variables :(
void MD5App::Update(double_t dt) {

    Bone * waist = md5_.skeleton().bone("upper_arm.L");
    waist->applyRotation( glm::angleAxis( -0.005f, glm::vec3(0.0,0.0,1.0)) );

    Bone * luparm = md5_.skeleton().bone("thigh.L");
    luparm->applyRotation( glm::angleAxis( 0.005f, glm::vec3(0.0,1.0,0.0))  );

    md5_.skeleton().update();
}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void MD5App::Display(GLFWwindow *window, double_t dt){

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );
    
    if (show_wireframe_){
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
    } else {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);
    }

    rotation_ += 0.5;
    glm::mat4 Model = glm::rotate(glm::mat4(), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));
    Model = glm::translate(Model, glm::vec3(0.0,-0.8,0.0));
    //Model = glm::scale(Model, glm::vec3(0.01,0.01,0.01));
    Model = glm::rotate(Model, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    node_.set_matrix(Model);
    node_full_.set_matrix(Model);

    if (show_wireframe_)
        node_.draw();
    else
        node_full_.draw();

    CXGLERROR
}



/*
 * This is called by the wrapper function when an event is fired
 */

void MD5App::ProcessEvent(MouseEvent e, GLFWwindow *window){}

/*
 * Called when the window is resized. You should set cameras here
 */

void MD5App::ProcessEvent(ResizeEvent e, GLFWwindow *window){
    glViewport(0,0,e.w,e.h);
    camera_.resize(e.w,e.h);
}

void MD5App::ProcessEvent(KeyboardEvent e, GLFWwindow *window){
    // If 'w' pressed

    if (e.key == 87) {
        if (e.action == GLFW_PRESS){
          
            show_wireframe_ = !show_wireframe_;
            if (show_wireframe_){
                md5_.set_geometry_cast(WIREFRAME);
            } else {
                md5_.set_geometry_cast(NONE);
            }
        }
        
    }

}

/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {

    MD5App b;

#ifdef _SEBURO_OSX
    GLFWApp a(b, 3, 2);
#else
    GLFWApp a(b);
#endif

    a.Run();

    return EXIT_SUCCESS;

}