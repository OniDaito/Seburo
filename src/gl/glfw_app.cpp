/**
* @brief GLFW Bit
* @file glfwpThis->_app.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 25/07/2012
*
*/

#include "s9/gl/glfw_app.hpp"

using namespace s9;
using namespace s9::gl;
using namespace std;

GLFWApp* GLFWApp::pThis;
string GLFWApp::mTitle;


void GLFWApp::mainLoop() {
	pThis->mRunning = true;

	while (pThis->mRunning){

		double_t t = glfwGetTime();

		BOOST_FOREACH ( GLFWwindow b, pThis->vWindows) {	
			glfwMakeContextCurrent(b);
			_display(b);
			glfwSwapBuffers(b);
		}

		pThis->_dt = glfwGetTime() - t;
		
		glfwPollEvents();

#ifdef _GEAR_X11_GLX
		gtk_main_iteration_do(false);
#endif
		
 	}
 	_shutdown();
  // Exiting state
	glfwTerminate();
	exit( EXIT_SUCCESS );
}


/*
 * GLFW Callback for resizing a window
 */

void GLFWApp::_reshape(GLFWwindow window, int w, int h) {
	ResizeEvent e (w,h,glfwGetTime());
	TwWindowSize(e.mW, e.mH);
	pThis->_app.fireEvent(e);
}


/*
 * GLFW Shutdown
 */

void GLFWApp::_shutdown() {
	pThis->_app.shutdown();
}

/*
 * GLFW display
 */

void GLFWApp::_display(GLFWwindow window) {
	pThis->_app.display(pThis->_dt);
	TwDraw();
}

/*
 * GLFW Callback for the keyboard
 */


void GLFWApp::_keyCallback(GLFWwindow window, int key, int action) {
	KeyboardEvent e (key,action,glfwGetTime());
	pThis->_app.fireEvent(e);
}

/*
 * GLFW Mouse button callback - sends a full event with current position as well
 */

void GLFWApp::_mouseButtonCallback(GLFWwindow window, int button, int action) {
	if (!TwEventMouseButtonGLFW(button,action)){
		switch(button){
			case 0: {
				if (action){
					pThis->mFlag |= MOUSE_LEFT_DOWN;
					pThis->mFlag ^= MOUSE_LEFT_UP;
					MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
					pThis->_app.fireEvent(e);
				}
				else{
					pThis->mFlag |= MOUSE_LEFT_UP;
					pThis->mFlag ^= MOUSE_LEFT_DOWN;
					MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
					pThis->_app.fireEvent(e);
					pThis->mFlag ^= MOUSE_LEFT_UP;
				}
				break;
			}
			case 1: {
				if (action){
					pThis->mFlag |= MOUSE_RIGHT_DOWN;
					pThis->mFlag ^= MOUSE_RIGHT_UP;
					MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
					pThis->_app.fireEvent(e);
				}
				else{
					pThis->mFlag |= MOUSE_RIGHT_UP;
					pThis->mFlag ^= MOUSE_RIGHT_DOWN;
					MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
					pThis->_app.fireEvent(e);
					pThis->mFlag ^= MOUSE_RIGHT_UP;
				}
				break;
			}
			case 2: {
				if (action) {
					pThis->mFlag |= MOUSE_MIDDLE_DOWN;
					pThis->mFlag ^= MOUSE_MIDDLE_UP;
					MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
					pThis->_app.fireEvent(e);
				}
					
				else{

					pThis->mFlag |= MOUSE_MIDDLE_UP;
					pThis->mFlag ^= MOUSE_MIDDLE_DOWN;
					MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
					pThis->_app.fireEvent(e);
					pThis->mFlag ^= MOUSE_MIDDLE_UP;
				}
				break;
			}
		}
	}
}


void GLFWApp::_mousePositionCallback(GLFWwindow window, int x, int y) {
	if( !TwEventMousePosGLFW(x, y) ){  
		pThis->mMX = x;
		pThis->mMY = y;
		MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
		pThis->_app.fireEvent(e);
	}

}

int GLFWApp::_window_close_callback(GLFWwindow window) {
	pThis->mRunning = GL_FALSE;
	return GL_TRUE;
}


void GLFWApp::_mouseWheelCallback(GLFWwindow window, double xpos, double ypos) {

	if (ypos == 1) {
		pThis->mFlag |= MOUSE_WHEEL_UP;	
		MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
		pThis->_app.fireEvent(e);
		pThis->mFlag ^= MOUSE_WHEEL_UP;
		
	}else if (ypos == -1) {
		pThis->mFlag |= MOUSE_WHEEL_DOWN;
		MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
		pThis->_app.fireEvent(e);
		pThis->mFlag ^= MOUSE_WHEEL_DOWN;
	}	
}


GLFWwindow GLFWApp::createWindow(const char * title ="S9Gear", int w=800, int h=600) {
	GLFWwindow win = glfwCreateWindow(800,600, GLFW_WINDOWED, title, NULL);
	if (!win){
		std::cout << "Failed to open GLFW window: " << glfwErrorString(glfwGetError()) << std::endl;				
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowPos(win,100,100);
	
	return win;
}

/*
 * Threaded update function. NO GL calls can be made from it
 */

void GLFWApp::_update(){
	while(pThis->mRunning)
		pThis->_app.update(pThis->_dt);
}



/*
 * Perform OpenGL initialisation using GLEW
 * \todo FULLSCREEN apps
 */

 void GLFWApp::initGL( const int w = 800, const int h =600,
 		const int major = 3, const int minor = 3) {

	//glfwWindowHint(GLFW_OPENGL_VERSION_MAJOR, major);
	//glfwWindowHint(GLFW_OPENGL_VERSION_MINOR, minor);
	
	///\todo fully switch to core profile - there is something causing an error in core
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


 	glfwWindowHint(GLFW_DEPTH_BITS, 16);
	glfwWindowHint(GLFW_FSAA_SAMPLES, 4);

	GLFWwindow window = createWindow(mTitle.c_str(),w, h);

	glfwSetWindowSizeCallback(_reshape);
	glfwMakeContextCurrent(window);
	
	CXGLERROR
	
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	// Set Basic Callbacks
	glfwSetKeyCallback(_keyCallback);
	glfwSetCursorPosCallback(_mousePositionCallback);
	glfwSetMouseButtonCallback(_mouseButtonCallback);
	glfwSetScrollCallback(_mouseWheelCallback);
	
	glfwSetWindowCloseCallback( _window_close_callback );
		
  glfwSwapInterval(1);
	CXGLERROR

	if( !window ) {
		fprintf( stderr, "Failed to open GLFW window\n" );
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	CXGLERROR
	// Call only after one window / context has been created!
	
	glewExperimental = true;
	GLenum err=glewInit();

	if(err!=GLEW_OK) {
		std::cout << "GLEWInit failed, aborting with error: " << err << std::endl;
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	
	CXGLERROR
	pThis->vWindows.push_back(window);

	TwInit(TW_OPENGL, NULL);
	TwWindowSize(w, h);

	pThis->_app.init();

	// fire a cheeky resize event to make sure all is well
	ResizeEvent e (w,h,glfwGetTime());
	pThis->_app.fireEvent(e);

	// Fire up the thread to keep update happy

	// Use a thread for the updates
  pThis->_update_thread =  new boost::thread(&GLFWApp::_update);



	mainLoop();

}

