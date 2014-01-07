/**
* @brief GLFW Bit
* @file glfwpp_->app_.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 25/07/2012
*
*/

#include "s9/gl/glfw.hpp"


using namespace s9;
using namespace s9::gl;
using namespace std;

GLFWApp* GLFWApp::pp_;

GLFWApp::GLFWApp (WindowApp<GLFWwindow*> &app, const int major, const int minor) : app_(app) {
	
	if( !glfwInit() ){
		cerr << "SEBURO ERROR - Failed to initialize GLFW." << endl;
		exit( EXIT_FAILURE );
	}

	pp_ = this;
	flag_ = 0;

	// Add this app to the Eventor so it can listen for windows events
	eventor_.AddWindowListener(&app_);

	// Grab Monitor Details and print them out
	int count = 0;
	GLFWmonitor **monitors =  glfwGetMonitors(&count);

	if (count != 0) {
		for (size_t i = 0; i < count; ++i)
			cout << "SEBURO GLFW Monitor Name: " << glfwGetMonitorName(monitors[i]) << endl;
	}

	InitGL(major, minor);

}

/// Call this to start the application Running
void GLFWApp::Run() {

	// Cx to see if a window has been added. If not, create a default one
	if (windows_.size() == 0){
		CreateWindow("SEBURO", 800, 600);
	}


	// Call the application init method
	app_.Init();

	// Fire off resize events per window
	for ( GLFWwindow* b : windows_) {	
		glfwMakeContextCurrent(b);
		int w,h;
		glfwGetWindowSize(b,&w,&h);
		ResizeEvent e (w, h, glfwGetTime());
		eventor_.FireEvent(e,b);
	}

	// Fire up the thread to keep update happy
	// Use a thread for the updates
	update_thread_ = new std::thread(&GLFWApp::UpdateThread);
	running_ = true;
	dt_ = 0.0;

	while (running_){
		MainLoop();
 	}

 	Shutdown();
 	///\todo potentially give a return value?
}

bool GLFWApp::MainLoop() {
	double_t t = glfwGetTime();

	for ( GLFWwindow* b : windows_) {	
		glfwMakeContextCurrent(b);
		glfwSwapInterval( 1 );
		app_.Display(b, t);
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
	pp_->eventor_.FireEvent(e,window);
}


/*
 * GLFW Shutdown
 */

void GLFWApp::Shutdown() {
	running_ = false;
	glfwTerminate();
}

/*
 * GLFW display
 */

void GLFWApp::Display(GLFWwindow* window) {
	pp_->app_.Display(window, pp_->dt_);
	CXGLERROR
}



/*
 * GLFW Callback for the keyboard
 */

void GLFWApp::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	ScrollEvent e (xoffset,yoffset,glfwGetTime());
	pp_->eventor_.FireEvent(e,window);
}

/*
 * GLFW Callback for the keyboard
 */

void GLFWApp::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	KeyboardEvent e (key,action,glfwGetTime());
	pp_->eventor_.FireEvent(e,window);
}

/*
 * GLFW Mouse button callback - sends a full event with current position as well
 */

void GLFWApp::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	switch(button){
		case 0: {
			if (action){
				pp_->flag_ |= MOUSE_LEFT_DOWN;
				pp_->flag_ ^= MOUSE_LEFT_UP;
				MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
				pp_->eventor_.FireEvent(e,window);
			}
			else{
				pp_->flag_ |= MOUSE_LEFT_UP;
				pp_->flag_ ^= MOUSE_LEFT_DOWN;
				MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
				pp_->eventor_.FireEvent(e,window);
				pp_->flag_ ^= MOUSE_LEFT_UP;
			}
			break;
		}
		case 1: {
			if (action){
				pp_->flag_ |= MOUSE_RIGHT_DOWN;
				pp_->flag_ ^= MOUSE_RIGHT_UP;
				MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
				pp_->eventor_.FireEvent(e,window);
			}
			else{
				pp_->flag_ |= MOUSE_RIGHT_UP;
				pp_->flag_ ^= MOUSE_RIGHT_DOWN;
				MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
				pp_->eventor_.FireEvent(e,window);
				pp_->flag_ ^= MOUSE_RIGHT_UP;
			}
			break;
		}
		case 2: {
			if (action) {
				pp_->flag_ |= MOUSE_MIDDLE_DOWN;
				pp_->flag_ ^= MOUSE_MIDDLE_UP;
				MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
				pp_->eventor_.FireEvent(e,window);
			}
				
			else{
				pp_->flag_ |= MOUSE_MIDDLE_UP;
				pp_->flag_ ^= MOUSE_MIDDLE_DOWN;
				MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
				pp_->eventor_.FireEvent(e,window);
				pp_->flag_ ^= MOUSE_MIDDLE_UP;
			}
			break;
		}
	}
	
}


void GLFWApp::MousePositionCallback(GLFWwindow* window, double x, double y){
	pp_->mx_ = x;
	pp_->my_ = y;
	MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
	pp_->eventor_.FireEvent(e,window);
}

void GLFWApp::WindowCloseCallback(GLFWwindow* window) {
	CloseWindowEvent e (glfwGetTime());
	pp_->eventor_.FireEvent(e,window);

	// Remove the window
	for (std::vector<GLFWwindow*>::iterator it = pp_->windows_.begin(); it != pp_->windows_.end(); ) {
		if  (*it == window){
			pp_->windows_.erase(it);
		} else {
			++it;
		}
	}

	// Default behavior - if there are no GLFW windows - quit
	if (pp_->windows_.size() == 0) {
		pp_->Shutdown();
	}

}


