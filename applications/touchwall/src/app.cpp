/**
* @brief Basic Application TouchwallApp
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
using namespace s9::compvis;
using namespace touchwall;
using namespace cv;

namespace po = boost::program_options;


/*
 * Called when the mainloop starts, just once
 */

 void TouchwallApp::init(){

    _obj.reset(new SharedObj());

    // Init Geometries

    _obj->_test_quad = gl::Quad(1.0,1.0);
    _obj->_test_quad.move(glm::vec3(-0.5,-0.5,0.0));

    _obj->_video_quad = gl::Quad(640.0,480.0);
    _obj->_video_quad.setScale(glm::vec3(0.5,0.5,0.5));

    _obj->_camera.move(glm::vec3(0,0,20.0f));

    _obj->_cv_result = cv::Mat(cv::Size(640,480),CV_8UC3);

#ifdef _SEBURO_LINUX
    _obj->_video = VidCam("/dev/video0",640, 480,60);
#endif

    // Load Shaders

    _obj->_shader_basic.load("./data/quad.vert", "./data/quad.frag");
    _obj->_shader_video.load("./data/quad_texture.vert", "./data/quad_texture.frag");

    parseXML("./data/settings.xml");

    _obj->_tex_grey0 = Texture(glm::vec2(640,480), TEXTURE_GREY);
    _obj->_tex_grey1 = Texture(glm::vec2(640,480), TEXTURE_GREY);
    _obj->_tex_grey2 = Texture(glm::vec2(640,480), TEXTURE_GREY);
    _obj->_tex_grey3 = Texture(glm::vec2(640,480), TEXTURE_GREY);
    _obj->_tex_final = Texture(glm::vec2(640,480), TEXTURE_RGB);
           

    addTweakBar();

    // Link things to listen for events
    _obj->_process.addBlock<BlockGreyscale>()
        .addBlock<BlockThreshold>()
        .addBlock<BlockSmooth>()
        .addBlock<BlockContours>();


    _obj->_process.getBlock(1).setValue<int>("bottom",200);
    _obj->_process.getBlock(1).setValue<int>("top",240);

    link(*this);
    link(_obj->_camera);
    link(_obj->_camera_screen);
}

/*
 * Setup Controls for the application
 */

 void TouchwallApp::addTweakBar(){
    _bar = TwNewBar("TweakBar");
    TwDefine(" GLOBAL help='TouchwallApp application.' "); // Message added to the help bar.  

    TwAddButton(_bar, "Button", _buttonCallback, this, " label='A Button' ");
  
    TwAddSeparator(_bar, "Seperator", NULL);
    TwAddVarCB(_bar,"Value",TW_TYPE_UINT8, _setValue, _getValue, _obj.get()," label='Value' precision=1 help='Alter a value' " );
   
}

void TouchwallApp::_buttonCallback(void * obj){
}


void TouchwallApp::_setValue(const void *value, void *clientData){
}



/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */

 void TouchwallApp::display(double_t dt){

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    _obj->_shader_basic.bind();
    glm::mat4 mvp = _obj->_camera.getMatrix() * _obj->_test_quad.getMatrix();
    _obj->_shader_basic.s("uMVPMatrix", mvp );
    _obj->_test_quad.draw();
    _obj->_shader_basic.unbind();

    _obj->_video.update();



    // Display the processed video
    if (_obj->_process) {

        _obj->_tex_grey0.update( MatToGL(_obj->_process.getBlock(1).getResult()));
        _obj->_tex_grey1.update( MatToGL(_obj->_process.getBlock(2).getResult()));
        _obj->_tex_final.update( MatToGL(_obj->_cv_result));

        _obj->_video_quad.setPos(glm::vec3(0,0,0));
        mvp = _obj->_camera_screen.getMatrix() * _obj->_video_quad.getMatrix();
        _obj->_shader_video.bind();
        _obj->_shader_video.s("uMVPMatrix",mvp);
        _obj->_shader_video.s("uBaseTex",0);
        _obj->_video.bind();
        _obj->_video_quad.draw();
        _obj->_video.unbind();
       

        _obj->_video_quad.move(glm::vec3(320,0,0));
        mvp = _obj->_camera_screen.getMatrix() * _obj->_video_quad.getMatrix();
        _obj->_shader_video.s("uMVPMatrix",mvp);
        _obj->_tex_grey0.bind();
        _obj->_video_quad.draw();
        _obj->_tex_grey0.unbind();

         _obj->_video_quad.move(glm::vec3(320,0,0));
        mvp = _obj->_camera_screen.getMatrix() * _obj->_video_quad.getMatrix();
        _obj->_shader_video.s("uMVPMatrix",mvp);
        _obj->_tex_grey1.bind();
        _obj->_video_quad.draw();
        _obj->_tex_grey1.unbind();


        _obj->_video_quad.setPos(glm::vec3(0,240,0));
        mvp = _obj->_camera_screen.getMatrix() * _obj->_video_quad.getMatrix();
        _obj->_shader_video.s("uMVPMatrix",mvp);
        _obj->_tex_final.bind();
        _obj->_video_quad.draw();
        _obj->_tex_final.unbind();


        _obj->_shader_video.unbind();   
    }

    _obj->_video.update();

    CXGLERROR
}

/*
 * Update method - threaded
 */

void TouchwallApp::update(double_t dt) {
    cv::Mat t(cv::Size(640,480), CV_8UC3, _obj->_video.getBuffer());
   _obj->_cv_result = _obj->_process.process(t);
}



/*
 * This is called by the wrapper function when an event is fired
 */

 void TouchwallApp::processEvent(MouseEvent e){
 }

/*
 * Called when the window is resized. You should set cameras here
 */

 void TouchwallApp::processEvent(ResizeEvent e){
    glViewport(0,0,e.mW,e.mH);

}

void TouchwallApp::processEvent(KeyboardEvent e){
    cout << "Key Pressed: " << e.mKey << endl;

    if (e.mKey == GLFW_KEY_L && e.mAction == 0){
       
    }
}

/*
 * Parse the XML for Cameras and other settings
 */

 void TouchwallApp::parseXML(std::string filename) {
    if (_obj->_settings.loadFile(filename)){

   /*     uint32_t w = fromStringS9<uint32_t> ( _obj->_settings["TouchwallApp/cameras/width"]);
        uint32_t h = fromStringS9<uint32_t> ( _obj->_settings["TouchwallApp/cameras/height"]);
        uint32_t f = fromStringS9<uint32_t> ( _obj->_settings["TouchwallApp/cameras/fps"]);
        
        XMLIterator i = _obj->_settings.iterator("TouchwallApp/cameras/cam");
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
    ("help", "TouchwallApp Application - No Options")
    ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    
    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    TouchwallApp b;

    GLFWApp a(b, 800, 600, false, argc, argv, "TouchwallApp");

    return EXIT_SUCCESS;

}