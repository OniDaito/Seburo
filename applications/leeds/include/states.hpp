/*
* @brief Leeds States
* @file states.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/08/2012
*
*/

#ifndef LEEDS_STATES_HPP
#define LEEDS_STATES_HPP

#include "s9/common.hpp"
#include "s9/camera.hpp"
#include "s9/s9xml.hpp"
#include "s9/gl/shapes.hpp"
#include "s9/gl/shader.hpp"
#include "s9/gl/video.hpp"
#include "s9/gl/glasset.hpp"
#include "s9/gl/glfw_app.hpp"
#include "s9/gl/fbo.hpp"


#ifdef _GEAR_X11_GLX  
#include "s9/linux/gtk_functions.hpp"
#include "s9/linux/defines.hpp"
#endif

#include "s9/state.hpp"
#include "s9/events.hpp"
#include "scanner.hpp"


using namespace s9;

 
namespace leeds {

  // special typedefs
  typedef Geometry<VertPNT8F> GeometryLeeds;

  // This shared object exports the state of the system

  struct SharedObj{

    // Geometry
    gl::Quad _test_quad;
    gl::Quad _camera_quad;
    gl::Quad _fbo_quad;
    gl::GLAsset<GeometryPNF> _gripper_model;
    gl::GLAsset<GeometryPNF> _ground_model;
    gl::GLAsset<GeometryLeeds> _ground_textured_model;
    gl::GLAsset<GeometryLeeds> _textured_model;
    gl::GLAsset<GeometryPNF> _model;

    // Cameras
    OrbitCamera _camera;
    Camera _gripper_camera;
    Camera _overview_camera;
    ScreenCamera _screen_camera;

    // Settings
    XMLSettings _settings;

    // Video Cameras
    std::vector<gl::VidCam> _cameras;
    std::vector<gl::compvis::CVVidCam> _cv_cameras;

    // Shaders
    gl::Shader _shader_camera;
    gl::Shader _shader_basic;
    gl::Shader _shader_light;
    gl::Shader _shader_leeds;
    gl::Shader _shader_pick;

    //FBOs
    gl::FBO   _gripper_fbo;
    gl::FBO   _overview_fbo;
    gl::FBO   _picking_fbo;

    // Video Camera settings
    uint8_t _cam_contrast;
    uint8_t _cam_gain;
    uint8_t _cam_brightness;
    uint8_t _cam_exposure;
    uint8_t _cam_saturation;

    uint32_t _screen_width, _screen_height;

    // Mesh Reconstruction

    Scanner _scanner;
  };

  typedef boost::shared_ptr<SharedObj> SharedObjPtr;

  /*
   * Initial State of the System
   */

  class StateInitial : public State, public WindowResponder {
  public:
    StateInitial(SharedObjPtr p) : 
      State(static_cast<boost::shared_ptr <void> > (p)) {
        _active = true;
        _picked = false;
        _mouse_pre = glm::vec2(0.0,0.0);
      };

    void processEvent(MouseEvent e);
    void processEvent(ResizeEvent e);


  protected:
    void _update(double_t dt);

    void _draw(double_t dt);
    bool _picked;
    glm::vec2 _mouse_pre;
  };

  /*
   * Drawing the mesh we want
   */

  class StateDrawMesh : public State {
  public:
    StateDrawMesh(SharedObjPtr p) : 
      State(static_cast<boost::shared_ptr <void> > (p)) {};

  protected:
    void _update(double_t dt);
    void _draw(double_t dt);
  };

  /*
   * Drawing the textured mesh
   */

  class StateDrawTexturedMesh : public State {
  public:
    StateDrawTexturedMesh(SharedObjPtr p) :
      State(static_cast<boost::shared_ptr <void> > (p)) {};

  protected:
    void _update(double_t dt);
    void _draw(double_t dt);
  };

  /*
   * Scanning State with the laser
   */

  class StateScan : public State {
  public:
    StateScan(SharedObjPtr p);

  protected:
    void _update(double_t dt);
    void _draw(double_t dt);

    std::vector<s9::gl::Texture> _results;
  };

  /*
   * Calibrating the world state
   */

  class StateCalibrate : public State {
  public:
    StateCalibrate(SharedObjPtr p) :
      State(static_cast<boost::shared_ptr <void> > (p)) {};

  protected:
    void _update(double_t dt);
    void _draw(double_t dt);
  };

  /*
   * Generate the mesh using PCL
   */

  class StateGenerateMesh : public State {
  public:
    StateGenerateMesh(SharedObjPtr p) :
      State(static_cast<boost::shared_ptr <void> > (p)) {};

  protected:
    void _update(double_t dt);
    void _draw(double_t dt);
  };

}


#endif