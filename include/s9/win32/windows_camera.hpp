/**
* @brief Windows Based Camera
* @file windows_camera.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 15/05/2013
*
*/

#ifndef S9_WINDOWS_CAMERA
#define S9_WINDOWS_CAMERA

#include "s9/common.hpp"

//\TODO - Complete this class at some point 

namespace s9{

  class WindowsCamera {
  public:
    WindowsCamera () {};
    bool startCapture(std::string devname, unsigned int width, unsigned int height, unsigned int fps);
    void stop() {};
    unsigned char* getBuffer() { return NULL; };

    void video_list_controls(int dev) {};
    void set_control(unsigned int id, int value) {};

  protected:
  };
}


#endif