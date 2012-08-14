
/**
* @brief States for the Leeds App
* @file states.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/09/2012
*
*/

#include "states.hpp"


using namespace std;
using namespace boost;
using namespace boost::assign;
using namespace s9;
using namespace s9::gl;
using namespace s9::compvis;
using namespace s9::gl::compvis;
using namespace cv;
using namespace leeds;


/*
 * Draw Cameras along bottom of the screen - resizing as needed
 */

void drawCameras(SharedObjPtr obj, vector<Texture> textures, glm::vec2 offset = glm::vec2(0,0)) {

    if (obj->_cameras.size() > 0) {

        obj->_shader_camera.bind();
        obj->_shader_camera.s("uBaseTex",0);

        float_t scale =  static_cast<float_t>(obj->_screen_width) / (obj->_cameras.size() *  fromStringS9<float_t> (obj->_settings["leeds/cameras/width"]));
    
        obj->_camera_quad.setPos(glm::vec3(0.0f, obj->_screen_height - (scale * fromStringS9<float_t> (obj->_settings["leeds/cameras/height"])), 0.0f) );
        obj->_camera_quad.setScale(glm::vec3(scale,scale,scale));

        obj->_camera_quad.move(glm::vec3(offset.x * scale, offset.y * scale,0.0));


        for (int i =0; i < textures.size(); ++i){
            textures[i].bind();
            
            glm::vec3 m (fromStringS9<size_t> (obj->_settings["leeds/cameras/width"]) * scale,0.0f,0.0f);
            if (i != 0 )
                obj->_camera_quad.move(m);
            
            glm::mat4 mvp = obj->_screen_camera.getMatrix() * obj->_camera_quad.getMatrix();
            obj->_shader_camera.s("uMVPMatrix",mvp);

            obj->_camera_quad.draw();
            textures[i].unbind();
            CXGLERROR

        }
        obj->_shader_camera.unbind();
        CXGLERROR
    }
}



/*
 * Draw the loaded mesh
 */

void drawMesh(SharedObjPtr obj, Camera &cam){
    glm::mat4 mvp = cam.getMatrix() * obj->_model.getMatrix();
    glm::mat4 mv = cam.getViewMatrix() * obj->_model.getMatrix();
    obj->_shader_light.bind();
    glm::mat4 mn =  glm::transpose(glm::inverse(mv));
    obj->_shader_light.s("uMVPMatrix",mvp).s("uShininess",128.0f).s("uMVMatrix",mv)
    .s("uNMatrix",mn).s("uLight0",glm::vec3(15.0,15.0,15.0));
    obj->_model.draw();
    obj->_ground_model.draw();
    obj->_shader_light.unbind();
}


/*
 * Draw the textured mesh with the special geometry
 */


void drawTexturedMesh(SharedObjPtr obj, Camera &cam){
    glm::mat4 mvp = cam.getMatrix() * obj->_textured_model.getMatrix();
    glm::mat4 mv = cam.getViewMatrix() * obj->_textured_model.getMatrix();
    obj->_shader_leeds.bind();
    glm::mat4 mn =  glm::transpose(glm::inverse(mv));

    obj->_shader_leeds.s("uMVPMatrix",mvp).s("uShininess",128.0f).s("uMVMatrix",mv)
    .s("uNMatrix",mn).s("uLight0",glm::vec3(15.0,15.0,15.0)).s("uMaxX",640.0f).s("uMaxY",320.0f).s("uShowPos",0);

    for (int i=0; i < obj->_cv_cameras.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i);
        obj->_cv_cameras[i].bind();
        
        std::stringstream Num;
        std::string str;
        Num << i;
        str = "uCam" + Num.str();
        
        cv::Mat n = obj->_cv_cameras[i].getNormal();
        glm::vec3 nn (n.at<double_t>(0,0),n.at<double_t>(1,0),n.at<double_t>(2,0));
        nn  = glm::normalize(nn);
        obj->_shader_leeds.s(str.c_str(), nn);
    }

    obj->_textured_model.draw();
    obj->_ground_textured_model.draw();

    obj->_shader_leeds.unbind();

    for (int i=0; i < obj->_cv_cameras.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glActiveTexture(GL_TEXTURE0);
}

