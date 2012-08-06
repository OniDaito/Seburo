/**
* @brief GLFW Bit
* @file glfw_app.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 25/07/2012
*
*/

#include "s9/gl/glfw_app.hpp"

using namespace s9;
using namespace s9::gl;
using namespace std;

GLFWApp* GLFWApp::pThis;
VisualApp* GLFWApp::pApp;
string GLFWApp::mTitle;

GLFWApp::GLFWApp (VisualApp* app, int argc = 0, const char * argv[] = NULL, const char * title = "S9Gear"){
	if( !glfwInit() ){
		fprintf( stderr, "Failed to initialize GLFW\n" );
		exit( EXIT_FAILURE );
	}
	pApp = app;
	pThis = this;
	mFlag = 0x00;
	mTitle = title;    
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

#ifdef _GEAR_X11_GLX
		gtk_main_iteration_do(false);
#endif
		
 	}

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
	pApp->fireEvent(e);
}

/*
 * GLFW display
 */

void GLFWApp::_display(GLFWwindow window) {
	pApp->display(pThis->mDX);
	TwDraw();
}

/*
 * GLFW Callback for the keyboard
 */


void GLFWApp::_keyCallback(GLFWwindow window, int key, int action) {
	KeyboardEvent e (key,action,glfwGetTime());
	pApp->fireEvent(e);
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
					pApp->fireEvent(e);
				}
				else{
					pThis->mFlag |= MOUSE_LEFT_UP;
					pThis->mFlag ^= MOUSE_LEFT_DOWN;
					MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
					pApp->fireEvent(e);
					pThis->mFlag ^= MOUSE_LEFT_UP;
				}
				break;
			}
			case 1: {
				if (action){
					pThis->mFlag |= MOUSE_RIGHT_DOWN;
					pThis->mFlag ^= MOUSE_RIGHT_UP;
					MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
					pApp->fireEvent(e);
				}
				else{
					pThis->mFlag |= MOUSE_RIGHT_UP;
					pThis->mFlag ^= MOUSE_RIGHT_DOWN;
					MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
					pApp->fireEvent(e);
					pThis->mFlag ^= MOUSE_RIGHT_UP;
				}
				break;
			}
			case 2: {
				if (action) {
					pThis->mFlag |= MOUSE_MIDDLE_DOWN;
					pThis->mFlag ^= MOUSE_MIDDLE_UP;
					MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
					pApp->fireEvent(e);
				}
					
				else{

					pThis->mFlag |= MOUSE_MIDDLE_UP;
					pThis->mFlag ^= MOUSE_MIDDLE_DOWN;
					MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
					pApp->fireEvent(e);
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
		pApp->fireEvent(e);
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
		pApp->fireEvent(e);
		pThis->mFlag ^= MOUSE_WHEEL_UP;
		
	}else if (ypos == -1) {
		pThis->mFlag |= MOUSE_WHEEL_DOWN;
		MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
		pApp->fireEvent(e);
		pThis->mFlag ^= MOUSE_WHEEL_DOWN;
	}	
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
	
	GLFWwindow w = createWindow(mTitle.c_str());
	
	CXGLERROR
	
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	
	// Set Basic Callbacks
	glfwSetKeyCallback(_keyCallback);
	glfwSetCursorPosCallback(_mousePositionCallback);
	glfwSetMouseButtonCallback(_mouseButtonCallback);
	glfwSetScrollCallback(_mouseWheelCallback);
	glfwSetWindowSizeCallback(_reshape);
	glfwSetWindowCloseCallback( _window_close_callback );
		

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

	TwInit(TW_OPENGL, NULL);

	pApp->init();

	mainLoop();

}

