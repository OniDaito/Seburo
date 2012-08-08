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
using namespace leeds;
using namespace cv;

namespace po = boost::program_options;


/*
 * Called when the mainloop starts, just once
 */

 void Leeds::init(){

    _test_quad = gl::Quad(1.0,1.0);
    _fbo_quad = gl::Quad(640.0,480.0);
    _fbo_quad.setScale(glm::vec3(0.5,0.5,0.0));

    _test_quad.move(glm::vec3(-0.5,-0.5,0.0));

    _camera.move(glm::vec3(0,0,20.0f));
    _overview_camera.move(glm::vec3(0,0,16.0f));

    _gripper_fbo = gl::FBO(640,480);
    _overview_fbo = gl::FBO(640,480);
    _picking_fbo = gl::FBO(800,600);

    _shader_camera.load("./data/quad_texture.vert", "./data/quad_texture.frag");
    _shader_basic.load("./data/quad.vert", "./data/quad.frag");
    _shader_light.load("./data/basic_lighting.vert", "./data/basic_lighting.frag");
    _shader_leeds.load("./data/leedsmesh.vert","./data/leedsmesh.frag");
    _shader_pick.load("./data/picker.vert", "./data/picker.frag");



    parseXML("./data/settings.xml");

    _camera_quad = gl::Quad(fromStringS9<float_t> ( mSettings["leeds/cameras/width"]),
        fromStringS9<float_t> ( mSettings["leeds/cameras/height"]));

    _ground_model = GLAsset<GeometryPNF>(AssetImporter::load("./data/ground.stl"));

    _gripper_model = GLAsset<GeometryPNF>(AssetImporter::load("./data/gripper.stl"));
    _gripper_model.setScale(glm::vec3(0.2,0.2,0.2));

    addTweakBar();

    link(*this);
    link(_camera);
    link(_gripper_camera);
    link(_overview_camera);
    link(_screen_camera);
    
    _scanning = false;

    glEnable(GL_DEPTH_TEST);
}

/*
 * Setup Controls for the application
 */

 void Leeds::addTweakBar(){
    pBar = TwNewBar("TweakBar");
    TwDefine(" GLOBAL help='Basic Leeds viewer application for scanned meshes.' "); // Message added to the help bar.  

    TwAddButton(pBar, "Generate Texture",  _generateTexturedCallback, this, " label='Generate Textures for the mesh' ");
    TwAddSeparator(pBar, "Camera Controls", NULL);
    TwAddVarCB(pBar,"Contrast",TW_TYPE_UINT8, _setContrast, _getContrast, this," label='Contrast' precision=1 help='Adjust the contrast of all the cameras' " );
    TwAddVarCB(pBar,"Brightness",TW_TYPE_UINT8, _setBrightness, _getBrightness, this," label='Brightness' precision=1 help='Adjust the contrast of all the cameras' " );
    TwAddVarCB(pBar,"Gain",TW_TYPE_UINT8, _setGain, _getGain, this," label='Gain' precision=1 help='Adjust the contrast of all the cameras' " );
    TwAddVarCB(pBar,"Exposure",TW_TYPE_UINT8, _setExposure, _getExposure, this," label='Exposure' precision=1 help='Adjust the contrast of all the cameras' " );
    TwAddVarCB(pBar,"Saturation",TW_TYPE_UINT8, _setSaturation, _getSaturation, this," label='Saturation' precision=1 help='Adjust the contrast of all the cameras' " );

}

void Leeds::_generateTexturedCallback(void * obj){
    static_cast<Leeds*>(obj)->createTextured();
}

void Leeds::_setContrast(const void *value, void *clientData){
     static_cast<Leeds*>(clientData)->_cam_contrast = *(const uint8_t *)value;  // for instance

#ifdef _GEAR_X11_GLX    
    BOOST_FOREACH(gl::VidCam v, 
        static_cast<Leeds*>(clientData)->vCameras){

        v.setControl(UVC_CONTRAST,
            static_cast<Leeds*>(clientData)->_cam_contrast);
    }
#endif
}


void Leeds::_setBrightness(const void *value, void *clientData){
     static_cast<Leeds*>(clientData)->_cam_brightness = *(const uint8_t *)value;  // for instance

#ifdef _GEAR_X11_GLX    
    BOOST_FOREACH(gl::VidCam v, 
        static_cast<Leeds*>(clientData)->vCameras){

        v.setControl(UVC_BRIGHTNESS,
            static_cast<Leeds*>(clientData)->_cam_brightness);
    }
#endif
}



void Leeds::_setGain(const void *value, void *clientData){
     static_cast<Leeds*>(clientData)->_cam_gain = *(const uint8_t *)value;  // for instance

#ifdef _GEAR_X11_GLX    
    BOOST_FOREACH(gl::VidCam v, 
        static_cast<Leeds*>(clientData)->vCameras){

        v.setControl(UVC_GAIN,
            static_cast<Leeds*>(clientData)->_cam_gain);
    }
#endif
}



