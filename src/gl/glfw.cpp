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

/// Internal GLWindow function that creates our actual window using GLFW
void GLWindow::Create(const char * title, size_t width, size_t height, bool fullscreen, const char * monitor_name, int major, int minor, GLFWwindow *share) {

	window_name_ = std::string(title);

	// InitGL and setup Context
	int depthbits = 16;

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

					glfw_window_ = glfwCreateWindow(width, height, title, monitors[i], share);
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
		
		glfw_window_ = glfwCreateWindow(mode->width, mode->height, title, m, share);

	} else {
		glfw_window_ = glfwCreateWindow(width, height, title, NULL, share);
		glfwSetWindowPos(glfw_window_,100,100);
	}

  if (!glfw_window_){
		std::cerr << "SEBURO: Failed to open GLFW window." << std::endl;				
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	CXGLERROR

	glfwMakeContextCurrent(glfw_window_);
	glfwSwapInterval( 1 );

	CXGLERROR

	cout << "SEBURO OpenGL Version: " << glGetString(GL_VERSION) << endl;

	// Set Basic Callbacks - Checking for AntTweakBar overridding


	glfwSetKeyCallback 						( glfw_window_, GLFWWindowManager::KeyCallback 						);
	glfwSetCursorPosCallback			( glfw_window_, GLFWWindowManager::MousePositionCallback 	);
	glfwSetMouseButtonCallback		(	glfw_window_, GLFWWindowManager::MouseButtonCallback 		);
	glfwSetScrollCallback					( glfw_window_, GLFWWindowManager::MouseWheelCallback 		);
	glfwSetWindowCloseCallback		(	glfw_window_, GLFWWindowManager::WindowCloseCallback  	);
	glfwSetFramebufferSizeCallback( glfw_window_, GLFWWindowManager::Reshape								);

	// Init GLEW for this context

	glfwMakeContextCurrent(glfw_window_);
	// Call only after one window / context has been created!
	glewExperimental = true;
	GLenum err = glewInit();

	// Problem here - see http://www.opengl.org/wiki/OpenGL_Loading_Library
	CXGLERROR

	if(err!=GLEW_OK) {
		std::cerr << "SEBURO: GLEW init failed! Aborting with error: " << err << std::endl;
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	CXGLERROR
}

GLFWWindowManager* GLFWWindowManager::pp_; // Static pointer to the current active manager

GLFWWindowManager::GLFWWindowManager () {
	
	if( !glfwInit() ){
		cerr << "SEBURO ERROR - Failed to initialize GLFW." << endl;
		exit( EXIT_FAILURE );
	}

#ifdef USE_ANTTWEAKBAR
  TwInit(TW_OPENGL, NULL);
#endif


	pp_ = this;
	flag_ = 0;
	
	// Grab Monitor Details and print them out
	int count = 0;
	GLFWmonitor **monitors =  glfwGetMonitors(&count);

	if (count != 0) {
		for (size_t i = 0; i < count; ++i)
			cout << "SEBURO GLFW Monitor Name: " << glfwGetMonitorName(monitors[i]) << endl;
	}


 	glfwSetErrorCallback(ErrorCallback);


}

/// Call this to start the application Running
/*
void GLFWWindowManager::Run() {

	// Cx to see if a window has been added. If not, create a default one
	if (windows_.size() == 0){
		CreateWindow("SEBURO", 800, 600);
	}


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



 	Shutdown();
 	///\todo potentially give a return value?
	glfwTerminate();
}*/


/*
 * Shutdown Callback for resizing a window
 */

void GLFWWindowManager::Reshape(GLFWwindow* window, int w, int h) {
	ResizeEvent e (w,h,glfwGetTime());
	pp_->FireEvent(pp_->GetWindow(window), e);
}


/*
 * GLFW Shutdow
 */

void GLFWWindowManager::Shutdown() {

#ifdef USE_ANTTWEAKBAR
  TwTerminate();
#endif

	glfwTerminate();

}


/*
 * GLFW Callback for the keyboard
 */

void GLFWWindowManager::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

	ScrollEvent e (xoffset,yoffset,glfwGetTime());
	pp_->FireEvent(pp_->GetWindow(window),e);
}

/*
 * GLFW Callback for the keyboard
 */

void GLFWWindowManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

