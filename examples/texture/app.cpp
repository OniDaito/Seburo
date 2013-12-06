/**
* @brief GLFW Window based solution
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 21/10/2013
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

void TextureApp::init(){
    shader_ = Shader( s9::File("./shaders/3/quad_texture.vert"), s9::File("./shaders/3/quad_texture.frag"));

    addWindowListener(this);

    glEnable(GL_TEXTURE_RECTANGLE);
    glEnable(GL_TEXTURE_2D);

    quad_ = Quad(1.0,1.0);
    texture_ = Texture( Image(s9::File("./data/astley.jpg")) );
    rotation_ = 0;
    camera_ = Camera(glm::vec3(0,0,-6.0f));

    node_.add(quad_).add(camera_);

    shader_.bind();
    texture_.bind();
   
}

///\todo seems not to want to update member variables :(
void TextureApp::update(double_t dt) {}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void TextureApp::display(double_t dt){

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    rotation_ += 1.0;
    camera_.update(dt);

    glm::mat4 mat = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));

    node_.setMatrix(mat);
    node_.draw();
}



/*
 * This is called by the wrapper function when an event is fired
 */

void TextureApp::processEvent(MouseEvent e){}

/*
 * Called when the window is resized. You should set cameras here
 */

void TextureApp::processEvent(ResizeEvent e){
    glViewport(0,0,e.w,e.h);
    camera_.resize(e.w,e.h);
}

void TextureApp::processEvent(KeyboardEvent e){}

/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {

    TextureApp b;

#ifdef _SEBURO_OSX
    GLFWApp a(b, 800, 600, false, argc, argv, "Texture", 3, 2);
#else
    GLFWApp a(b, 800, 600, false, argc, argv, "Texture");
#endif

    return EXIT_SUCCESS;

}