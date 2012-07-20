/**
* @brief GLFW Window based solution
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#include "app.hpp"


using namespace std;
using namespace boost;
using namespace boost::assign;
using namespace s9;

/*
 * Test App Variables - global for now
 */
 
OrbitCamera sCam;
gl::Quad sRefQuad(1.0,1.0);

Shader sShaderQuad;

/*
 * GLApp Static variables
 */

MouseStatus GLApp::mMouseStatus;
std::vector<GLFWwindow> GLApp::vWindows;
GLboolean GLApp::mRunning;


/*
 * Create our Windows and contexts
 */

GLApp::GLApp() {
	
	if( !glfwInit(NULL) ){
		fprintf( stderr, "Failed to initialize GLFW\n" );
		exit( EXIT_FAILURE );
	}

}

/*
 * display the stuff in this window
 */

void GLApp::display(GLFWwindow window){
	int w,h;
	glfwGetWindowSize(window, &w,&h);
	glViewport(0,0,w,h);
	
	sCam.setRatio(static_cast<float_t>(w) / static_cast<float_t>(h));
	
	glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.93f, 1.0f)[0]);
	GLfloat depth = 1.0f;
	glClearBufferfv(GL_DEPTH, 0, &depth );
	
	glm::mat4 mvp = getMatrix(sCam, sRefQuad);
	
	sShaderQuad.bind();
	
	glUniformMatrix4fv(sShaderQuad.location("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));

	sRefQuad.draw();

	sShaderQuad.unbind();
	
	CXGLERROR
}

/*
 * Create Window
 */
 
 GLFWwindow GLApp::createWindow(std::string title="S9Gear", size_t w = 800, size_t h = 600){
	GLFWwindow win = glfwOpenWindow(w, h, GLFW_WINDOWED, title.c_str(), NULL);
	if (!win){
		cerr << "Failed to open GLFW window: " << glfwErrorString(glfwGetError()) << endl;				
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowPos(win,100,100);
	
	return win;
}


/*
 * Main Loop - calls the drawing methods and events for our mini apps
 */

void GLApp::mainLoop() {

	mRunning = GL_TRUE;
	double_t t = glfwGetTime();

	while (mRunning){

		BOOST_FOREACH ( GLFWwindow b, vWindows) {	
			glfwMakeContextCurrent(b);
			display(b);
			glfwSwapBuffers();
		}
		t = glfwGetTime();
		
	
		glfwPollEvents();
		
#ifdef _GEAR_WIN32_WGL
		gtk_main_iteration_do(false);
#endif
		
		BOOST_FOREACH ( GLFWwindow b, vWindows) {
			if (!glfwIsWindow(b))
				mRunning = GL_FALSE;
		}
    }
}

/*
 * Key Callback - Check the window against the apps and call the app
 */
 
void GLApp::keyCallback(GLFWwindow window, int key, int action){
	
#ifdef DEBUG
	cout << "Pressed: " << key << " Action: " << action << endl;
#endif
		
}

/*
 * Mouse Callback for Button
 */


void GLApp::mouseButtonCallback(GLFWwindow window, int button, int action) {
	if (!TwEventMouseButtonGLFW(button,action)){
		switch(button){
			case 0: {
				if (action)
					mMouseStatus.mButton  |= MOUSE_LEFT;
				else
					mMouseStatus.mButton  ^= MOUSE_LEFT;
				break;
			}
			case 1: {
				if (action)
					mMouseStatus.mButton  |= MOUSE_RIGHT;
				else
					mMouseStatus.mButton  ^= MOUSE_RIGHT;
				break;
			}
			case 3: {
				if (action)
					mMouseStatus.mButton  |= MOUSE_MIDDLE;
				else
					mMouseStatus.mButton  ^= MOUSE_MIDDLE;
				break;
			}
		}
	}
	
}

/*
 * Mouse Callback for Position
 */

static float_t msense = 0.1;

void GLApp::mousePositionCallback(GLFWwindow window, int x, int y) {
	if( !TwEventMousePosGLFW(x, y) ){  
		if (mMouseStatus.mButton & MOUSE_LEFT){
	
			sCam.yaw(static_cast<float_t>(mMouseStatus.mDX) * msense);
			sCam.pitch(static_cast<float_t>(mMouseStatus.mDY) * msense);
			
		}
		
		mMouseStatus.mDX = x - mMouseStatus.mX;
		mMouseStatus.mDY = y - mMouseStatus.mY;
		mMouseStatus.mX = x;
		mMouseStatus.mY = y;
			
	}

#ifdef DEBUG
	cout << "DX: " << mMouseStatus.mDX << ", DY: " << mMouseStatus.mDY << endl;
	printf("%08x at %0.3f: Mouse position: %i %i\n", 0, glfwGetTime(), x, y);
#endif

}

/*
 * Mouse Wheel Callback
 */

static float_t zsense = 0.1; 

void GLApp::mouseWheelCallback(GLFWwindow window, int xpos, int ypos){
	sCam.zoom(ypos * zsense);	
}

/*
 * File Dialog running with GTK
 */
 
void GLApp::loadFile() {
	
#ifdef _GEAR_WIN32_WGL	
	
	Gtk::FileChooserDialog dialog("Please choose a Mesh File", Gtk::FILE_CHOOSER_ACTION_OPEN);

	//Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);
	
	int result = dialog.run();

	//Handle the response:
	switch(result) {
		case(Gtk::RESPONSE_OK): {
			cout << dialog.get_filename() << endl;
			break;
		}
		case(Gtk::RESPONSE_CANCEL): {
			break;
		}
		default:{
			break;
		}
	}
#endif

}
/*
 * Monitor Callback
 */
 
void GLApp::monitorCallback( GLFWmonitor m, int p){
	cout << "Monitor " << p << endl;
}


/*
 * Fire the reshape event for a window
 */
 
void GLApp::reshape(GLFWwindow window, int w, int h ) {

}

/*
 * Init the applications now we have a GL Context
 */
 
void GLApp::init() {
	
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);
	
	///\todo fully switch to core profile - there is something causing an error in core
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	
	GLFWwindow w = createWindow();
	
	CXGLERROR
	
	cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
	
	// Set Basic Callbacks
	glfwSetKeyCallback(keyCallback);
	glfwSetMousePosCallback(mousePositionCallback);
	glfwSetMouseButtonCallback(mouseButtonCallback);
	glfwSetScrollCallback(mouseWheelCallback);
	glfwSetWindowSizeCallback(reshape);
	glfwSetMonitorDeviceCallback(monitorCallback);

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
		cout << "GLEWInit failed, aborting." << endl;
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	
	CXGLERROR
	
	TwInit(TW_OPENGL, NULL);
	
	// Now we can commit
	
	vWindows.push_back(w);
	
	// Create basic references


	// Move the Camera
	sCam.move(glm::vec3(0,0,20));

	// Load Basic Shader
	sShaderQuad.load("../../../shaders/quad.vert", "../../../shaders/quad.frag");
	
	
	CXGLERROR
	
	// OpenGL Constants
	glEnable(GL_TEXTURE_RECTANGLE);
}



int main(int argc, char *argv[]) {

#ifdef _GEAR_WIN32_WGL	
	// Linux only - need equivalent for Mac / Windows
	Gtk::Main kit(argc, argv);
#endif
	
	GLApp app;
  
	app.init();

	app.mainLoop();

	glfwTerminate();
	
	exit( EXIT_SUCCESS );
}
