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

void MD5App::init(){
    shader_ = Shader(s9::File("./shaders/3/skinning.glsl"));
    shader_colour_ = Shader(s9::File("./shaders/3/solid_colour.glsl"));

    addWindowListener(this);

    quad_ = Quad(1.0,1.0);
    rotation_ = 0;

    bone_colour_ = glm::vec4(1.0,0.0,0.0,1.0);
    orient_colour_ = glm::vec4(0.0,0.0,1.0,1.0);
   
    camera_= Camera( glm::vec3(0,0,18.0f));
    md5_ = MD5Model( s9::File("./data/hellknight.md5mesh") ); 
    md5_.set_geometry_cast(WIREFRAME);

    node_.add(md5_).add(camera_).add(shader_);
 
    glm::mat4 Model = glm::rotate(glm::mat4(), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));
    Model = glm::translate(Model, glm::vec3(0.0,-6.0,0.0));
    Model = glm::scale(Model, glm::vec3(0.1,0.1,0.1));
    node_.setMatrix(Model);

    skeleton_shape_ = SkeletonShape(md5_.skeleton());
    skeleton_shape_.set_geometry_cast(WIREFRAME);
    skeleton_shape_.add(shader_colour_).add(camera_);
  
    node_.add(skeleton_shape_);

    Bone * neck = md5_.skeleton().bone("neck");
    neck->applyRotation ( glm::angleAxis( 20.0f, glm::vec3(0.0,1.0,0.0)) );

    Bone * waist = md5_.skeleton().bone("waist");
    waist->applyRotation( glm::angleAxis( -45.0f, glm::vec3(1.0,0.0,0.0)) );
    waist->applyRotation( glm::angleAxis( -25.0f, glm::vec3(0.0,1.0,0.0)) );


    Bone * luparm = md5_.skeleton().bone("luparm");
    luparm->applyRotation( glm::angleAxis( -45.0f, glm::vec3(1.0,0.0,0.0))  );
    luparm->applyRotation( glm::angleAxis( 15.0f, glm::vec3(0.0,0.0,1.0))  );

    Bone * lloarm = md5_.skeleton().bone("lloarm");
    lloarm->applyRotation( glm::angleAxis( -45.0f, glm::vec3(1.0,0.0,0.0))  );
    lloarm->applyRotation( glm::angleAxis( 15.0f, glm::vec3(0.0,0.0,1.0))  );
    
    cout << node_ << endl;

    CXGLERROR

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

///\todo seems not to want to update member variables :(
void MD5App::update(double_t dt) {

    Bone * waist = md5_.skeleton().bone("lupleg");
    waist->applyRotation( glm::angleAxis( -0.005f, glm::vec3(0.0,0.0,1.0)) );

    md5_.skeleton().update();
}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void MD5App::display(double_t dt){


    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );
    
    rotation_ += 0.5;
    glm::mat4 Model = glm::rotate(glm::mat4(), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));
    Model = glm::translate(Model, glm::vec3(0.0,-6.0,0.0));
    Model = glm::scale(Model, glm::vec3(0.1,0.1,0.1));
    Model = glm::rotate(Model, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    node_.setMatrix(Model);

    
    node_.draw();


    //CXGLERROR
}



/*
 * This is called by the wrapper function when an event is fired
 */

void MD5App::processEvent(MouseEvent e){}

/*
 * Called when the window is resized. You should set cameras here
 */

void MD5App::processEvent(ResizeEvent e){
    glViewport(0,0,e.w,e.h);
    camera_.resize(e.w,e.h);
}

void MD5App::processEvent(KeyboardEvent e){}

/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {

    MD5App b;

#ifdef _SEBURO_OSX
    GLFWApp a(b, 800, 600, false, argc, argv, "MD5", 3, 2);
#else
    GLFWApp a(b, 800, 600, false, argc, argv, "MD5");
#endif

    return EXIT_SUCCESS;

}