void drawGripper(SharedObjPtr obj, Camera &cam){
    glm::mat4 mvp = cam.getMatrix() * obj->_gripper_model.getMatrix();
    glm::mat4 mv = cam.getViewMatrix() * obj->_gripper_model.getMatrix();
    obj->_shader_light.bind();
    glm::mat4 mn =  glm::transpose(glm::inverse(mv));
    obj->_shader_light.s("uMVPMatrix",mvp).s("uShininess",12.0f).s("uMVMatrix",mv)
    .s("uNMatrix",mn).s("uLight0",glm::vec3(15.0,15.0,15.0));
    obj->_gripper_model.draw();
    obj->_shader_light.unbind();
}


void drawToPicker(SharedObjPtr obj){
    obj->_shader_pick.bind();
    obj->_picking_fbo.bind();
    float_t depth = 1.0;
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );

    obj->_shader_pick.s("uColour",obj->_gripper_model.getColour());
    glm::mat4 mvp = obj->_camera.getMatrix() * obj->_gripper_model.getMatrix();
    obj->_shader_pick.s("uMVPMatrix",mvp);

    obj->_gripper_model.draw();
    obj->_shader_pick.unbind();
    obj->_picking_fbo.unbind();
}

void drawDisplays(SharedObjPtr obj) {

    obj->_fbo_quad.setPos(glm::vec3(0.0,0.0,0.0));

    glm::mat4 mvp = obj->_screen_camera.getMatrix() * obj->_fbo_quad.getMatrix();
    glViewport(0,0,obj->_screen_width,obj->_screen_height);

    obj->_shader_camera.bind();
    obj->_shader_camera.s("uBaseTex",0);
    obj->_shader_camera.s("uMVPMatrix",mvp);
    obj->_gripper_fbo.bindColour();
    obj->_fbo_quad.draw();


    obj->_overview_fbo.bindColour();
    obj->_fbo_quad.move(glm::vec3(obj->_screen_width - 320.0,0.0,0.0));
    mvp = obj->_screen_camera.getMatrix() * obj->_fbo_quad.getMatrix();
    obj->_shader_camera.s("uMVPMatrix",mvp);
    obj->_fbo_quad.draw();
    obj->_overview_fbo.unbindColour();

    obj->_shader_camera.unbind();
}


void drawReference(SharedObjPtr obj) {
    obj->_shader_basic.bind();

    // Our matrix = the object * camera
    glm::mat4 mvp = obj->_camera.getMatrix() * obj->_test_quad.getMatrix();

    obj->_shader_basic.s("uMVPMatrix",mvp);

    obj->_test_quad.draw();
    obj->_shader_basic.unbind(); 
}


/*
 * Initial State
 */


void StateInitial::_update(double_t dt){
}

static float_t msense = 0.1;

void StateInitial::_draw(double_t dt){
    SharedObjPtr obj = static_pointer_cast<SharedObj>(_data);
    drawReference(obj);

    vector<Texture> textures;
    BOOST_FOREACH(VidCam c, obj->_cameras)
        textures.push_back(c.getTexture());

    drawCameras(obj,textures);
    drawGripper(obj,obj->_camera);
    drawToPicker(obj);

    // Cant call update for cameras as they are GL! 
    BOOST_FOREACH(VidCam c, obj->_cameras)
    c.update();
    BOOST_FOREACH(CVVidCam c, obj->_cv_cameras)
    c.update();
}

/*
 * Basic mouse event processing that isnt part of the normal
 * movememnt. Selection and movement of the gripper
 */