#ifdef USE_ANTTWEAKBAR
	if( TwEventKeyGLFW(key, action ))
		return;
#endif

	KeyboardEvent e (key,action,glfwGetTime());
	pp_->FireEvent(pp_->GetWindow(window), e);
}

/*
 * GLFW Mouse button callback - sends a full event with current position as well
 */

void GLFWWindowManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

#ifdef USE_ANTTWEAKBAR
	if( TwEventMouseButtonGLFW(button, action) !=0 )
		return;
#endif

	switch(button){
		case 0: {
			if (action){
				pp_->flag_ |= MOUSE_LEFT_DOWN;
				pp_->flag_ ^= MOUSE_LEFT_UP;
				MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
				pp_->FireEvent(pp_->GetWindow(window), e);
			}
			else{
				pp_->flag_ |= MOUSE_LEFT_UP;
				pp_->flag_ ^= MOUSE_LEFT_DOWN;
				MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
				pp_->FireEvent(pp_->GetWindow(window),e);
				pp_->flag_ ^= MOUSE_LEFT_UP;
			}
			break;
		}
		case 1: {
			if (action){
				pp_->flag_ |= MOUSE_RIGHT_DOWN;
				pp_->flag_ ^= MOUSE_RIGHT_UP;
				MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
				pp_->FireEvent(pp_->GetWindow(window),e);
			}
			else{
				pp_->flag_ |= MOUSE_RIGHT_UP;
				pp_->flag_ ^= MOUSE_RIGHT_DOWN;
				MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
				pp_->FireEvent(pp_->GetWindow(window), e);
				pp_->flag_ ^= MOUSE_RIGHT_UP;
			}
			break;
		}
		case 2: {
			if (action) {
				pp_->flag_ |= MOUSE_MIDDLE_DOWN;
				pp_->flag_ ^= MOUSE_MIDDLE_UP;
				MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
				pp_->FireEvent(pp_->GetWindow(window), e);
			}
				
			else{
				pp_->flag_ |= MOUSE_MIDDLE_UP;
				pp_->flag_ ^= MOUSE_MIDDLE_DOWN;
				MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
				pp_->FireEvent(pp_->GetWindow(window), e);
				pp_->flag_ ^= MOUSE_MIDDLE_UP;
			}
			break;
		}
	}
	
}


void GLFWWindowManager::MousePositionCallback(GLFWwindow* window, double x, double y){

#ifdef USE_ANTTWEAKBAR
	if( TwEventMousePosGLFW(x, y) !=0 )
		return;
#endif

	pp_->mx_ = x;
	pp_->my_ = y;
	MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
	pp_->FireEvent(pp_->GetWindow(window),e);
}

void GLFWWindowManager::WindowCloseCallback(GLFWwindow* window) {
	
	CloseWindowEvent e (glfwGetTime());
	pp_->FireEvent(pp_->GetWindow(window), e);
	
	glfwDestroyWindow(window);

	pp_->DestroyWindow(pp_->GetWindow(window));
	
}


/**
 * Threaded update function. NO GL calls can be made from it. Calls the application update passing time
 * elapsed in seconds
 */


void GLFWWindowManager::MouseWheelCallback(GLFWwindow* window, double xpos, double ypos) {

#ifdef USE_ANTTWEAKBAR
	if( TwEventMouseWheelGLFW(static_cast<int>(ypos)) !=0 )
		return;
#endif


	if (ypos == 1) {
		pp_->flag_ |= MOUSE_WHEEL_UP;	
		MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
		pp_->FireEvent(pp_->GetWindow(window),e);
		pp_->flag_ ^= MOUSE_WHEEL_UP;
		
	} else if (ypos == -1) {
		pp_->flag_ |= MOUSE_WHEEL_DOWN;
		MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
		pp_->FireEvent(pp_->GetWindow(window),e);
		pp_->flag_ ^= MOUSE_WHEEL_DOWN;
	}	
}


/**
 * Create a fullscreen window
 * \param title - the title of the window
 * \param width - the width of the window in pixels - a 0 means use native width
 * \param height - the height of the window in pixels  - a 0 means use native height
 * \param monitor - the monitor name to go fullscreen on. nullptr/default means the main screen
 */
void GLFWWindowManager::ErrorCallback(int error, const char* description) {
    std::cerr << "SEBURO - GLFW Error: " <<  error << " - " << description << std::endl;
}


