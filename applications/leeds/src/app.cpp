/**
* @brief GLFW Window based solution
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
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
using namespace leeds;
using namespace cv;

namespace po = boost::program_options;


/*
 * Called when the mainloop starts, just once
 */

 void Leeds::init(){

    _obj.reset(new SharedObj());

    // Init Geometries

    _obj->_test_quad = gl::Quad(1.0,1.0);
    _obj->_fbo_quad = gl::Quad(640.0,480.0);
    _obj->_fbo_quad.setScale(glm::vec3(0.5,0.5,0.0));

    _obj->_test_quad.move(glm::vec3(-0.5,-0.5,0.0));

    _obj->_camera.move(glm::vec3(0,0,20.0f));
    _obj->_overview_camera.move(glm::vec3(0,0,16.0f));

    _obj->_gripper_fbo = gl::FBO(640,480);
    _obj->_overview_fbo = gl::FBO(640,480);
    _obj->_picking_fbo = gl::FBO(800,600);

    // Load Shaders

    _obj->_shader_camera.load("./data/quad_texture.vert", "./data/quad_texture.frag");
    _obj->_shader_basic.load("./data/quad.vert", "./data/quad.frag");
    _obj->_shader_light.load("./data/basic_lighting.vert", "./data/basic_lighting.frag");
    _obj->_shader_leeds.load("./data/leedsmesh.vert","./data/leedsmesh.frag");
    _obj->_shader_pick.load("./data/picker.vert", "./data/picker.frag");

    parseXML("./data/settings.xml");

    _obj->_camera_quad = gl::Quad(fromStringS9<float_t> ( _obj->_settings["leeds/cameras/width"]),
        fromStringS9<float_t> ( _obj->_settings["leeds/cameras/height"]));

    _obj->_ground_model = GLAsset<GeometryPNF>(AssetImporter::load("./data/ground.stl"));

    _obj->_gripper_model = GLAsset<GeometryPNF>(AssetImporter::load("./data/gripper.stl"));
    _obj->_gripper_model.setScale(glm::vec3(0.2,0.2,0.2));

    addTweakBar();

    // Link things to listen for events

    link(*this);
    link(_obj->_gripper_camera);
    link(_obj->_overview_camera);
    link(_obj->_screen_camera);

    // Initialise State
    _state.reset(new StateInitial(_obj) );
    link(*(static_pointer_cast<StateInitial>(_state)));

    glEnable(GL_DEPTH_TEST);
}

/*
 * Setup Controls for the application
 */

 void Leeds::addTweakBar(){
    pBar = TwNewBar("TweakBar");
    TwDefine(" GLOBAL help='Basic Leeds viewer application for scanned meshes.' "); // Message added to the help bar.  

    TwAddButton(pBar, "Generate Texture", _generateTexturedCallback, this, " label='Generate Textures for the mesh' ");
    TwAddButton(pBar, "Toggle Scanning", _scanningCallback, this, " label='Toggle Scanning' ");
    TwAddSeparator(pBar, "Camera Controls", NULL);
    TwAddVarCB(pBar,"Contrast",TW_TYPE_UINT8, _setContrast, _getContrast, _obj.get()," label='Contrast' precision=1 help='Adjust the contrast of all the cameras' " );
    TwAddVarCB(pBar,"Brightness",TW_TYPE_UINT8, _setBrightness, _getBrightness, _obj.get()," label='Brightness' precision=1 help='Adjust the contrast of all the cameras' " );
    TwAddVarCB(pBar,"Gain",TW_TYPE_UINT8, _setGain, _getGain, _obj.get()," label='Gain' precision=1 help='Adjust the contrast of all the cameras' " );
    TwAddVarCB(pBar,"Exposure",TW_TYPE_UINT8, _setExposure, _getExposure, _obj.get()," label='Exposure' precision=1 help='Adjust the contrast of all the cameras' " );
    TwAddVarCB(pBar,"Saturation",TW_TYPE_UINT8, _setSaturation, _getSaturation, _obj.get()," label='Saturation' precision=1 help='Adjust the contrast of all the cameras' " );

}

