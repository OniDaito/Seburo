/**
* @brief OpenNI2 Helper Classes
* @file openni.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 18/09/2013
*
*/



#ifndef ONI_OPENNI_HPP
#define ONI_OPENNI_HPP

#include "../common.hpp"

#include <OpenNI.h>

namespace s9 {


  namespace oni {

    /**
     * Basic OpenNI Entry class that deals with config and middleware
     * \TODO - needs more options! Many more options
     */
     
    class SEBUROAPI OpenNIBase {
    
    protected:
      struct SharedObj {

        openni::VideoFrameRef   mDepthFrame;
        openni::VideoFrameRef   mColourFrame;

        openni::Device&         mDevice;
        openni::VideoStream&    mDepthStream;
        openni::VideoStream&    mColourStream;
        openni::VideoStream**   mStreams;

        openni::RGB888Pixel*  m_pTexMap;
        uint16_t     mWidth;
        uint16_t     mHeight;

        SharedObj( openni::Device& d,  openni::VideoStream& ds,
         openni::VideoStream&  cs ) : mDevice(d), mDepthStream(ds), mColourStream(cs) {}

      };

      std::shared_ptr<SharedObj> _obj;

      int init(openni::Device& d, openni::VideoStream&  ds, openni::VideoStream&  cs );

    public:
      OpenNIBase();
      OpenNIBase(std::string device_name);
      ~OpenNIBase();

   
    };
  }

}


#endif