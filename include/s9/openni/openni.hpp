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

#define S9_OPENNI_MAX_DEPTH 10000

namespace s9 {


  namespace oni {

    /**
     * Basic OpenNI Entry class that deals with config and middleware
     * \TODO - needs more options! Many more options
     */
     
    class SEBUROAPI OpenNIBase {
    
    public:
      struct SharedObj {

        openni::VideoFrameRef   mDepthFrame;
        openni::VideoFrameRef   mColourFrame;

        openni::Device          mDevice;
        openni::VideoStream     mDepthStream;
        openni::VideoStream     mColourStream;
        openni::VideoStream**   mStreams;

        openni::RGB888Pixel*    pTexMap;
        uint16_t     mWidth;
        uint16_t     mHeight;

        float         mDepthHist[S9_OPENNI_MAX_DEPTH];

        ~SharedObj();

      };

      std::shared_ptr<SharedObj> _obj;

      void init();

    public:
      OpenNIBase() {};
      OpenNIBase(const char * deviceURI); // openni::ANY_DEVICE normally
      void update();

      static void calculateHistogram(float* pHistogram, int histogramSize, const openni::VideoFrameRef& frame);
   
    };
  }

}


#endif