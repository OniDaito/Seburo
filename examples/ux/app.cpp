/**
* @brief GLFW Window based solution
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#include "app.hpp"

using namespace std;
using namespace s9;
using namespace s9::gl;


/*
 * Called when the mainloop starts, just once
 */

void UxApp::init(){
    shader_ = Shader( s9::File("./shaders/3/quad.vert"),  s9::File("./shaders/3/quad.frag"));

    addWindowListener(this);

    cuboid_ = Cuboid(3.0,2.0,1.0);

    camera_= Camera( glm::vec3(0,0,10.0f));

    Spike s (4,1,1.0f,2.0f);

    node_.add(cuboid_).add(shader_).add(camera_);

    top_node_2_.add(s);
    top_node_2_.add(camera_).add(shader_);

    Node spike_node(s);

    spike_node.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,2.5f,0.0f)));

    node_.add(spike_node);

    top_node_.add(node_);
    top_node_.setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f,0.0f,0.0f)));

    rotation_ = 0;

}


void UxApp::update(double_t dt) {

}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void UxApp::display(double_t dt){

    // Place on the main thread 
#ifdef _SEBURO_LINUX
    gtk_main_iteration_do(false);
#endif

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    rotation_ += 1.0;
   
    glm::mat4 Model = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));
    node_.setMatrix(Model);

    top_node_.draw();
    top_node_2_.draw();
   
    CXGLERROR
}


/*
 * This is called by the wrapper function when an event is fired
 */

void UxApp::processEvent(MouseEvent e){
}

/*
 * Called when the window is resized. You should set cameras here
 */

void UxApp::processEvent(ResizeEvent e){
    cout << "Window Resized:" << e.w << "," << e.h << endl;
    glViewport(0,0,e.w,e.h);
    camera_.resize(e.w,e.h);
}

void UxApp::processEvent(KeyboardEvent e){
    cout << "Key Pressed: " << e.key << endl;
}


/*
 * The UX window Class
 */

#ifdef _SEBURO_LINUX


UXWindow::UXWindow(UxApp &app) : app_(app), button_ ("Hello World")  {   // creates a new button with label "Hello World".
  // Sets the border width of the window.
  set_border_width(10);

  // When the button receives the "clicked" signal, it will call the
  // on_button_clicked() method defined below.
  button_.signal_clicked().connect(sigc::mem_fun(*this, &UXWindow::on_button_clicked));

  // This packs the button into the Window (a container).
  add(button_);

  show_all();

}

UXWindow::~UXWindow() {
  app_.shutdown();
}

void UXWindow::on_button_clicked() {
  cout << "Hello World" << endl;
}

#endif

/*
 * Main function - uses boost to parse program arguments
 */

int main (int argc, const char * argv[]) {

  UxApp b;

#ifdef _SEBURO_OSX
  GLFWGTKApp a(b, 800, 600, argc, argv, "UX", 3, 2);
#else
  GLFWGTKApp a(b, 800, 600, argc, argv, "UX");
#endif

  UXWindow ux(b);

  return a.run(ux);

}