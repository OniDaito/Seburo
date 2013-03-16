/**
* @brief Basic Application TemplateApp
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 20/08/2012
*
*/

#include "app.hpp"


#include <boost/program_options.hpp>
#include <signal.h>

using namespace std;
using namespace boost;
using namespace boost::assign;
using namespace s9;
using namespace s9::gl;
using namespace s9::gl::compvis;
using namespace TemplateNamespace;
using namespace cv;

namespace po = boost::program_options;


/*
 * Called when the mainloop starts, just once
 */

 void TemplateApp::init(){

    _obj.reset(new SharedObj());

    // Init Geometries

    _obj->_test_quad = gl::Quad(1.0,1.0);
    _obj->_test_quad.move(glm::vec3(-0.5,-0.5,0.0));

    _obj->_camera.move(glm::vec3(0,0,20.0f));

    // Load Shaders

    _obj->_shader_basic.load("./data/quad.vert", "./data/quad.frag");
    
    parseXML("./data/settings.xml");

    addTweakBar();

    // Link things to listen for events

    link(*this);
    link(_obj->_camera);
}

/*
 * Setup Controls for the application
 */

 void TemplateApp::addTweakBar(){
    _bar = TwNewBar("TweakBar");
    TwDefine(" GLOBAL help='TemplateApp application.' "); // Message added to the help bar.  

    TwAddButton(_bar, "Button", _buttonCallback, this, " label='A Button' ");
  
    TwAddSeparator(_bar, "Seperator", NULL);
    TwAddVarCB(_bar,"Value",TW_TYPE_UINT8, _setValue, _getValue, _obj.get()," label='Value' precision=1 help='Alter a value' " );
   
}

void TemplateApp::_buttonCallback(void * obj){
}


void TemplateApp::_setValue(const void *value, void *clientData){
}



/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */

 void TemplateApp::display(double_t dt){

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    _obj->_shader_basic.bind();

    glm::mat4 mvp = _obj->_camera.getMatrix() * _obj->_test_quad.getMatrix();

    _obj->_shader_basic.s("uMVPMatrix", mvp );
    _obj->_test_quad.draw();


glDisableVertexAttribArray(0);

    _obj->_shader_basic.unbind();

    CXGLERROR
}

/*
 * Update method - threaded
 */

 void TemplateApp::update(double_t dt) {
 }



/*
 * This is called by the wrapper function when an event is fired
 */

 void TemplateApp::processEvent(MouseEvent e){
 }

/*
 * Called when the window is resized. You should set cameras here
 */

 void TemplateApp::processEvent(ResizeEvent e){
    glViewport(0,0,e.mW,e.mH);

}

void TemplateApp::processEvent(KeyboardEvent e){
    cout << "Key Pressed: " << e.mKey << endl;

    if (e.mKey == GLFW_KEY_L && e.mAction == 0){
       
    }
}

/*
 * Parse the XML for Cameras and other settings
 */

 void TemplateApp::parseXML(std::string filename) {
    if (_obj->_settings.loadFile(filename)){

   /*     uint32_t w = fromStringS9<uint32_t> ( _obj->_settings["TemplateApp/cameras/width"]);
        uint32_t h = fromStringS9<uint32_t> ( _obj->_settings["TemplateApp/cameras/height"]);
        uint32_t f = fromStringS9<uint32_t> ( _obj->_settings["TemplateApp/cameras/fps"]);
        
        XMLIterator i = _obj->_settings.iterator("TemplateApp/cameras/cam");
        while (i){

            string dev = i["dev"];
            
            VidCam p (dev,w,h,f);
            _obj->_cameras.push_back(p);
            
            CVVidCam c(_obj->_cameras.back());
            c.loadParameters("./data/" + i["in"]);
            
            _obj->_cv_cameras.push_back(c);

            i.next();
        }*/
    }
}

/*
 * Main function - uses boost to parse program arguments
 */

 int main (int argc, const char * argv[]) {

#ifdef _SEBURO_X11_GLX    
    // Linux only - need equivalent for Mac / Windows
    Gtk::Main kit(&argc, (char***)&argv);
#endif

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "TemplateApp Application - No Options")
    ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    
    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    TemplateApp b;

    GLFWApp a(b, 800, 600, false, argc, argv, "TemplateApp");

    return EXIT_SUCCESS;

}