void Leeds::_setExposure(const void *value, void *clientData){
     static_cast<Leeds*>(clientData)->_cam_exposure = *(const uint8_t *)value;  // for instance

#ifdef _GEAR_X11_GLX    
    BOOST_FOREACH(gl::VidCam v, 
        static_cast<Leeds*>(clientData)->vCameras){

        v.setControl(UVC_EXPOSURE,
            static_cast<Leeds*>(clientData)->_cam_exposure);
    }
#endif
}


void Leeds::_setSaturation(const void *value, void *clientData){
     static_cast<Leeds*>(clientData)->_cam_saturation = *(const uint8_t *)value;  // for instance

#ifdef _GEAR_X11_GLX    
    BOOST_FOREACH(gl::VidCam v, 
        static_cast<Leeds*>(clientData)->vCameras){

        v.setControl(UVC_SATURATION,
            static_cast<Leeds*>(clientData)->_cam_saturation);
    }
#endif
}




/*
 * Draw Cameras along bottom of the screen - resizing as needed
 */

 void Leeds::drawCameras() {

    if (vCameras.size() > 0) {

        _shader_camera.bind();
        _shader_camera.s("uBaseTex",0);

        float_t scale = static_cast<float_t>(mScreenW) / (vCameras.size() *  fromStringS9<float_t> (mSettings["leeds/cameras/width"]));
        _camera_quad.setPos(glm::vec3(0.0f, mScreenH - (scale * fromStringS9<float_t> (mSettings["leeds/cameras/height"])), 0.0f) );

        _camera_quad.setScale(glm::vec3(scale,scale,scale));

        for (int i =0; i < vCameras.size(); ++i){
            vCameras[i].bind();
            
            if (i != 0 )
                _camera_quad.move(glm::vec3((fromStringS9<size_t> (mSettings["leeds/cameras/width"]) * scale),0.0f,0.0f));
            
            glm::mat4 mvp = _screen_camera.getMatrix() * _camera_quad.getMatrix();
            _shader_camera.s("uMVPMatrix",mvp);

            _camera_quad.draw();
            vCameras[i].unbind();
            CXGLERROR

        }
        _shader_camera.unbind();
        CXGLERROR
    }
}

