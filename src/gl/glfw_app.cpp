/**
* @brief GLFW Bit
* @file glfw_app.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 25/07/2012
*
*/

#include "gl/glfw_app.hpp"

using namespace s9;
using namespace s9::gl;

GLFWApp* GLFWApp::pThis;
VisualApp* GLFWApp::pApp;


GLFWApp::GLFWApp (VisualApp* app){
	if( !glfwInit(NULL) ){
		fprintf( stderr, "Failed to initialize GLFW\n" );
		exit( EXIT_FAILURE );
	}
	pApp = app;
	pThis = this;
	mFlag = 0x00;
}


void GLFWApp::mainLoop() {
	pThis->mRunning = true;

	while (pThis->mRunning){

		double_t t = glfwGetTime();

		BOOST_FOREACH ( GLFWwindow b, pThis->vWindows) {	
			glfwMakeContextCurrent(b);
			_display(b);
			glfwSwapBuffers();
		}

		pThis->mDX = glfwGetTime() - t;
		
		glfwPollEvents();
			
		
		BOOST_FOREACH ( GLFWwindow b, pThis->vWindows) {
			if (!glfwIsWindow(b))
				pThis->mRunning = false;
		}
    }

    // Exiting state
    glfwTerminate();
	exit( EXIT_SUCCESS );
}

/*
 * GLFW Callback for resizing a window
 */

void GLFWApp::_reshape(GLFWwindow window, int w, int h) {
	ResizeEvent e (w,h);
	pApp->fireEvent(e);
}

/*
 * GLFW display
 */

void GLFWApp::_display(GLFWwindow window) {
	pApp->display();
}

/*
 * GLFW Callback for the keyboard
 */


void GLFWApp::_keyCallback(GLFWwindow window, int key, int action) {
	KeyboardEvent e (key,action);
	pApp->fireEvent(e);
}

/*
 * GLFW Mouse button callback - sends a full event with current position as well
 */

void GLFWApp::_mouseButtonCallback(GLFWwindow window, int button, int action) {
	
	switch(button){
		case 0: {
			if (action){
				pThis->mFlag |= MOUSE_LEFT_DOWN;
				pThis->mFlag ^= MOUSE_LEFT_UP;
			}
			else{
				pThis->mFlag |= MOUSE_LEFT_UP;
				pThis->mFlag ^= MOUSE_LEFT_DOWN;
			}
			break;
		}
		case 1: {
			if (action){
				pThis->mFlag |= MOUSE_RIGHT_DOWN;
				pThis->mFlag ^= MOUSE_RIGHT_UP;
			}
			else{
				pThis->mFlag |= MOUSE_RIGHT_UP;
				pThis->mFlag ^= MOUSE_RIGHT_DOWN;
			}
			break;
		}
		case 3: {
			if (action) {
				pThis->mFlag |= MOUSE_MIDDLE_DOWN;
				pThis->mFlag ^= MOUSE_MIDDLE_UP;
			}
				
			else{
				pThis->mFlag |= MOUSE_MIDDLE_UP;
				pThis->mFlag ^= MOUSE_MIDDLE_DOWN;
			}
			break;
		}
	}

	MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag);
	pApp->fireEvent(e);
}


void GLFWApp::_mousePositionCallback(GLFWwindow window, int x, int y) {
	pThis->mMX = x;
	pThis->mMY = y;
	MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag);
	pApp->fireEvent(e);

}

void GLFWApp::_mouseWheelCallback(GLFWwindow window, int xpos, int ypos) {

	if (ypos == 1) {
		pThis->mFlag |= MOUSE_WHEEL_UP;
		pThis->mFlag ^= MOUSE_WHEEL_DOWN;
	}else {
		pThis->mFlag ^= MOUSE_WHEEL_UP;
		pThis->mFlag |= MOUSE_WHEEL_DOWN;
	}

	MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag);
	pApp->fireEvent(e);
	
	pThis->mFlag ^= MOUSE_WHEEL_UP;
	pThis->mFlag ^= MOUSE_WHEEL_DOWN;
	
}

void GLFWApp::_monitorCallback( GLFWmonitor m, int p) {

}



GLFWwindow GLFWApp::createWindow(const char * title ="S9Gear", size_t w=800, size_t h=600) {
	GLFWwindow win = glfwOpenWindow(w, h, GLFW_WINDOWED, title, NULL);
	if (!win){
		std::cerr << "Failed to open GLFW window: " << glfwErrorString(glfwGetError()) << std::endl;				
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowPos(win,100,100);
	
	return win;
}



/*
 * Perform OpenGL initialisation using GLEW
 */

 void GLFWApp::init(int major = 3, int minor = 2) {

	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, major);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, minor);
	
	///\todo fully switch to core profile - there is something causing an error in core
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	
	GLFWwindow w = createWindow();
	
	CXGLERROR
	
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	
	// Set Basic Callbacks
	glfwSetKeyCallback(_keyCallback);
	glfwSetMousePosCallback(_mousePositionCallback);
	glfwSetMouseButtonCallback(_mouseButtonCallback);
	glfwSetScrollCallback(_mouseWheelCallback);
	glfwSetWindowSizeCallback(_reshape);
	glfwSetMonitorDeviceCallback(_monitorCallback);

    glfwSwapInterval(1);
	

	if( !w ) {
		fprintf( stderr, "Failed to open GLFW window\n" );
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	
	// Call only after one window / context has been created!
	
	glewExperimental = true;
	GLenum err=glewInit();

	if(err!=GLEW_OK) {
		std::cout << "GLEWInit failed, aborting." << std::endl;
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	
	CXGLERROR
	pThis->vWindows.push_back(w);

	mainLoop();

}