void StateInitial::processEvent(MouseEvent e) {
    SharedObjPtr obj = static_pointer_cast<SharedObj>(_data);

    if (e.mFlag & MOUSE_MIDDLE_DOWN && _picked){
        // we are picking so move

        glm::vec2 mouse_d = glm::vec2(e.mX,e.mY) - _mouse_pre;
        glm::vec3 dir = obj->_camera.getPos() - obj->_camera.getLook();
        dir = glm::normalize(dir);
        glm::vec3 shiftx = glm::cross(dir,obj->_camera.getUp());
        shiftx *= (-mouse_d.x * msense);
        glm::vec3 shifty = obj->_camera.getUp() * (-mouse_d.y * msense);

        glm::vec3 dpos = shiftx + shifty;
        obj->_gripper_model.move(dpos);
        _mouse_pre = glm::vec2(e.mX,e.mY);
    
    } 

    if(e.mFlag & MOUSE_LEFT_DOWN){

        // test to see if anything has been picked
        obj->_picking_fbo.bind();
        glReadBuffer(GL_COLOR_ATTACHMENT0);
            
        GLfloat *data = (GLfloat*) new GLfloat[4];
        glReadPixels(e.mX, obj->_screen_height - e.mY, 1, 1, GL_RGBA, GL_FLOAT, data);
        glm::vec4 c( data[0], data[1], data[2], data[3]);
        if (c == obj->_gripper_model.getColour()){
            _picked = true;
            _mouse_pre = glm::vec2(e.mX,e.mY);
        }
        else
            _picked = false;

        obj->_picking_fbo.unbind();
    }

    if (!_picked)
        obj->_camera.processEvent(e);

    CXGLERROR
};


void StateInitial::processEvent(ResizeEvent e) { 
    SharedObjPtr obj = static_pointer_cast<SharedObj>(_data);
    obj->_camera.setRatio( static_cast<float_t>(e.mW) / e.mH);
}


/*
 * Having loaded a mesh, draw this
 */


void StateDrawMesh::_update(double_t dt){

} 

void StateDrawMesh::_draw(double_t dt){
    SharedObjPtr obj = static_pointer_cast<SharedObj>(_data);
    drawMesh(obj,obj->_camera);


    // Draw to the gripper view
    GLfloat depth = 1.0;
    obj->_gripper_camera.align(obj->_gripper_model);
    obj->_gripper_camera.move(glm::vec3(5.0,0.0,2.0));
    obj->_gripper_camera.yaw(90.0);

    obj->_gripper_fbo.bind();
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );
    drawMesh(obj, obj->_gripper_camera);
    drawGripper(obj, obj->_gripper_camera);
    obj->_gripper_fbo.unbind();


    // Draw the overview
    obj->_overview_fbo.bind();

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );
 
    drawMesh(obj, obj->_overview_camera);

    drawGripper(obj, obj->_overview_camera);

    obj->_overview_fbo.unbind();

    drawDisplays(obj);
}

/*
 * IDraw Textured if we can
 */


void StateDrawTexturedMesh::_update(double_t dt){
    
}

void StateDrawTexturedMesh::_draw(double_t dt){
  SharedObjPtr obj = static_pointer_cast<SharedObj>(_data);
    drawTexturedMesh(obj,obj->_camera);


    // Draw to the gripper view
    GLfloat depth = 1.0;
    obj->_gripper_camera.align(obj->_gripper_model);
    obj->_gripper_camera.move(glm::vec3(5.0,0.0,2.0));
    obj->_gripper_camera.yaw(90.0);

    obj->_gripper_fbo.bind();
    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );
    drawTexturedMesh(obj, obj->_gripper_camera);
    drawGripper(obj, obj->_gripper_camera);
    obj->_gripper_fbo.unbind();


    // Draw the overview
    obj->_overview_fbo.bind();

    glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)[0]);
    glClearBufferfv(GL_DEPTH, 0, &depth );
 
    drawTexturedMesh(obj, obj->_overview_camera);

    drawGripper(obj, obj->_overview_camera);

    obj->_overview_fbo.unbind();

    drawDisplays(obj);
}

/*
 * Start Scanning
 */

StateScan::StateScan(SharedObjPtr p) : State(static_cast<boost::shared_ptr <void> > (p)) {
    // Create 8 new textures for our drawing section
    BOOST_FOREACH(CVVidCam c, p->_cv_cameras) {
        _results.push_back( Texture(c.getSize()) );
        cv::Mat tm = cv::Mat(cv::Size(c.getSize().x, c.getSize().y),CV_8UC3);
        _result_matrices.push_back(tm);
    }

    // Fire up our geometry - non indexed with initially 200 points
    vector<VertPF> tv;
    for (int i=0; i < 200; i ++){
        VertPF tp;
        tv.push_back(tp);
    }
    p->_points = GeometryPF(tv);
    p->_points.setNumElements(0);
    p->_point_mesh = GLAsset<GeometryPF>(p->_points);   
}

