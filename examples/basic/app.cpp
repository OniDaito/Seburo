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

void BasicApp::Init(Context context){
    //shader_ = Shader( s9::File("./shaders/3/quad.vert"),  s9::File("./shaders/3/quad.frag"));
    ShaderBuilder builder (context); 
    builder.AddSnippet("Basic").AddSnippet("VertexColour").AddSnippet("BasicCamera");

    // Annoyingly, still need to shift on context here for the moment
    //if (global_context->major_version() < 4){
    //    builder.AddUserText(VERTEX_MAIN, "gl_Position = uCameraMatrix * uProjectionMatrix * uModelMatrix * aVertexPosition;");
    //    builder.AddUserText(FRAGMENT_MAIN, "gl_FragColor = vVertexColour;");
    //}
    //else {
        builder.AddUserText(VERTEX_MAIN, "vVertexPosition = uCameraMatrix * uProjectionMatrix * uModelMatrix * aVertexPosition;");
        builder.AddUserText(FRAGMENT_MAIN, "fragColor = vVertexColour;");
    //}

    shader_ = builder.Build();

    camera_= Camera( glm::vec3(0,0,10.0f), glm::vec3(0,0,0.0f));
    top_node_.Add(shader_).Add(camera_);
    camera_.set_field(55.0f);

    cuboid_ = Cuboid(1.0f,2.5f,1.0f);
    Node cuboid_node(cuboid_);
    cuboid_node.set_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,2.0f)));
    top_node_.Add(cuboid_node);

  
    Sphere s (0.5f, 20);
    Node spike_node(s);
    spike_node.set_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f,0.0f,0.0f)));
    top_node_.Add(spike_node);

    Node spike_node2(s);
    spike_node2.set_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f,0.0f,0.0f)));
    top_node_.Add(spike_node2);

    rotation_ = 0.1f;

}

void BasicApp::Update(double_t dt) {

}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void BasicApp::Display(Context context, GLFWwindow* window, double_t dt){

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    rotation_ += 1.0f;
   
    glm::mat4 Model = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));


    top_node_.set_matrix(Model);

    top_node_.Draw();
   
    CXGLERROR
}


void BasicApp::ProcessEvent(CloseWindowEvent e, GLFWwindow* window) {

}

/*
 * This is called by the wrapper function when an event is fired
 */

void BasicApp::ProcessEvent(MouseEvent e, GLFWwindow* window){
}

/*
 * Called when the window is resized. You should set cameras here
 */

void BasicApp::ProcessEvent(ResizeEvent e, GLFWwindow* window){
    cout << "Window Resized:" << e.w << "," << e.h << endl;
    camera_.Resize(e.w,e.h);
}

void BasicApp::ProcessEvent(KeyboardEvent e, GLFWwindow* window){
    cout << "Key Pressed: " << e.key << endl;
}

/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {
  
    ///\todo better command line args parsing

    BasicApp b;

#ifdef _SEBURO_OSX
    GLFWApp a(b,3,2);
#else
    GLFWApp a(b);
#endif

    // Create our initial window
    a.CreateWindow("Basic", 800, 600);

    // Now kick off the OpenGL side of things
    a.Run();
}