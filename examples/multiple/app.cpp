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

MultipleApp::MultipleApp(){

    window_manager_.AddListener(this);
    time_passed_ = 0;
    frames_ = 0;

    // AntTweakBar
    tweakbar_ = TwNewBar("Tweakbar MultipleApp");
    TwAddVarRW(tweakbar_, "NameOfMyVariable", TW_TYPE_FLOAT, &rotation_, "");
    
    // Create a GL Window, passing functions to that window

    std::function<void()> initZero = std::bind(&MultipleApp::InitZero, this);
    std::function<void(double_t)> drawZero = std::bind(&MultipleApp::DrawZero, this, std::placeholders::_1);
    const GLWindow &windowZero = window_manager_.CreateWindow("Multiple Application 0",800,600, initZero, drawZero );

    std::function<void()> initOne = std::bind(&MultipleApp::InitOne, this);
    std::function<void(double_t)> drawOne = std::bind(&MultipleApp::DrawOne, this, std::placeholders::_1);
    const GLWindow &windowOne = window_manager_.CreateWindow("Multiple Application 1",800,600, initOne, drawOne );


    rotation_ = 0.1f;
}


void MultipleApp::InitZero() {
    //shader_ = Shader( s9::File("./shaders/3/quad.vert"),  s9::File("./shaders/3/quad.frag"));
    
    TwWindowSize(800, 600);

    int major_version;
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    ShaderBuilder builder (major_version); 

    builder.AddSnippet("Basic4").AddSnippet("VertexColour").AddSnippet("BasicCamera");

    texture_ = Texture( Image(s9::File("./data/astley.jpg")));
    quad_zero_ = Quad(1.0,1.0);

   
    builder.AddUserText(VERTEX_MAIN, "vVertexPosition = aVertexPosition;");
    builder.AddUserText(VERTEX_MAIN, "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vVertexPosition;");
    builder.AddUserText(FRAGMENT_MAIN, "fragColour = vVertexColour;");
    

    shader_ = builder.Build();

    camera_zero_= Camera( glm::vec3(0,0,10.0f), glm::vec3(0,0,0.0f));
    top_node_zero_.Add(shader_).Add(camera_zero_);
    camera_zero_.set_field(55.0f);

    cuboid_ = Cuboid(1.0f,2.5f,1.0f);
    Node cuboid_node(cuboid_);
    cuboid_node.set_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,2.0f)));
    top_node_zero_.Add(cuboid_node);

  
    Sphere s (0.5f, 20);
    Node spike_node(s);
    spike_node.set_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f,0.0f,0.0f)));
    top_node_zero_.Add(spike_node);

    Node spike_node2(s);
    spike_node2.set_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f,0.0f,0.0f)));
    top_node_zero_.Add(spike_node2);

    //glFinish();

    
}

void MultipleApp::InitOne() {
    int major_version;
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    ShaderBuilder builder (major_version); 
    builder.AddSnippet("Basic4").AddSnippet("VertexColour").AddSnippet("BasicCamera").AddSnippet("VertexTexCoord").AddSnippet("BasicTexture2");
    builder.AddUserText(FRAGMENT_MAIN,"fragColour = vec4(texcolour.rgb,1.0);");

    builder.AddUserText(VERTEX_MAIN, "vVertexPosition = aVertexPosition;");
    builder.AddUserText(VERTEX_MAIN, "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vVertexPosition;");

    shader_texture_ = builder.Build();

    camera_one_= Camera( glm::vec3(0,0,10.0f), glm::vec3(0,0,0.0f));
    top_node_one_.Add(camera_one_);
    camera_one_.set_field(55.0f);

    quad_one_ = Quad(1.0,1.0);
    top_node_one_.Add(quad_one_).Add(texture_).Add(shader_texture_);
}


void MultipleApp::MainLoop(double_t dt) {
    window_manager_.MainLoop();
}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void MultipleApp::DrawZero(double_t dt){

    frames_++;
    if (frames_ > 100){
        cout << "FPS: " << 100 / time_passed_ << endl;
        frames_ = 0;
        time_passed_ = 0;
    }
    time_passed_ += dt;

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );
   
    glm::mat4 Model = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));


    top_node_zero_.set_matrix(Model);

    top_node_zero_.Draw();

    TwDraw();
   
    CXGLERROR

    glFinish();
}

void MultipleApp::DrawOne(double_t dt){

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.1f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );


    glm::mat4 Model = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));

    top_node_one_.set_matrix(Model);

    top_node_one_.Draw();
 
    CXGLERROR

}



void MultipleApp::ProcessEvent( const GLWindow &window, CloseWindowEvent e) {
    
    // One window left and we are about to close it

    if (window_manager_.NumWindows() <= 1){
        Shutdown();
    }
}

/*
 * This is called by the wrapper function when an event is fired
 */

void MultipleApp::ProcessEvent( const GLWindow &window, MouseEvent e){

}

/*
 * Called when the window is resized. You should set cameras here
 */

void MultipleApp::ProcessEvent( const GLWindow &window, ResizeEvent e){
    cout << "Window Resized:" << e.w << "," << e.h << endl;
    
    if ( window.window_name().compare("Multiple Application 0") == 0) {
        camera_zero_.Resize(e.w,e.h);
        TwWindowSize(e.w,e.h);
    }
}

void MultipleApp::ProcessEvent( const GLWindow &window, KeyboardEvent e){
    cout << "Key Pressed: " << e.key << endl;
}

/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {
  
    ///\todo better command line args parsing

    MultipleApp a;

    // Now kick off the OpenGL side of things
    a.Run();
    
    return EXIT_SUCCESS;
}