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

namespace po = boost::program_options;


/*
 * Called when the mainloop starts, just once
 */

void Leeds::init(){
    
    mTestQuad = gl::Quad(1.0,1.0);

    mTestQuad.move(glm::vec3(-0.5,-0.5,0.0));

    mCamera.move(glm::vec3(0,0,20.0f));

    mShaderCamera.load("./data/quad_texture.vert", "./data/quad_texture.frag");
    mShaderBasic.load("./data/quad.vert", "./data/quad.frag");
    mShaderLighting.load("./data/basic_lighting.vert", "./data/basic_lighting.frag");

    parseXML("./data/settings.xml");

    mCamQuad = gl::Quad(fromStringS9<float_t> ( mSettings["leeds/cameras/width"]),
        fromStringS9<float_t> ( mSettings["leeds/cameras/height"]));
}

/*
 * Draw Cameras along bottom of the screen - resizing as needed
 */

void Leeds::drawCameras() {
    
    if (vCameras.size() > 0) {

        mShaderCamera.bind();
        mShaderCamera.s("uBaseTex",0);

        float_t scale = static_cast<float_t>(mScreenW) / (vCameras.size() *  fromStringS9<float_t> (mSettings["leeds/cameras/width"]));
        mCamQuad.setPos(glm::vec3(0.0f, mScreenH - (scale * fromStringS9<float_t> (mSettings["leeds/cameras/height"])), 0.0f) );
       
        mCamQuad.setScale(glm::vec3(scale,scale,scale));

        for (int i =0; i < vCameras.size(); ++i){
            vCameras[i].bind();
            
            if (i != 0 )
                mCamQuad.move(glm::vec3((fromStringS9<size_t> (mSettings["leeds/cameras/width"]) * scale),0.0f,0.0f));
            
            glm::mat4 mvp = mScreenCamera.getMatrix() * mCamQuad.getMatrix();
            mShaderCamera.s("uMVPMatrix",mvp);

            mCamQuad.draw();
            vCameras[i].unbind();
            CXGLERROR

        }
        mShaderCamera.unbind();
        CXGLERROR
    }
}




/*
 * Called as fast as possible. Not set FPS wise but dt is passed in
 */
		
void Leeds::display(double_t dt){
    
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)[0]);
    GLfloat depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depth );

    mShaderBasic.bind();

    // Our matrix = the object * camera
    glm::mat4 mvp = mCamera.getMatrix() * mTestQuad.getMatrix();


    mShaderBasic.s("uMVPMatrix",mvp);

    mTestQuad.draw();
    mShaderBasic.unbind();

    // Draw our mesh
    if(mMesh) {
        mShaderLighting.bind();
        glm::mat4 mv = mCamera.getViewMatrix() * mMesh.getMatrix();
        glm::mat4 mn =  glm::transpose(glm::inverse(mv));
        mShaderLighting.s("uMVPMatrix",mvp).s("uShininess",128.0f).s("uMVMatrix",mv)
        .s("uNMatrix",mn).s("uLight0",glm::vec3(15.0,15.0,15.0));

        mMesh.draw();
        mShaderLighting.unbind();
    }

    mCamera.update(dt);

    BOOST_FOREACH(VidCam c, vCameras)
        c.update();

    drawCameras();

    CXGLERROR
}


/*
 * This is called by the wrapper function when an event is fired
 */

void Leeds::fireEvent(MouseEvent &e){
    mCamera.passEvent(e);
}

/*
 * Called when the window is resized. You should set cameras here
 */

void Leeds::fireEvent(ResizeEvent &e){
    glViewport(0,0,e.mW,e.mH);
    mCamera.setRatio( static_cast<float_t>(e.mW) / e.mH);
    mScreenCamera.setDim( e.mW, e.mH);

    mScreenW = e.mW;
    mScreenH = e.mH;
}

void Leeds::fireEvent(KeyboardEvent &e){
    cout << "Key Pressed: " << e.mKey << endl;

    if (e.mKey == 76 && e.mAction == 0){
        string s = loadFileDialog();
        if (s != ""){
            mMesh = gl::Geometry<GeometryPNF>( AssetImporter::load(s));
        }
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

int main (int argc, char * argv[]) {

#ifdef _GEAR_X11_GLX    
    // Linux only - need equivalent for Mac / Windows
    Gtk::Main kit(&argc, &argv);
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

  	GLFWApp a(&b, argc, argv, "Leeds");
  	a.init(4,0); 

    return EXIT_SUCCESS;

}