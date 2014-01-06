/**
* @brief GLFW Bit
* @file glfwpp_->app_.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 25/07/2012
*
*/

#include "s9/gl/glfw_app.hpp"

#ifdef _SEBURO_LINUX
#include <gtkmm.h>
#endif

using namespace s9;
using namespace s9::gl;
using namespace std;

GLFWApp* GLFWApp::pp_;
string GLFWApp::title_;


///\todo potentially dont call the loop here as we might want to get a return value

GLFWApp::GLFWApp (WindowApp &app, const size_t w, const size_t h, 
	int argc, const char * argv[], 
	const char * title, const int major, const int minor, const int depthbits,
	const char* monitor_name) : WindowSystem(app) {
	
	if( !glfwInit() ){
		cerr << "SEBURO ERROR - Failed to initialize GLFW." << endl;
		exit( EXIT_FAILURE );
	}

	pp_ = this;
	flag_ = 0x00;
	title_ = title;
	requested_depth_bits_ = depthbits;
	initial_width_ = w;
	initial_height_ = h;
	requested_major_ = major;
	requested_minor_ = minor;
	monitor_name_ = monitor_name;

	// Grab Monitor Details and print them out
	int count = 0;
	GLFWmonitor **monitors =  glfwGetMonitors(&count);

	if (count != 0) {
		for (size_t i = 0; i < count; ++i)
			cout << "SEBURO GLFW Monitor Name: " << glfwGetMonitorName(monitors[i]) << endl;
	}

	initGL();

		// Call the application init method
	pp_->app_.init();

	// Initial startup here - fire a resize event
	ResizeEvent e (initial_width_, initial_height_, glfwGetTime());
	pp_->app_.fireEvent(e);

}

/// Call this to start the application Running
int GLFWApp::Run() {

	// Fire up the thread to keep update happy
	// Use a thread for the updates
	pp_->update_thread_ = new std::thread(&GLFWApp::Update);
	running_ = true;

	while (running_){
		MainLoop();
 	}

 	Shutdown();

 	return 1; ///\todo actually fix this please

}

bool GLFWApp::MainLoop() {
	double_t t = glfwGetTime();

	for ( GLFWwindow* b : windows_) {	
		glfwMakeContextCurrent(b);
		glfwSwapInterval( 1 );
		Display(b);
		glfwSwapBuffers(b);

	}

	// check to see if the app connected to this window is shutdown

	///\todo when we have multiple windows we need to decide how a Window App responds to that

	dt_ = glfwGetTime() - t;

	glfwPollEvents();

	return true;
}

/*
 * Shutdown Callback for resizing a window
 */

void GLFWApp::Reshape(GLFWwindow* window, int w, int h) {
	ResizeEvent e (w,h,glfwGetTime());
	//TwWindowSize(e.w, e.h);
	pp_->app_.fireEvent(e);
}


/*
 * GLFW Shutdown
 */

void GLFWApp::Shutdown() {
	running_ = false;
		
	app_.shutdown(); ///\tod move this to the window it is currently in I think.

	glfwTerminate();
}

/*
 * GLFW display
 */

void GLFWApp::Display(GLFWwindow* window) {
	pp_->app_.display(pp_->dt_);
	//TwDraw();
	//CXGLERROR
}

void GLFWApp::Display(){
  pp_->app_.display(pp_->dt_);
  //TwDraw();
 //CXGLERROR - TwDraw keeps throwing out invalid operations. Not so good! ><
}


/*
 * GLFW Callback for the keyboard
 */

void GLFWApp::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	ScrollEvent e (xoffset,yoffset,glfwGetTime());
	pp_->app_.fireEvent(e);

}


/*
 * GLFW Callback for the keyboard
 */


void GLFWApp::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	KeyboardEvent e (key,action,glfwGetTime());
	pp_->app_.fireEvent(e);
}

/*
 * GLFW Mouse button callback - sends a full event with current position as well
 */

