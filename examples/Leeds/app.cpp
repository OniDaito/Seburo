/**
* @brief Leeds Medical Viewer app - GLFW Window based solution
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/07/2012
*
*/

#include "app.hpp"


using namespace std;
using namespace boost;
using namespace boost::assign;
using namespace s9;


///\todo technically we can remove all references as we are placing all important stuff on the stack with SharedObj

/*
 * Test App Variables - global for now
 */
 
FBO sFBO, sFBOPick;
OrbitCamera sCam;
Camera sGripperCam;
ScreenCamera sHUDCam;
Primitive sRefQuad;
Primitive sHUDQuad;
Primitive sCamQuad;
Primitive sModel;
Primitive sGripper;
Primitive sModelTextured;
Shader sShaderQuad;
Shader sShaderTexQuad;
Shader sShaderMesh;
Shader sShaderLeedsMesh;
Shader sShaderGripper;
Shader sShaderPick;

/*
 * GLApp Static variables
 */

MouseStatus GLApp::mMouseStatus;
std::vector<GLFWwindow> GLApp::vWindows;
GLboolean GLApp::mRunning;
PrimPtr GLApp::pPicked;
XMLSettings GLApp::mSettings;
std::vector<S9VidCam> GLApp::vCameras;
std::vector<CVVidCam> GLApp::vCVCameras;
WingedEdge GLApp::mWE;
TwBar* GLApp::pBar; 
std::vector< std::vector<float_t> > GLApp::vExtra;
		

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
 * Render a Mesh
 */
 