void Leeds::_generateTexturedCallback(void * obj){
    static_cast<Leeds*>(obj)->createTextured();
}


void Leeds::_scanningCallback(void * obj){
    Leeds* l = static_cast<Leeds*>(obj);
    // Skip all stages save initial 
    if (l->_state_scanning == StatePtr()){
        l->_state_scanning.reset(new StateScan(l->_obj));
        l->_state_scanning->setBlocking(true);
    }

    if(!l->_state_scanning->isActive())
        l->_state->add(l->_state_scanning);
    else
        l->_state_scanning->remove();

}


void Leeds::_setContrast(const void *value, void *clientData){
     static_cast< SharedObj* >(clientData)->_cam_contrast = *(const uint8_t *)value;  // for instance

#ifdef _GEAR_X11_GLX    
    BOOST_FOREACH(gl::VidCam v, 
        static_cast< SharedObj* >(clientData)->_cameras){
        v.setControl(UVC_CONTRAST,
            static_cast< SharedObj* >(clientData)->_cam_contrast);
    }
#endif
}


void Leeds::_setBrightness(const void *value, void *clientData){
     static_cast< SharedObj* >(clientData)->_cam_brightness = *(const uint8_t *)value;  // for instance

#ifdef _GEAR_X11_GLX    
    BOOST_FOREACH(gl::VidCam v, 
        static_cast<SharedObj* >(clientData)->_cameras){
        v.setControl(UVC_BRIGHTNESS,
            static_cast<SharedObj* >(clientData)->_cam_brightness);
    }
#endif
}



void Leeds::_setGain(const void *value, void *clientData){
     static_cast< SharedObj* >(clientData)->_cam_gain = *(const uint8_t *)value;  // for instance

#ifdef _GEAR_X11_GLX    
    BOOST_FOREACH(gl::VidCam v, 
        static_cast<SharedObj* >(clientData)->_cameras){
        v.setControl(UVC_GAIN,
            static_cast<SharedObj* >(clientData)->_cam_gain);
    }
#endif
}



void Leeds::_setExposure(const void *value, void *clientData){
     static_cast<SharedObj* >(clientData)->_cam_exposure = *(const uint8_t *)value;  // for instance

#ifdef _GEAR_X11_GLX    
    BOOST_FOREACH(gl::VidCam v, 
        static_cast<SharedObj* >(clientData)->_cameras){
        v.setControl(UVC_EXPOSURE,
            static_cast<SharedObj* >(clientData)->_cam_exposure);
    }
#endif
}


void Leeds::_setSaturation(const void *value, void *clientData){
     static_cast< SharedObj* >(clientData)->_cam_saturation = *(const uint8_t *)value;  // for instance

#ifdef _GEAR_X11_GLX    
    BOOST_FOREACH(gl::VidCam v, 
        static_cast< SharedObj* >(clientData)->_cameras){
        v.setControl(UVC_SATURATION,
            static_cast< SharedObj* >(clientData)->_cam_saturation);
    }
#endif
}



/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */

 void Leeds::display(double_t dt){

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );
    glViewport(0,0,_obj->_screen_width,_obj->_screen_height);

    _state->update(dt);
    _state->draw(dt);

    CXGLERROR
}

