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


#if defined(_SEBURO_BUILD_DLL)

// SEBURO DLL entry point
//
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
    return TRUE;
}

#endif // _SEBURO_BUILD_DLL

GLFWApp::GLFWApp (WindowApp &app, const int w, const int h, 
	bool fullscreen, int argc, const char * argv[], 
	const char * title, const int major, const int minor, const int depthbits) : WindowSystem(app) {
	
	if( !glfwInit() ){
		fprintf( stderr, "Failed to initialize GLFW\n" );
		exit( EXIT_FAILURE );
	}
	pThis = this;
	mFlag = 0x00;
	mTitle = title;
	initGL(w,h,major,minor,depthbits);
}


void GLFWApp::mainLoop() {
	pThis->mRunning = true;

	while (pThis->mRunning){

		double_t t = glfwGetTime();

		BOOST_FOREACH ( GLFWwindow* b, pThis->vWindows) {	
			glfwMakeContextCurrent(b);
			glfwSwapInterval( 1 );
			_display(b);
			glfwSwapBuffers(b);
		}

		pThis->_dt = glfwGetTime() - t;
		
		glfwPollEvents();

#ifdef _SEBURO_X11_GLX
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

void GLFWApp::_reshape(GLFWwindow* window, int w, int h) {
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

void GLFWApp::_display(GLFWwindow* window) {
	pThis->_app.display(pThis->_dt);
	//TwDraw();
	//CXGLERROR
}

void GLFWApp::_display(){
  pThis->_app.display(pThis->_dt);
  //TwDraw();
 	//CXGLERROR - TwDraw keeps throwing out invalid operations. Not so good! ><
}


/*
 * GLFW Callback for the keyboard
 */

void GLFWApp::_scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	ScrollEvent e (xoffset,yoffset,glfwGetTime());
	pThis->_app.fireEvent(e);

}



/*
 * GLFW Callback for the keyboard
 */


void GLFWApp::_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	KeyboardEvent e (key,action,glfwGetTime());
	pThis->_app.fireEvent(e);
}

/*
 * GLFW Mouse button callback - sends a full event with current position as well
 */

void GLFWApp::_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
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


void GLFWApp::_mousePositionCallback(GLFWwindow* window, double x, double y){
	if( !TwEventMousePosGLFW(x, y) ){  
		pThis->mMX = x;
		pThis->mMY = y;
		MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
		pThis->_app.fireEvent(e);
	}

}

void GLFWApp::_window_close_callback(GLFWwindow* window) {
	pThis->mRunning = GL_FALSE;
}


void GLFWApp::_mouseWheelCallback(GLFWwindow* window, double xpos, double ypos) {

	if (ypos == 1) {
		pThis->mFlag |= MOUSE_WHEEL_UP;	
		MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
		pThis->_app.fireEvent(e);
		pThis->mFlag ^= MOUSE_WHEEL_UP;
		
	} else if (ypos == -1) {
		pThis->mFlag |= MOUSE_WHEEL_DOWN;
		MouseEvent e (pThis->mMX,pThis->mMY,pThis->mFlag,glfwGetTime());
		pThis->_app.fireEvent(e);
		pThis->mFlag ^= MOUSE_WHEEL_DOWN;
	}	
}


GLFWwindow* GLFWApp::createWindow(const char * title ="Seburo", int w=800, int h=600) {

  GLFWwindow* win = glfwCreateWindow(w,h, title, NULL, NULL);

  if (!win){
		std::cout << "Seburo: Failed to open GLFW window" << std::endl;				
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
	while(pThis->mRunning)
		pThis->_app.update(pThis->_dt);
}



void GLFWApp::_error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

/*
 * Perform OpenGL initialisation using GLEW
 * \todo FULLSCREEN apps
 */

 void GLFWApp::initGL( const int w = 800, const int h =600,
 		const int major = 4, const int minor = 1, const int depthbits = 16) {

 	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
 	glfwWindowHint(GLFW_DEPTH_BITS, depthbits);

#ifdef _SEBURO_OSX
 	// Forward compatible and CORE Profile
 	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
 	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	//glfwWindowHint(GLFW_FSAA_SAMPLES, 4);

 	glfwSetErrorCallback(_error_callback);

	GLFWwindow* window = createWindow(mTitle.c_str(),w, h);

	if( !window ) {
		std::cerr << "Seburo Failed to open GLFW window\n" << std::endl;
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
		std::cerr << "Seburo: GLEWInit failed, aborting with error: " << err << std::endl;
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	CXGLERROR

	
	
	pThis->vWindows.push_back(window);


	TwInit(TW_OPENGL, NULL);
	TwWindowSize(w, h);

	pThis->_dt = 0.0;
	pThis->_app.init();

	CXGLERROR

	// fire a cheeky resize event to make sure all is well
	ResizeEvent e (w,h,glfwGetTime());
	pThis->_app.fireEvent(e);

	// Fire up the thread to keep update happy
	// Use a thread for the updates
  pThis->_update_thread =  new boost::thread(&GLFWApp::_update);

	mainLoop();

}

