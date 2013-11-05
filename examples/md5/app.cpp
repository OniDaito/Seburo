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
    shader_.load( s9::File("./shaders/3/quad_texture.vert").path(), s9::File("./shaders/3/quad_texture.frag").path());

    addWindowListener(this);

    quad_ = Quad(1.0,1.0);
    //node_.add(quad_);
    //texture_ = Texture( Image(s9::File("./data/astley.jpg")) );
    rotation_ = 0;

    camera_.set_pos(glm::vec3(0,0,18.0f));

    md5_ = MD5Model( s9::File("./data/hellknight.md5mesh") ); 

    node_.add(shader_);
    node_.add(md5_);

    cout << md5_ << endl;
}

///\todo seems not to want to update member variables :(
void MD5App::update(double_t dt) {}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void MD5App::display(double_t dt){

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    rotation_ += 1.0;
    camera_.update(dt);
    /*shader_.bind();

    
    
    glm::mat4 Model = glm::rotate(glm::mat4(), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));
    Model = glm::translate(Model, glm::vec3(0.0,-6.0,0.0));
    
    Model = glm::scale(Model, glm::vec3(0.1,0.1,0.1));
    glm::mat4 MVP = camera_.projection_matrix() * camera_.view_matrix() * Model;

    shader_.s("uMVPMatrix",MVP);*/
   
    // texture_.bind();
    node_.draw();
    // texture_.unbind();

    //shader_.unbind();
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