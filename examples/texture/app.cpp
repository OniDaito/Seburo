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

void TextureApp::Init(Context context){
    shader_ = Shader( s9::File("./shaders/3/quad_texture.vert"), s9::File("./shaders/3/quad_texture.frag"));

    quad_ = Quad(1.0,1.0);
    texture_ = Texture( Image(s9::File("./data/astley.jpg")));
    rotation_ = 0;
    camera_ = Camera(glm::vec3(0,0,-6.0f));
    unit_ = 0;

    node_.Add(quad_).Add(shader_).Add(texture_).Add(camera_).Add( gl::ShaderClause<int,1>("uTexSampler0", unit_) );
}


void TextureApp::Update(double_t dt) {}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void TextureApp::Display(Context context, GLFWwindow* window, double_t dt){

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    rotation_ += 1.0;
    camera_.Update(dt);

    glm::mat4 mat = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));

    node_.set_matrix(mat);
    node_.Draw();
}



/*
 * This is called by the wrapper function when an event is fired
 */

void TextureApp::ProcessEvent(MouseEvent e, GLFWwindow* window){}

/*
 * Called when the window is resized. You should set cameras here
 */

void TextureApp::ProcessEvent(ResizeEvent e, GLFWwindow* window){
    camera_.Resize(e.w,e.h);
}

void TextureApp::ProcessEvent(KeyboardEvent e, GLFWwindow* window){

}


/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {

    TextureApp b;

#ifdef _SEBURO_OSX
    GLFWApp a(b, 3, 2);
#else
    GLFWApp a(b);
#endif

    a.Run();

    return EXIT_SUCCESS;

}