/*
 * Create the textured Geometry
 */

 void Leeds::createTextured() {

    // recreate the models 

    _ground_textured_model = _ground_model.getGeometry().convert<GeometryLeeds>();
    _textured_model = _model.getGeometry().convert<GeometryLeeds>();


    // Now create the texture co-ordinates

    for (size_t j=0; j < vCVCameras.size(); j++){

        vector<cv::Point2f> results;
        CVVidCam cam = vCVCameras[j];
        CameraParameters in = cam.getParams();
        vector<cv::Point3f> tOPoints;
        
        // Convert Mesh points to OpenCV and process

        GeometryLeeds geom = _textured_model.getGeometry();


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
        geom = _ground_textured_model.getGeometry();


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
    
    CXGLERROR
}


/*
 * Draw the loaded mesh
 */

void Leeds::drawMesh(Camera &cam){
    glm::mat4 mvp = cam.getMatrix() * _gripper_model.getMatrix();
    glm::mat4 mv = cam.getViewMatrix() * _gripper_model.getMatrix();
    _shader_light.bind();
    glm::mat4 mn =  glm::transpose(glm::inverse(mv));
    _shader_light.s("uMVPMatrix",mvp).s("uShininess",128.0f).s("uMVMatrix",mv)
    .s("uNMatrix",mn).s("uLight0",glm::vec3(15.0,15.0,15.0));
    _model.draw();
    _ground_model.draw();
    _shader_light.unbind();
}


/*
 * Draw the textured mesh with the special geometry
 */


void Leeds::drawTexturedMesh(Camera &cam){
    glm::mat4 mvp = cam.getMatrix() * _gripper_model.getMatrix();
    glm::mat4 mv = cam.getViewMatrix() * _gripper_model.getMatrix();
    _shader_leeds.bind();
    glm::mat4 mn =  glm::transpose(glm::inverse(mv));

    _shader_leeds.s("uMVPMatrix",mvp).s("uShininess",128.0f).s("uMVMatrix",mv)
    .s("uNMatrix",mn).s("uLight0",glm::vec3(15.0,15.0,15.0)).s("uMaxX",640.0f).s("uMaxY",320.0f).s("uShowPos",0);

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
        _shader_leeds.s(str.c_str(), nn);
    }

    _textured_model.draw();
    _ground_textured_model.draw();

    _shader_leeds.unbind();

    for (int i=0; i < vCVCameras.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Leeds::drawGripper(Camera &cam){
    glm::mat4 mvp = cam.getMatrix() * _gripper_model.getMatrix();
    glm::mat4 mv = cam.getViewMatrix() * _gripper_model.getMatrix();
    _shader_light.bind();
    glm::mat4 mn =  glm::transpose(glm::inverse(mv));
    _shader_light.s("uMVPMatrix",mvp).s("uShininess",12.0f).s("uMVMatrix",mv)
    .s("uNMatrix",mn).s("uLight0",glm::vec3(15.0,15.0,15.0));
    _gripper_model.draw();
    _shader_light.unbind();
}


void Leeds::drawToPicker(){
    _shader_pick.bind();
    glm::mat4 mvp = _camera.getMatrix() * _gripper_model.getMatrix();

    _shader_pick.unbind();
}

void Leeds::drawDisplays() {

    _fbo_quad.setPos(glm::vec3(0.0,0.0,0.0));

    glm::mat4 mvp = _screen_camera.getMatrix() * _fbo_quad.getMatrix();

    _shader_camera.bind();
    _shader_camera.s("uBaseTex",0);
    _shader_camera.s("uMVPMatrix",mvp);
    _gripper_fbo.bindColour();

    _fbo_quad.draw();

    _gripper_fbo.unbindColour();

    _overview_fbo.bindColour();

    _fbo_quad.move(glm::vec3(mScreenW - 640.0,0.0,0.0));

    mvp = _screen_camera.getMatrix() * _fbo_quad.getMatrix();
    _shader_camera.s("uMVPMatrix",mvp);

    _fbo_quad.draw();
    _overview_fbo.unbindColour();
    _shader_camera.unbind();
}

/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */

 void Leeds::display(double_t dt){

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );
    glViewport(0,0,mScreenW,mScreenH);

    _shader_basic.bind();

    // Our matrix = the object * camera
    glm::mat4 mvp = _camera.getMatrix() * _test_quad.getMatrix();

    _shader_basic.s("uMVPMatrix",mvp);

    _test_quad.draw();
    _shader_basic.unbind();

    // First, attempt to draw ubermesh
    cv::Point2f p;
    if (_scanner.detectPoint(vCVCameras[0],p))
        cout << "Found Point" << endl;


    if(_textured_model) 
        drawTexturedMesh(_camera);
    else if(_model) 
        drawMesh(_camera);

    // Draw the gripper
    drawGripper(_camera);
    
    // Draw to the picker
    drawToPicker();

    drawCameras();

    // Draw to the gripper view

    _gripper_fbo.bind();
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );
    glViewport(0,0,mScreenW,mScreenH);

    if(_textured_model) 
        drawTexturedMesh(_gripper_camera);
    else if(_model) 
        drawMesh(_gripper_camera);

    drawGripper(_gripper_camera);
    _gripper_fbo.unbind();


    // Draw the overview
    _overview_fbo.bind();

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );
    glViewport(0,0,mScreenW,mScreenH);
   
    if(_textured_model) 
        drawTexturedMesh(_overview_camera);
    else if(_model) 
        drawMesh(_overview_camera);

    drawGripper(_overview_camera);

    _overview_fbo.unbind();

    drawDisplays();

    _camera.update(dt);

    BOOST_FOREACH(VidCam c, vCameras)
    c.update();

    BOOST_FOREACH(CVVidCam c, vCVCameras)
    c.update();

   

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

    mScreenW = e.mW;
    mScreenH = e.mH;
}

void Leeds::processEvent(KeyboardEvent e){
    cout << "Key Pressed: " << e.mKey << endl;

    if (e.mKey == GLFW_KEY_L && e.mAction == 0){
        string s = loadFileDialog();
        if (s != ""){
            cout << "Leeds - Loading " << s << endl;
            _model = gl::GLAsset<GeometryPNF>( AssetImporter::load(s));
        }
    }
    if (e.mKey == GLFW_KEY_R && e.mAction == 0){
        _camera.reset();
        _camera.move(glm::vec3(0.0,0.0,20.0f));
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
    if (mSettings.loadFile(filename)){

        uint32_t w = fromStringS9<uint32_t> ( mSettings["leeds/cameras/width"]);
        uint32_t h = fromStringS9<uint32_t> ( mSettings["leeds/cameras/height"]);
        uint32_t f = fromStringS9<uint32_t> ( mSettings["leeds/cameras/fps"]);
        
        XMLIterator i = mSettings.iterator("leeds/cameras/cam");
        while (i){

            string dev = i["dev"];
            
            VidCam p (dev,w,h,f);
            vCameras.push_back(p);
            
            CVVidCam c(vCameras.back());
            c.loadParameters("./data/" + i["in"]);
            
            vCVCameras.push_back(c);

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