/*
 * Create the textured Geometry
 */

 void Leeds::createTextured() {

    // recreate the models 

    _obj->_ground_textured_model = _obj->_ground_model.getGeometry().convert<GeometryLeeds>();
    _obj->_textured_model = _obj->_model.getGeometry().convert<GeometryLeeds>();


    // Now create the texture co-ordinates

    for (size_t j=0; j < _obj->_cv_cameras.size(); j++){

        vector<cv::Point2f> results;
        CVVidCam cam = _obj->_cv_cameras[j];
        CameraParameters in = cam.getParams();
        vector<cv::Point3f> tOPoints;
        
        // Convert Mesh points to OpenCV and process

        GeometryLeeds geom = _obj->_textured_model.getGeometry();


        for (size_t i =0; i < geom.size(); ++i){
            VertPNT8F v = geom.getVertex(i);
            tOPoints.push_back(cv::Point3f( v.mP.x, v.mP.y, v.mP.z));            
        }

        cv::projectPoints(tOPoints, in.R, in.T, in.M, in.D, results );
        
        for (size_t i =0; i < tOPoints.size(); ++i){
            VertPNT8F v = geom.getVertex(i);
            Float2 f = {results[i].x, results[i].y};
            v.mT[j] = f;   
            geom.setVertex(v,i);
        }

        // Convert plane points
        tOPoints.clear();
        geom = _obj->_ground_textured_model.getGeometry();


        for (size_t i =0; i < geom.size(); ++i){
            VertPNT8F v = geom.getVertex(i);
            tOPoints.push_back(cv::Point3f( v.mP.x, v.mP.y, v.mP.z));            
        }

        cv::projectPoints(tOPoints, in.R, in.T, in.M, in.D, results );
        
        for (size_t i =0; i < tOPoints.size(); ++i){
            VertPNT8F v = geom.getVertex(i);
            Float2 f = {results[i].x, results[i].y};
            v.mT[j] = f;   
            geom.setVertex(v,i);
        }

    }
    if (_state_textured == StatePtr())
        _state_textured.reset(new StateDrawTexturedMesh(_obj));

    if (_state_mesh != StatePtr()){
        if (_state_mesh->isActive())
            _state_mesh->remove();
    }

    if (!_state_textured->isActive())
        _state->add(_state_textured);
   
    
    CXGLERROR
}


/*
 * This is called by the wrapper function when an event is fired
 */

 void Leeds::processEvent(MouseEvent e){
 }

/*
 * Called when the window is resized. You should set cameras here
 */

 void Leeds::processEvent(ResizeEvent e){
    glViewport(0,0,e.mW,e.mH);

    _obj->_screen_width = e.mW;
    _obj->_screen_height = e.mH;

    _obj->_picking_fbo.resize(e.mW,e.mH);
}

void Leeds::processEvent(KeyboardEvent e){
    cout << "Key Pressed: " << e.mKey << endl;

    if (e.mKey == GLFW_KEY_L && e.mAction == 0){
        string s = loadFileDialog();
        if (s != ""){
            cout << "Leeds - Loading " << s << endl;
            _obj->_model = gl::GLAsset<GeometryPNF>( AssetImporter::load(s));
            _state_mesh.reset(new StateDrawMesh(_obj));
            _state->add(_state_mesh);
        }
    }
    if (e.mKey == GLFW_KEY_R && e.mAction == 0){
        _obj->_camera.reset();
        _obj->_camera.move(glm::vec3(0.0,0.0,20.0f));
    }

    if (e.mKey == GLFW_KEY_T && e.mAction == 0){
     createTextured();
     cout << "Leeds - Created Textured" << endl;
 }
}

/*
 * Parse the XML for Cameras and other settings
 */

 void Leeds::parseXML(std::string filename) {
    if (_obj->_settings.loadFile(filename)){

        uint32_t w = fromStringS9<uint32_t> ( _obj->_settings["leeds/cameras/width"]);
        uint32_t h = fromStringS9<uint32_t> ( _obj->_settings["leeds/cameras/height"]);
        uint32_t f = fromStringS9<uint32_t> ( _obj->_settings["leeds/cameras/fps"]);
        
        XMLIterator i = _obj->_settings.iterator("leeds/cameras/cam");
        while (i){

            string dev = i["dev"];
            
            VidCam p (dev,w,h,f);
            _obj->_cameras.push_back(p);
            
            CVVidCam c(_obj->_cameras.back());
            c.loadParameters("./data/" + i["in"]);
            
            _obj->_cv_cameras.push_back(c);

            i.next();
        }
        
    }
}

/*
 * Main function - uses boost to parse program arguments
 */

 int main (int argc, const char * argv[]) {

#ifdef _GEAR_X11_GLX    
    // Linux only - need equivalent for Mac / Windows
    Gtk::Main kit(&argc, (char***)&argv);
#endif

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "S9Gear Basic Application - No Options")
    ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    Leeds b;

    GLFWApp a(b, 800, 600, false, argc, argv, "Leeds",4,0);

    return EXIT_SUCCESS;

}