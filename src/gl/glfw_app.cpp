/**
* @brief GLFW Bit
* @file glfwpp_->_app.cpp
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


GLFWApp::GLFWApp (WindowApp &app, const size_t w, const size_t h, 
	bool fullscreen, int argc, const char * argv[], 
	const char * title, const int major, const int minor, const int depthbits, bool auto_start) : WindowSystem(app) {
	
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

	initGL(initial_width_, initial_height_, requested_major_, requested_minor_, requested_depth_bits_);

	pp_->_app.init();

	// Initial startup here - fire a resize event
	ResizeEvent e (initial_width_, initial_height_, glfwGetTime());
	pp_->_app.fireEvent(e);
	
	if (auto_start) {
		mainLoop();
	}
	
	
}

/// Run for one frame
void GLFWApp::run() {

	double_t t = glfwGetTime();

	for ( GLFWwindow* b : windows_) {	
		glfwMakeContextCurrent(b);
		glfwSwapInterval( 1 );
		_display(b);
		glfwSwapBuffers(b);
	}

	dt_ = glfwGetTime() - t;
	
	glfwPollEvents();
	
}

// Start the main loop

void GLFWApp::mainLoop() {
	running_ = true;

	while (running_){
		run();
 	}

 	shutdown();
}


/*
 * shutdown Callback for resizing a window
 */

void GLFWApp::_reshape(GLFWwindow* window, int w, int h) {
	ResizeEvent e (w,h,glfwGetTime());
	//TwWindowSize(e.w, e.h);
	pp_->_app.fireEvent(e);
}


/*
 * GLFW Shutdown
 */

void GLFWApp::shutdown() {
	running_ = false;
	
	
	_app.shutdown();

	glfwTerminate();
}

/*
 * GLFW display
 */

void GLFWApp::_display(GLFWwindow* window) {
	pp_->_app.display(pp_->dt_);
	//TwDraw();
	//CXGLERROR
}

void GLFWApp::_display(){
  pp_->_app.display(pp_->dt_);
  //TwDraw();
 //CXGLERROR - TwDraw keeps throwing out invalid operations. Not so good! ><
}


/*
 * GLFW Callback for the keyboard
 */

void GLFWApp::_scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	ScrollEvent e (xoffset,yoffset,glfwGetTime());
	pp_->_app.fireEvent(e);

}


/*
 * GLFW Callback for the keyboard
 */


void GLFWApp::_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	KeyboardEvent e (key,action,glfwGetTime());
	pp_->_app.fireEvent(e);
}

/*
 * GLFW Mouse button callback - sends a full event with current position as well
 */

void GLFWApp::_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	//if (!TwEventMouseButtonGLFW(button,action)){
		switch(button){
			case 0: {
				if (action){
					pp_->flag_ |= MOUSE_LEFT_DOWN;
					pp_->flag_ ^= MOUSE_LEFT_UP;
					MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
					pp_->_app.fireEvent(e);
				}
				else{
					pp_->flag_ |= MOUSE_LEFT_UP;
					pp_->flag_ ^= MOUSE_LEFT_DOWN;
					MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
					pp_->_app.fireEvent(e);
					pp_->flag_ ^= MOUSE_LEFT_UP;
				}
				break;
			}
			case 1: {
				if (action){
					pp_->flag_ |= MOUSE_RIGHT_DOWN;
					pp_->flag_ ^= MOUSE_RIGHT_UP;
					MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
					pp_->_app.fireEvent(e);
				}
				else{
					pp_->flag_ |= MOUSE_RIGHT_UP;
					pp_->flag_ ^= MOUSE_RIGHT_DOWN;
					MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
					pp_->_app.fireEvent(e);
					pp_->flag_ ^= MOUSE_RIGHT_UP;
				}
				break;
			}
			case 2: {
				if (action) {
					pp_->flag_ |= MOUSE_MIDDLE_DOWN;
					pp_->flag_ ^= MOUSE_MIDDLE_UP;
					MouseEvent e (pp_->mx_, pp_->my_, pp_->flag_,glfwGetTime());
					pp_->_app.fireEvent(e);
				}
					
				else{

					pp_->flag_ |= MOUSE_MIDDLE_UP;
					pp_->flag_ ^= MOUSE_MIDDLE_DOWN;
					MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
					pp_->_app.fireEvent(e);
					pp_->flag_ ^= MOUSE_MIDDLE_UP;
				}
				break;
			}
		}
	//}
}


void GLFWApp::_mousePositionCallback(GLFWwindow* window, double x, double y){
	//if( !TwEventMousePosGLFW(x, y) ){  
		pp_->mx_ = x;
		pp_->my_ = y;
		MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
		pp_->_app.fireEvent(e);
	//}

}

/// todo - Just the single window at present
void GLFWApp::_window_close_callback(GLFWwindow* window) {
	pp_->shutdown();
}


void GLFWApp::_mouseWheelCallback(GLFWwindow* window, double xpos, double ypos) {

	if (ypos == 1) {
		pp_->flag_ |= MOUSE_WHEEL_UP;	
		MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
		pp_->_app.fireEvent(e);
		pp_->flag_ ^= MOUSE_WHEEL_UP;
		
	} else if (ypos == -1) {
		pp_->flag_ |= MOUSE_WHEEL_DOWN;
		MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
		pp_->_app.fireEvent(e);
		pp_->flag_ ^= MOUSE_WHEEL_DOWN;
	}	
}


GLFWwindow* GLFWApp::createWindow(const char * title ="SEBURO", int w=800, int h=600) {

  GLFWwindow* win = glfwCreateWindow(w,h, title, NULL, NULL);

  if (!win){
		std::cerr << "SEBURO: Failed to open GLFW window" << std::endl;				
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowPos(win,100,100);
	
	return  win;
}


void GLFWApp::_error_callback(int error, const char* description) {
    std::cerr << "SEBURO - GLFW Error: " <<  error << " - " << description << std::endl;
}

/*
 * Perform OpenGL initialisation using GLEW
 * \todo FULLSCREEN apps
 * \todo pass params as a struct perhaps?
 */

 void GLFWApp::initGL( const size_t w = 800, const size_t h =600,
 		const int major = -1, const int minor = -1, const int depthbits = 16) {

 	if (major != -1 and minor != -1) {
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

 	glfwSetErrorCallback(_error_callback);

	GLFWwindow* window = createWindow(title_.c_str(),w, h);

	if( !window ) {
		std::cerr << "SEBURO Failed to open GLFW window\n" << std::endl;
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	CXGLERROR

	glfwSetFramebufferSizeCallback(window, _reshape);
	glfwMakeContextCurrent(window);
	glfwSwapInterval( 1 );

	CXGLERROR

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	// Set Basic Callbacks
	glfwSetKeyCallback(window, _keyCallback);
	glfwSetCursorPosCallback(window, _mousePositionCallback);
	glfwSetMouseButtonCallback(window, _mouseButtonCallback);
	glfwSetScrollCallback(window, _mouseWheelCallback);
	glfwSetWindowCloseCallback(window, _window_close_callback );
		
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

