/**
* @brief Quicktime Camera controller for Linux
* @file quicktime_camera.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/05/2012
*
*/

#ifndef S9_OSX_QUICKTIME
#define S9_OSX_QUICKTIME

#include "s9/common.hpp"

namespace s9{

  class QuicktimeCamera {
  public:
    QuicktimeCamera () {};
    bool StartCapture(std::string devname, unsigned int width, unsigned int height, unsigned int fps);
    void Stop();
    unsigned char* GetBuffer();

    void video_list_controls(int dev);
    void set_control(unsigned int id, int value);

  protected:
  };
}


#endif