void GLFWApp::CloseWindow(GLFWwindow* window) {
	WindowCloseCallback(window);
}

/**
 * Threaded update function. NO GL calls can be made from it
 * \todo which dt should we send here? We need to be careful with clocks
 */

void GLFWApp::UpdateThread(){
  while(pp_->running_)
  	pp_->app_.Update(pp_->dt_);
}



void GLFWApp::MouseWheelCallback(GLFWwindow* window, double xpos, double ypos) {

	if (ypos == 1) {
		pp_->flag_ |= MOUSE_WHEEL_UP;	
		MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
		pp_->eventor_.FireEvent(e,window);
		pp_->flag_ ^= MOUSE_WHEEL_UP;
		
	} else if (ypos == -1) {
		pp_->flag_ |= MOUSE_WHEEL_DOWN;
		MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
		pp_->eventor_.FireEvent(e,window);
		pp_->flag_ ^= MOUSE_WHEEL_DOWN;
	}	
}

/// Create a window that has a size greater than 0, with a title
GLFWwindow* GLFWApp::CreateWindow(const char * title ="SEBURO", size_t width =800, size_t height =600) {

	GLFWwindow* window;

	///\todo potential to share resources with another window

	if (width == 0 || height == 0) {
		std::cerr << "SEBURO: Cannot have width or height as 0 when creating a GLFW Window." << std::endl;				
		glfwTerminate();
		exit(EXIT_FAILURE);

	} else {
		window = glfwCreateWindow(width, height, title, NULL, NULL);
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
		
	windows_.push_back(window);
	InitGLEW(window);

	return  window;
}


/**
 * Create a fullscreen window
 * \param title - the title of the window
 * \param width - the width of the window in pixels - a 0 means use native width
 * \param height - the height of the window in pixels  - a 0 means use native height
 * \param monitor - the monitor name to go fullscreen on. nullptr/default means the main screen
 */

GLFWwindow* GLFWApp::CreateWindowFullScreen(const char * title ="SEBURO",  size_t width = 0, size_t height = 0, const char * monitor_name = nullptr ) {

	GLFWwindow* window;

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
					if (width == 0 || height == 0) {
						GLFWmonitor * m = glfwGetPrimaryMonitor();
						const GLFWvidmode * mode = glfwGetVideoMode (m);
						width = mode->width; 
						height = mode->height;
					}

					window = glfwCreateWindow(width, height, title, monitors[i], NULL);
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

	} else if (width == 0 || height == 0) {
		// Go fullscreen on the primary monitor
		GLFWmonitor * m = glfwGetPrimaryMonitor();
		const GLFWvidmode * mode = glfwGetVideoMode (m);
		
		window = glfwCreateWindow(mode->width, mode->height, title, m, NULL);

	} else {
		window = glfwCreateWindow(width, height, title, NULL, NULL);
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

	windows_.push_back(window);
	InitGLEW(window);

	return window;
}

void GLFWApp::InitGLEW(GLFWwindow* window) {
	
	glfwMakeContextCurrent(window);
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

}


void GLFWApp::ErrorCallback(int error, const char* description) {
    std::cerr << "SEBURO - GLFW Error: " <<  error << " - " << description << std::endl;
}

/*
 * Perform OpenGL initialisation
 * \todo pass params as a struct perhaps? We will no-doubt need more in the future
 */

void GLFWApp::InitGL(int major, int minor, int depthbits) {

 	if (major != -1 || minor != -1) {
 		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
 	}

 	glfwWindowHint(GLFW_DEPTH_BITS, depthbits);

#ifdef _SEBURO_OSX
 	// Forward compatible and CORE Profile
 	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
 	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	//glfwWindowHint(GLFW_FSAA_SAMPLES, 4);

 	glfwSetErrorCallback(ErrorCallback);


}


WithUXApp::WithUXApp(WindowApp<GLFWwindow*> &app, int argc, const char * argv[], 
	const int major, const int minor) : GLFWApp(app,major,minor) {


#ifdef _SEBURO_LINUX
	char ** casted =  new char* [argc];

  for (size_t i =0; i < argc; ++i){
    casted[i] = new char[strlen(argv[i])];
    strcpy(casted[i],argv[i]);
  }
		
  gtk_init(&argc, &casted);

	gtk_app_ = Gtk::Application::create(argc, casted, "uk.co.section9.seburo");
	idle_connection_ = Glib::signal_idle().connect( sigc::mem_fun(*this, &WithUXApp::MainLoop) );

#endif

}


#ifdef _SEBURO_LINUX
void WithUXApp::Run(Gtk::Window &window) {
	// Cx to see if a window has been added. If not, create a default one
	if (windows_.size() == 0){
		CreateWindow("SEBURO", 800, 600);
	}

	
	// Call the application init method
	app_.Init();

	// Fire off resize events per window
	for ( GLFWwindow* b : windows_) {	
		glfwMakeContextCurrent(b);
		int w,h;
		glfwGetWindowSize(b,&w,&h);
		ResizeEvent e (w, h, glfwGetTime());
		eventor_.FireEvent(e,b);
	}

	update_thread_ = new std::thread(&GLFWApp::UpdateThread);
	running_ = true;
	gtk_app_->run(window);

}
#endif

void WithUXApp::Shutdown() {
	running_ = false;
	
	// Disconnect signals
	idle_connection_.disconnect();

	glfwTerminate();	
	gtk_app_->quit();

}