void GLApp::drawMesh(Camera &c) {

	if (sModel) {
		sShaderMesh.bind();
	//	glEnable(GL_CULL_FACE);
	//	glCullFace(GL_BACK);
			
		glm::mat4 mvp = getMatrix(c, sModel);
		glm::mat4 mv = sCam.getViewMatrix() * sModel.getMatrix();
		glm::mat4 mn = glm::inverseTranspose(c.getViewMatrix());
		
		glUniformMatrix4fv(sShaderMesh.location("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix4fv(sShaderMesh.location("uMVMatrix"), 1, GL_FALSE, glm::value_ptr(mv));
		glUniformMatrix4fv(sShaderMesh.location("uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(mn));
		glUniform1f(sShaderMesh.location("uShininess"), 20.0f);
		glm::vec3 l = sCam.getPos() + sCam.getLook();
		glUniform3f(sShaderMesh.location("uLight0"),l.x,l.y,l.z);

		sModel.draw();
		sShaderMesh.unbind();
	//	glDisable(GL_CULL_FACE);
	}
	
}



/*
 * Render a Leeds mesh, textured with Cameras
 */
 
void GLApp::drawLeedsMesh(Camera &c) {

	if (sModelTextured) {
		sShaderLeedsMesh.bind();
	//	glEnable(GL_CULL_FACE);
	//	glCullFace(GL_BACK);
			
		glm::mat4 mvp = getMatrix(c, sModel);
		glm::mat4 mv = sCam.getViewMatrix() * sModel.getMatrix();
		glm::mat4 mn = glm::inverseTranspose(c.getViewMatrix());
		
		glUniformMatrix4fv(sShaderLeedsMesh.location("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix4fv(sShaderLeedsMesh.location("uMVMatrix"), 1, GL_FALSE, glm::value_ptr(mv));
		glUniformMatrix4fv(sShaderLeedsMesh.location("uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(mn));
		
		for (int i=0; i < vCVCameras.size(); i++){
			glActiveTexture(GL_TEXTURE0 + i);
			vCVCameras[i].bind();
			
			std::stringstream Num;
			std::string str;
			Num << i;
			str = "uCam" + Num.str();
			
			cv::Mat n = vCVCameras[i].getNormal();
			glm::vec3 nn (n.at<double_t>(0,0),n.at<double_t>(1,0),n.at<double_t>(2,0));
			nn  = glm::normalize(nn);
			
			glUniform3f(sShaderLeedsMesh.location(str.c_str()),nn.x,nn.y,nn.z);
			
		}
		
		glUniform1f(sShaderLeedsMesh.location("uShininess"), 20.0f);
		glm::vec3 l = sCam.getPos() + sCam.getLook();
		glUniform3f(sShaderLeedsMesh.location("uLight0"),l.x,l.y,l.z);

		sModelTextured.draw();
		sShaderLeedsMesh.unbind();
	//	glDisable(GL_CULL_FACE);
		for (int i=0; i < vCVCameras.size(); i++){
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	
}


void GLApp::drawGripper(Camera &cam) {

	if (sGripper) {
		sShaderGripper.bind();
		
		glm::mat4 mvp = getMatrix(cam, sGripper);
		glm::mat4 mv = cam.getViewMatrix() * sGripper.getMatrix();
		glm::mat4 mn = glm::inverseTranspose(cam.getViewMatrix());
		glm::mat4 mi = glm::inverse(cam.getViewMatrix());
		
		glUniformMatrix4fv(sShaderGripper.location("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix4fv(sShaderGripper.location("uMVMatrix"), 1, GL_FALSE, glm::value_ptr(mv));
		glUniformMatrix4fv(sShaderGripper.location("uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(mn));
		glUniform1i(sShaderGripper.location("uBaseTex"), 0);
		glUniform1f(sShaderGripper.location("uShininess"), 20.0f);
		glm::vec3 l = cam.getPos();
		glUniform3f(sShaderGripper.location("uLight0"),l.x,l.y,l.z);
		
		sGripper.draw();
		sShaderGripper.unbind();
	}
}

/*
 * Check if an object has been picked, returning the value at the picked location
 */
 
void GLApp::picked(glm::vec2 m, glm::vec4 &c){
		
	sFBOPick.bind();
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	CXGLERROR
	
	GLfloat *data = (GLfloat*) new GLfloat[4];
	glReadPixels(m.x, m.y, 1, 1, GL_RGBA, GL_FLOAT, data);
	CXGLERROR
	c = glm::vec4 ( data[0], data[1], data[2], data[3]);
	
	CXGLERROR
	sFBOPick.unbind();

} 

/*
 * Specific to Leeds - Generate a new VBO with face data
 * \todo move to a special algorithms class / namespace?
 * \todo reference that paper as well!
 */

Primitive GLApp::generateTextured( WingedEdge &w){
	vector<glm::vec3> normals;
	Primitive p = mWE.flatten();
	
	// An extra 7 buffers please
	GLuint *vbo = new GLuint[7];
	glGenBuffers(7,vbo);
	for (int i =0; i < 7; i ++){
		vector<float_t> b;
		vExtra.push_back( b );
	}

	p.bind();
	
	for (size_t j=0; j < vCVCameras.size(); j++){
		
		vector<cv::Point2f> results;
		CVVidCam cam = vCVCameras[j];
		CameraParameters in = cam.getParams();
		vector<cv::Point3f> tOPoints;
		
		for (size_t i =0; i < p.getVBO().vVertices.size() / 3; ++i){
			if (j != 0) {
				vExtra[j-1].push_back(0.0);
				vExtra[j-1].push_back(0.0);
			}
			
			glm::vec3 vert( p.getVBO().vVertices[i*3], 
						p.getVBO().vVertices[i*3+1],
						p.getVBO().vVertices[i*3+2]);
			
			
			tOPoints.push_back(cv::Point3f(vert.x, vert.y, vert.z));
			
			// We need to mirror the co-ordinates here but only in the X plane
			
		}
		cv::projectPoints(tOPoints, in.R, in.T, in.M, in.D, results );
		
		for (size_t i =0; i < p.getVBO().vVertices.size() / 3; ++i){
			if (j == 0){
				p.getVBO().vTexCoords[i*2] = cam.getSize().x - results[i].x;
				p.getVBO().vTexCoords[i*2+1] = results[i].y;
			}
			else{
				vExtra[j-1][i*2] = cam.getSize().x - results[i].x;
				vExtra[j-1][i*2+1] = results[i].y;
			}
		}
	}
	
	p.getVBO().allocateTexCoords();
	
	for (int i =0; i < 7; i ++){
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, vExtra[i].size() * sizeof(GLfloat), &(vExtra[i][0]), GL_DYNAMIC_DRAW);
		
		glEnableVertexAttribArray(p.getVBO().mNumBufs + i);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		
		glVertexAttribPointer(p.getVBO().mNumBufs + i,2,GL_FLOAT,GL_FALSE,0, (GLubyte*) NULL);
	
	}
	

	
	p.unbind();
	
	delete vbo;
	
	return p;
}

/*
 * display the stuff in this window
 */

void GLApp::display(GLFWwindow window){
	int w,h;
	glfwGetWindowSize(window, &w,&h);
	glViewport(0,0,w,h);
	
	sCam.setRatio(static_cast<float_t>(w) / static_cast<float_t>(h));
	sGripperCam.setRatio(static_cast<float_t>(w) / static_cast<float_t>(h));
	
	glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.93f, 1.0f)[0]);
	GLfloat depth = 1.0f;
	glClearBufferfv(GL_DEPTH, 0, &depth );
	
	glm::mat4 mvp = getMatrix(sCam, sRefQuad);
	
	sShaderQuad.bind();
	glUniformMatrix4fv(sShaderQuad.location("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));
	sRefQuad.draw();
	sShaderQuad.unbind();
	
	if (sModelTextured) drawLeedsMesh(sCam); else drawMesh(sCam);
	drawGripper(sCam); 
	
	// Render to FBO
	
	sFBO.bind();

	sGripperCam.align(sGripper);
	sGripperCam.move(glm::vec3(5.0,0.0,2.0));
	sGripperCam.yaw(90.0);

	glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.8f, 0.8f, 0.83f, 1.0f)[0]);
	glClearBufferfv(GL_DEPTH, 0, &depth );
	
	glm::mat4 mvpgripper = getMatrix(sGripperCam, sRefQuad);
	
	sShaderQuad.bind();
	glUniformMatrix4fv(sShaderQuad.location("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(mvpgripper));
	sRefQuad.draw();
	sShaderQuad.unbind();
	
	if (sModelTextured) drawLeedsMesh(sGripperCam); else drawMesh(sGripperCam);
	drawGripper(sGripperCam);
	sFBO.unbind();

	// Render to Picker
	sFBOPick.bind();
	glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)[0]);
	glClearBufferfv(GL_DEPTH, 0, &depth );
	mvp = getMatrix(sCam, sGripper);
	sShaderPick.bind();
	glUniform4f(sShaderPick.location("uColour"), sGripper.getColour().x, sGripper.getColour().y, sGripper.getColour().z,1.0 );
	glUniformMatrix4fv(sShaderPick.location("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));
	sGripper.draw();
	sShaderPick.unbind();
	sFBOPick.unbind();

	
	// Draw FBO
	glActiveTexture(GL_TEXTURE0);
	mvp = sHUDCam.getMatrix();
	sShaderTexQuad.bind();
	sFBO.bindColour();
	glUniformMatrix4fv(sShaderTexQuad.location("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));
	sHUDQuad.draw();
	sFBO.unbindColour();
	sShaderTexQuad.unbind();
	
	
	// Draw Cameras
	
	float_t step = static_cast<float_t>(w) / 8.0; // assume 8 cameras for now
	float_t factor = static_cast<float_t>(w) / ( vCameras[0].getSize().x * 8.0);
	float_t t =0 ;
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(factor,factor,1.0));
	sShaderTexQuad.bind();
	BOOST_FOREACH(CVVidCam p, vCVCameras){
		p.update();
		p.bindRectified();
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(t, h - (p.getSize().y * factor) ,0));
		
		mvp = sHUDCam.getMatrix();
		mvp =  mvp * trans * scale;
		glUniformMatrix4fv(sShaderTexQuad.location("uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));
		sCamQuad.draw();
		p.unbind();
		t+= step;
	}
	sShaderTexQuad.unbind();
	
	
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
			TwDraw(); // On all windows for now
			glfwSwapBuffers();
		}
	
		t = glfwGetTime();
	
		glfwPollEvents();
		
#ifdef _GEAR_X11_GLX
		gtk_main_iteration_do(false);
#endif
		
		BOOST_FOREACH ( GLFWwindow b, vWindows) {
			if (!glfwIsWindow(b))
				mRunning = GL_FALSE;
		}
    }
    
    
    // Cleanup
    BOOST_FOREACH(S9VidCam p, vCameras){
		p.stop();
	}
    
}

/*
 * Key Callback - Check the window against the apps and call the app
 */
 
void GLApp::keyCallback(GLFWwindow window, int key, int action){
	if (key==76 && action == 0)
		loadFile();
	
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
				if (action) {
					mMouseStatus.mButton  |= MOUSE_LEFT;
					glm::vec4 c;
					// Convert to tex coordinates from screen
					int w,h;
					glfwGetWindowSize(window, &w,&h);
					picked(glm::vec2(mMouseStatus.mX, h - mMouseStatus.mY), c);
						// Normally loop through objects but we have no need here
					if (sGripper.getColour() == c){
						pPicked = boost::make_shared<Primitive>(sGripper);
#ifdef DEBUG
						cout << "S9Gear - Picked Gripper" << endl;
#endif
			
					}
					else{
						pPicked = PrimPtr(); // set to equivalent of NULL
#ifdef DEBUG
						cout << "S9Gear - Picked nothing" << endl;
#endif
					}
				
				}
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
			case 2: {
				if (action)
					mMouseStatus.mButton  |= MOUSE_MIDDLE;
				else
					mMouseStatus.mButton  ^= MOUSE_MIDDLE;
				break;
			}
		}
	}
}



static float_t msense = 0.1;

/*
 * Move Picked Function
 */
 
void GLApp::movePicked(glm::vec2 d){
	
	glm::vec3 dir = sCam.getPos() - sCam.getLook();
	dir = glm::normalize(dir);
	glm::vec3 shiftx = glm::cross(dir,sCam.getUp());
	shiftx *= (-d.x * msense);
	glm::vec3 shifty = sCam.getUp() * (-d.y * msense);

	glm::vec3 dpos = shiftx + shifty;
	sGripper.move(dpos);
	
#ifdef DEBUG
	cout << "S9Gear - Moved Gripper." << endl;
#endif
	
}


/*
 * Mouse Callback for Position
 */

void GLApp::mousePositionCallback(GLFWwindow window, int x, int y) {
	if( !TwEventMousePosGLFW(x, y) ){  
		if (mMouseStatus.mButton & MOUSE_LEFT){
			sCam.yaw(static_cast<float_t>(mMouseStatus.mDX) * msense);
			sCam.pitch(static_cast<float_t>(mMouseStatus.mDY) * msense);
		}
		
		if (mMouseStatus.mButton & MOUSE_MIDDLE){
			if (pPicked){
				movePicked(glm::vec2(mMouseStatus.mDX,mMouseStatus.mDY));
			}
			else{
				sCam.shift(glm::vec2(mMouseStatus.mDX * msense,mMouseStatus.mDY * msense));
			}
		}
		
		mMouseStatus.mDX = x - mMouseStatus.mX;
		mMouseStatus.mDY = y - mMouseStatus.mY;
		mMouseStatus.mX = x;
		mMouseStatus.mY = y;
			
	}

#ifdef DEBUG
	//cout << "DX: " << mMouseStatus.mDX << ", DY: " << mMouseStatus.mDY << endl;
	//printf("%08x at %0.3f: Mouse position: %i %i\n", 0, glfwGetTime(), x, y);
#endif

}


void GLApp::parseXML() {
	if (mSettings.loadFile("../../../data/Leeds/settings.xml")){
		
		size_t w = fromStringS9<size_t> ( mSettings["leeds/cameras/width"]);
		size_t h = fromStringS9<size_t> ( mSettings["leeds/cameras/height"]);
		size_t f = fromStringS9<size_t> ( mSettings["leeds/cameras/fps"]);
		
		makeQuad(sCamQuad,w,h);

		
		XMLIterator i = mSettings.iterator("leeds/cameras/cam");
		while (i){
			
			string dev = i["dev"];
			
			S9VidCam p;
			p.open(dev,w,h,f);
			vCameras.push_back(p);
			
			CVVidCam c(vCameras.back());
			c.loadParameters("../../../data/Leeds/" + i["in"]);
			
			vCVCameras.push_back(c);
					
			i.next();
		}
		
	}
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
	
#ifdef _GEAR_X11_GLX	
	
	Gtk::FileChooserDialog dialog("Please choose a Mesh File", Gtk::FILE_CHOOSER_ACTION_OPEN);

	//Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);
	
	/*Glib::RefPtr<Gtk::FileFilter> filter_text = Gtk::FileFilter::create();
	filter_text->set_name("PCD Files");
	filter_text->add_mime_type("text/plain");
	dialog.add_filter(filter_text);*/

	int result = dialog.run();

	//Handle the response:
	switch(result) {
		case(Gtk::RESPONSE_OK): {		
			AssetGenerator::loadAsset(dialog.get_filename(),sModel);
			mWE.make(sModel);
					
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
	sFBO.resize(w,h);
	sFBOPick.resize(w,h);
	resizeHUD(w,h);
	TwWindowSize(w, h);
}

void GLApp::resizeHUD(int w, int h){
	
	sHUDCam.setDim(w,h);
	// Resize the hud quad and also resize the texture coordinates
	if (sHUDQuad) {
		sHUDQuad.bind();
		sHUDQuad.getVBO().vTexCoords[1] = static_cast<float_t>(h);
		sHUDQuad.getVBO().vTexCoords[2] = static_cast<float_t>(w);
		sHUDQuad.getVBO().vTexCoords[3] = static_cast<float_t>(h);
		sHUDQuad.getVBO().vTexCoords[4] = static_cast<float_t>(w);
		sHUDQuad.getVBO().allocateTexCoords();
		
		sHUDQuad.getVBO().vTexCoords[1] = static_cast<float_t>(h);
		sHUDQuad.getVBO().vTexCoords[2] = static_cast<float_t>(w);
		sHUDQuad.getVBO().vTexCoords[3] = static_cast<float_t>(h);
		sHUDQuad.getVBO().vTexCoords[4] = static_cast<float_t>(w);
		sHUDQuad.getVBO().allocateTexCoords();
		
		sHUDQuad.unbind();
	}
}

/*
 * Add basic Tweakbar - assume context for window is set
 */

void TW_CALL GLApp::generateTexturedCallback(void * ){
	sModelTextured = generateTextured(mWE);
}
 
void GLApp::addTweakBar() {
	pBar = TwNewBar("TweakBar");
    TwDefine(" GLOBAL help='Basic Leeds viewer application for scanned meshes.' "); // Message added to the help bar.  

	TwAddButton(pBar, "Generate Texture",  generateTexturedCallback, NULL, " label='Generate Textures for the mesh' ");

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

	makeReferenceQuad(sRefQuad,1.0,1.0);
	makeReferenceQuad(sHUDQuad,400.0,300.0);


	// Move the Camera
	sCam.move(glm::vec3(0,0,20));
	resizeHUD(800,600);
	
	// Load Basic Shader
	sShaderQuad.load("../../../shaders/quad.vert", "../../../shaders/quad.frag");
	sShaderTexQuad.load("../../../shaders/quad_texture.vert", "../../../shaders/quad_texture.frag");
	sShaderMesh.load("../../../shaders/lighting.vert", "../../../shaders/lighting.frag");
	sShaderGripper.load("../../../shaders/gripper.vert", "../../../shaders/gripper.frag");
	sShaderPick.load("../../../shaders/picker.vert","../../../shaders/picker.frag");
	sShaderLeedsMesh.load("../../../shaders/leedsmesh.vert", "../../../shaders/leedsmesh.frag");

	// Load Objects
	AssetGenerator::loadAsset("../../../data/gripper.stl",sGripper);
	sGripper.setScale(glm::vec3(0.2,0.2,0.2));
	sGripper.setPos(glm::vec3(-0.1,-0.5,-2.0));
	sGripper.setColour(glm::vec4(1.0f,0.0f,0.0f,1.0f));
	sGripperCam.align(sGripper);
	sGripperCam.pitch(M_PI);
	
	// Setup FBOs
	
	sFBO.setup(800,600);
	sFBOPick.setup(800,600);

	//sFBO.printFramebufferInfo();
	
	CXGLERROR
	
	// OpenGL Constants
	glEnable(GL_TEXTURE_RECTANGLE);
	glEnable(GL_DEPTH_TEST);
	
	parseXML();
	
	addTweakBar();
	
}



int main(int argc, char *argv[]) {

#ifdef _GEAR_X11_GLX	
	// Linux only - need equivalent for Mac / Windows
	Gtk::Main kit(argc, argv);
#endif
	
	GLApp app;
  
	app.init();

	app.mainLoop();

	glfwTerminate();
	
	exit( EXIT_SUCCESS );
}