void GLFWApp::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	//if (!TwEventMouseButtonGLFW(button,action)){
		switch(button){
			case 0: {
				if (action){
					pp_->flag_ |= MOUSE_LEFT_DOWN;
					pp_->flag_ ^= MOUSE_LEFT_UP;
					MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
					pp_->app_.fireEvent(e);
				}
				else{
					pp_->flag_ |= MOUSE_LEFT_UP;
					pp_->flag_ ^= MOUSE_LEFT_DOWN;
					MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
					pp_->app_.fireEvent(e);
					pp_->flag_ ^= MOUSE_LEFT_UP;
				}
				break;
			}
			case 1: {
				if (action){
					pp_->flag_ |= MOUSE_RIGHT_DOWN;
					pp_->flag_ ^= MOUSE_RIGHT_UP;
					MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
					pp_->app_.fireEvent(e);
				}
				else{
					pp_->flag_ |= MOUSE_RIGHT_UP;
					pp_->flag_ ^= MOUSE_RIGHT_DOWN;
					MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
					pp_->app_.fireEvent(e);
					pp_->flag_ ^= MOUSE_RIGHT_UP;
				}
				break;
			}
			case 2: {
				if (action) {
					pp_->flag_ |= MOUSE_MIDDLE_DOWN;
					pp_->flag_ ^= MOUSE_MIDDLE_UP;
					MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
					pp_->app_.fireEvent(e);
				}
					
				else{

					pp_->flag_ |= MOUSE_MIDDLE_UP;
					pp_->flag_ ^= MOUSE_MIDDLE_DOWN;
					MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
					pp_->app_.fireEvent(e);
					pp_->flag_ ^= MOUSE_MIDDLE_UP;
				}
				break;
			}
		}
	//}
}


void GLFWApp::MousePositionCallback(GLFWwindow* window, double x, double y){
	//if( !TwEventMousePosGLFW(x, y) ){  
		pp_->mx_ = x;
		pp_->my_ = y;
		MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
		pp_->app_.fireEvent(e);
	//}

}

/// todo - Just the single window at present
void GLFWApp::WindowCloseCallback(GLFWwindow* window) {
	pp_->app_.shutdown();
}


/*
* Threaded update function. NO GL calls can be made from it
*/

void GLFWApp::Update(){
  while(pp_->running_)
  	pp_->app_.update(pp_->dt_);
}



void GLFWApp::MouseWheelCallback(GLFWwindow* window, double xpos, double ypos) {

	if (ypos == 1) {
		pp_->flag_ |= MOUSE_WHEEL_UP;	
		MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
		pp_->app_.fireEvent(e);
		pp_->flag_ ^= MOUSE_WHEEL_UP;
		
	} else if (ypos == -1) {
		pp_->flag_ |= MOUSE_WHEEL_DOWN;
		MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
		pp_->app_.fireEvent(e);
		pp_->flag_ ^= MOUSE_WHEEL_DOWN;
	}	
}


