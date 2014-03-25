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

void UxApp::Init(Context context){
    shader_ = Shader( s9::File("./shaders/3/quad.vert"),  s9::File("./shaders/3/quad.frag"));

    cuboid_ = Cuboid(3.0,2.0,1.0);

    camera_= Camera( glm::vec3(0,0,10.0f));

    Spike s (4,1,1.0f,2.0f);

    node_.Add(cuboid_).Add(shader_).Add(camera_);

    top_node_2_.Add(s);
    top_node_2_.Add(camera_).Add(shader_);

    Node spike_node(s);

    spike_node.set_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,2.5f,0.0f)));

    node_.Add(spike_node);

    top_node_.Add(node_);
    top_node_.set_matrix(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f,0.0f,0.0f)));

    rotation_ = 0;

}


void UxApp::Update(double_t dt) {

}


/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void UxApp::Display(Context context, GLFWwindow* window, double_t dt){

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    rotation_ += 1.0;
   
    glm::mat4 Model = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0f, 1.0f, 0.0f));
    node_.set_matrix(Model);

    top_node_.Draw();
    top_node_2_.Draw();
   
    CXGLERROR
}


/*
 * This is called by the wrapper function when an event is fired
 */

void UxApp::ProcessEvent(MouseEvent e, GLFWwindow* window){
}

/*
 * Called when the window is resized. You should set cameras here
 */

void UxApp::ProcessEvent(ResizeEvent e, GLFWwindow* window){
    cout << "Window Resized:" << e.w << "," << e.h << endl;
    camera_.Resize(e.w,e.h);
}

void UxApp::ProcessEvent(KeyboardEvent e, GLFWwindow* window){
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
  // Need to signal that we are done here
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
  WithUXApp a(b,argc,argv,3,2);
#else
  WithUXApp a(b,argc,argv);
#endif

  a.CreateWindow("UX", 800, 600);

#ifdef _SEBURO_LINUX
  UXWindow ux(b);
  a.Run(ux);
#endif

  // Call shutdown once the GTK Run loop has quit. This makes GLFW quit cleanly
  a.Shutdown();

  return EXIT_SUCCESS;

}