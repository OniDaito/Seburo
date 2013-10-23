/**
* @brief GLFW Bit
* @file glfwpp_->_app.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 25/07/2012
*
*/

#include "s9/gl/glfw_app.hpp"

using namespace s9;
using namespace s9::gl;
using namespace std;

GLFWApp* GLFWApp::pp_;
string GLFWApp::title_;


GLFWApp::GLFWApp (WindowApp &app, const int w, const int h, 
	bool fullscreen, int argc, const char * argv[], 
	const char * title, const int major, const int minor, const int depthbits) : WindowSystem(app) {
	
	if( !glfwInit() ){
		cerr << "SEBURO ERROR - Failed to initialize GLFW." << endl;
		exit( EXIT_FAILURE );
	}
	pp_ = this;
	flag_ = 0x00;
	title_ = title;
	initGL(w,h,major,minor,depthbits);

	pp_->_app.init();

	// Fire up the thread to keep update happy
	// Use a thread for the updates
 	pp_->update_thread_ =  new std::thread(&GLFWApp::_update);

	mainLoop();
}


void GLFWApp::mainLoop() {
	pp_->running_ = true;
	
	while (pp_->running_){

		double_t t = glfwGetTime();

		for ( GLFWwindow* b : pp_->windows_) {	
			glfwMakeContextCurrent(b);
			glfwSwapInterval( 1 );
			_display(b);
			glfwSwapBuffers(b);
		}

		pp_->dt_ = glfwGetTime() - t;
		
		glfwPollEvents();

#ifdef _SEBURO_LINUX
		//gtk_main_iteration_do(false);
#endif
		
 	}
 	_shutdown();
  // Exiting state
	glfwTerminate();
	
	// return to mainloop in the application for GLFW
}


/*
 * shutdown Callback for resizing a window
 */

void GLFWApp::_reshape(GLFWwindow* window, int w, int h) {
	ResizeEvent e (w,h,glfwGetTime());
	TwWindowSize(e.w, e.h);
	pp_->_app.fireEvent(e);
}


/*
 * GLFW Shutdown
 */

void GLFWApp::_shutdown() {
	pp_->running_ = false;
	if (pp_->update_thread_->joinable())
		pp_->update_thread_->join();
	pp_->_app.shutdown();
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
	if (!TwEventMouseButtonGLFW(button,action)){
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
	}
}


void GLFWApp::_mousePositionCallback(GLFWwindow* window, double x, double y){
	if( !TwEventMousePosGLFW(x, y) ){  
		pp_->mx_ = x;
		pp_->my_ = y;
		MouseEvent e (pp_->mx_,pp_->my_,pp_->flag_,glfwGetTime());
		pp_->_app.fireEvent(e);
	}

}

// TODO - Just the single window at present
void GLFWApp::_window_close_callback(GLFWwindow* window) {
	GLFWApp::_shutdown();
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

/*
 * Threaded update function. NO GL calls can be made from it
 */

void GLFWApp::_update(){
	while(pp_->running_)
		pp_->_app.update(pp_->dt_);
}



void GLFWApp::_error_callback(int error, const char* description) {
    std::cerr << "SEBURO - GLFW Error: " <<  error << " - " << description << std::endl;
}

/*
 * Perform OpenGL initialisation using GLEW
 * \todo FULLSCREEN apps
 * \todo pass params as a struct perhaps?
 */

 void GLFWApp::initGL( const int w = 800, const int h =600,
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

	TwInit(TW_OPENGL, NULL);
	TwWindowSize(w, h);

	pp_->dt_ = 0.0;
	
	CXGLERROR
	// fire a cheeky resize event to make sure all is well
	ResizeEvent e (w,h,glfwGetTime());
	pp_->_app.fireEvent(e);



}