void StateScan::_update(double_t dt){
    SharedObjPtr obj = static_pointer_cast<SharedObj>(_data);
    vector< std::pair<cv::Point2f, gl::compvis::CameraParameters> > points;

    int i=0;

    BOOST_FOREACH(CVVidCam c, obj->_cv_cameras) {
        cv::Point2f p;
        cv::Mat result;

        if (obj->_scanner.detectPoint(c,p,result)){
            points.push_back(pair<cv::Point2f, gl::compvis::CameraParameters>(p,c.getParams()));
        }  
        _result_matrices[i] = result;
        ++i;
    }

    if (points.size() > 1){
        cv::Point3f p = obj->_scanner.solveForAll(points); 
        cout << p << endl;

        VertPF nv;
        nv.mP.x = p.x;
        nv.mP.y = p.y;
        nv.mP.z = p.z;

        // Resize the buffer for more space
        if (obj->_points.numElements() >= (obj->_points.size() * 0.6)){
            VertPF np;
            int range = obj->_points.size();
            for (int i=0; i < range; ++i){
                obj->_points.addVertex(np);
            }
        }

        obj->_points.setVertex(nv,obj->_points.numElements());
        obj->_points.setNumElements(obj->_points.numElements()+1);

    }

}

void StateScan::_draw(double_t dt){

    int i = 0;
    BOOST_FOREACH(cv::Mat result, _result_matrices) {
        _results[i].update(MatToGL(result));
        ++i;
    }

    SharedObjPtr obj = static_pointer_cast<SharedObj>(_data);
    drawCameras(obj,_results, glm::vec2(0.0, -360.0));
  //  glEnable(GL_BLEND);
  //  glBlendFunc(GL_ONE,GL_SRC_ALPHA);
    glPointSize(2.0f);
    // Draw the mesh
    obj->_shader_point.bind();

    glm::mat4 mvp = obj->_camera.getMatrix() * obj->_point_mesh.getMatrix();
    obj->_shader_point.s("uMVPMatrix",mvp)
        .s("uColour",glm::vec4(0.12,0.31,0.95,0.91));

    obj->_point_mesh.draw(GL_POINTS);

    obj->_shader_point.unbind();
  //  glDisable(GL_BLEND);
}

/*
 * Calibrate to the world
 */


StateCalibrate::StateCalibrate(SharedObjPtr p) : State(static_cast<boost::shared_ptr <void> > (p)) {

    BOOST_FOREACH(CVVidCam c, p->_cv_cameras) {
        cv::Mat tm = cv::Mat(cv::Size(c.getSize().x, c.getSize().y),CV_8UC3);
        _results.push_back(tm);
    }
}


void StateCalibrate::_update(double_t dt){
    SharedObjPtr obj = static_pointer_cast<SharedObj>(_data);
    
    _calibrated = true;
  
    BOOST_FOREACH(CVVidCam cam, obj->_cv_cameras) {

        if (findChessboard(cam, corners, results[i], _board_size)){
          
          _image_points.push_back(corners);    
          
          for(int j=0;j< _board_size.height * _board_size.width; j++)
            tOPoints.push_back(Point3f(j/_board_size.width, j%_board_size.width, 0.0f));
      
          CameraParameters cp = cam.getParams();

          solvePnP(tOPoints,imagePoints[0],cp.M, cp.D, cp.R, cp.T, false, CV_ITERATIVE);
          ++i;
        }
        else
            _calibrated = false;
    }

    if (_calibrated){
        cout << "Calibrated to World" << endl;
        remove();
    }
}

void StateCalibrate::_draw(double_t dt){
    
}


/*
 * Use PCL to generate a mesh. This State blokcs and removes itself
 * when finished
 */


void StateGenerateMesh::_update(double_t dt){
    SharedObjPtr obj = static_pointer_cast<SharedObj>(_data);
    
    if(obj->_points) {
        cout << "Generating Mesh" << endl;
        obj->_model = obj->_mesher.generate(obj->_points);
    }

    remove();
    cout << "Removed State" << endl;
}

void StateGenerateMesh::_draw(double_t dt){

}