GLFWwindow* GLFWApp::CreateWindow(const char * title ="SEBURO", int w=800, int h=600, const char * monitor_name) {

	GLFWwindow* window;

	///\todo potential to share resources with another window

	// Check if we are going fullscreen on one particular monitor
	if (monitor_name != nullptr){
		int count = 0;
		string s0 (monitor_name);
		GLFWmonitor ** monitors = glfwGetMonitors(&count);

		if (count != 0) {
			bool found = false;
	
			for (size_t i = 0; i < count; ++i){
				const char* mn = glfwGetMonitorName(monitors[i]);
				string s1 (mn);
				if (s1.compare(s0) == 0 ){
					found = true;
					window = glfwCreateWindow(w, h, title, monitors[i], NULL);
					break;
				}
			}

			if (!found){
				std::cerr << "SEBURO: No Monitors found (that match the given name)!" << std::endl;				
				glfwTerminate();
				exit(EXIT_FAILURE);
			}

		} else {
			std::cerr << "SEBURO: No Monitors found!" << std::endl;				
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

	} else if (w == 0 || h == 0) {
		// Go fullscreen on the primary monitor
		GLFWmonitor * m = glfwGetPrimaryMonitor();
		const GLFWvidmode * mode = glfwGetVideoMode (m);
		
		w = mode->width;
		h = mode->height;

		window = glfwCreateWindow(w, h, title, m, NULL);

	} else {
		window = glfwCreateWindow(w,h, title, NULL, NULL);
		glfwSetWindowPos(window,100,100);
	}

  if (!window){
		std::cerr << "SEBURO: Failed to open GLFW window." << std::endl;				
		glfwTerminate();
		exit(EXIT_FAILURE);
	}


	CXGLERROR

	glfwSetFramebufferSizeCallback(window, Reshape);
	glfwMakeContextCurrent(window);
	glfwSwapInterval( 1 );

	CXGLERROR

	cout << "SEBURO OpenGL Version: " << glGetString(GL_VERSION) << endl;

	// Set Basic Callbacks
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MousePositionCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetScrollCallback(window, MouseWheelCallback);
	glfwSetWindowCloseCallback(window, WindowCloseCallback );
		
	return  window;
}


void GLFWApp::ErrorCallback(int error, const char* description) {
    std::cerr << "SEBURO - GLFW Error: " <<  error << " - " << description << std::endl;
}

/*
 * Perform OpenGL initialisation using GLEW
 * \todo FULLSCREEN apps
 * \todo pass params as a struct perhaps?
 */

 void GLFWApp::initGL() {

 	if (pp_->requested_major_ != -1 || pp_->requested_minor_ != -1) {
 		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, pp_->requested_major_);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, pp_->requested_minor_);
 	}

 	glfwWindowHint(GLFW_DEPTH_BITS, pp_->requested_depth_bits_);

#ifdef _SEBURO_OSX
 	// Forward compatible and CORE Profile
 	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
 	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	//glfwWindowHint(GLFW_FSAA_SAMPLES, 4);

 	glfwSetErrorCallback(ErrorCallback);

	GLFWwindow* window = CreateWindow(pp_->title_.c_str(), pp_->initial_width_, pp_->initial_height_, pp_->monitor_name_);


	// Call only after one window / context has been created!
	
	glewExperimental = true;
	GLenum err=glewInit();

	// Problem here - see http://www.opengl.org/wiki/OpenGL_Loading_Library
	CXGLERROR

	if(err!=GLEW_OK) {
		std::cerr << "SEBURO: GLEW init failed! Aborting with error: " << err << std::endl;
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	CXGLERROR

	pp_->windows_.push_back(window);

	//TwInit(TW_OPENGL, NULL);
	//TwWindowSize(w, h);

	pp_->dt_ = 0.0;
	
	CXGLERROR

}

#ifdef _SEBURO_LINUX

/**
 *	The GTK GLFW Class that allows a second GTK Control panel under Linux
 * 
 */

GLFWGTKApp::GLFWGTKApp (WindowApp &app, const size_t w, const size_t h, 
	int argc, const char* argv[], 
	const char* title, const int major, const int minor, const int depthbits,
	const char* monitor_name) : GLFWApp(app,w,h,argc,argv,title,major,minor,depthbits, monitor_name) {

	char ** casted =  new char* [argc];

  for (size_t i =0; i < argc; ++i){
    casted[i] = new char[strlen(argv[i])];
    strcpy(casted[i],argv[i]);
  }
		
  gtk_init(&argc, &casted);
	gtk_app_ = Gtk::Application::create(argc, casted, "uk.co.section9.seburo");
	idle_connection_ = Glib::signal_idle().connect( sigc::mem_fun(*this, &GLFWGTKApp::MainLoop) );

}

/// Overridden Run method that takes a reference to a GTK window in order to get Running
int GLFWGTKApp::Run(Gtk::Window &window) {

	update_thread_ = new std::thread(&GLFWApp::Update);
	running_ = true;
	return gtk_app_->run(window);
}

void GLFWGTKApp::Shutdown() {
	running_ = false;
	
	// Disconnect signals
	idle_connection_.disconnect();

	app_.shutdown();
	glfwTerminate();	
	gtk_app_->quit();

}


#endif