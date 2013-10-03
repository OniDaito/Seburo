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
#include <NiTE.h>

#define S9_OPENNI_MAX_DEPTH 10000
#define S9_NITE_MAX_USERS 10

namespace s9 {


  namespace oni {

    /**
     * Basic OpenNI Entry class that deals with config and middleware
     * \TODO - needs more options! Many more options
     */
     
    class SEBUROAPI OpenNIBase {
    
    protected:
      class SharedObj {
      public:

        openni::VideoFrameRef   mDepthFrame;
        openni::VideoFrameRef   mColourFrame;

        openni::Device          mDevice;
        openni::VideoStream     mDepthStream;
        openni::VideoStream     mColourStream;
        openni::VideoStream**   mStreams;

        // TODO - Depth could be a bit less?
        openni::RGB888Pixel*    pTexMapBuffer;

        // TODO - Add s9 textures

        uint16_t     mWidth;
        uint16_t     mHeight;

        float       mDepthHist[S9_OPENNI_MAX_DEPTH];

        bool        mReady = false;

        // NiTE functionality
        NiteUserTrackerHandle mUserTrackerHandle = nullptr;

        ~SharedObj();

      };

      std::shared_ptr<SharedObj> _obj;


    public:
      OpenNIBase() {};
      OpenNIBase(const char * deviceURI); // openni::ANY_DEVICE normally
      void update();

      static void calculateHistogram(float* pHistogram, int histogramSize, const openni::VideoFrameRef& frame);
   
    };

    /**
     * NiTE 2 Tracker that builds on the base and tracks the skeleton
     * Would have extended the nite UserTracker class but decided to poll instead
     */
     
    class SEBUROAPI OpenNISkeleton : public OpenNIBase {
      
    public:
      
      OpenNISkeleton() {};
      OpenNISkeleton(const char * deviceURI);

    protected:
      static bool sVisibleUsers[S9_NITE_MAX_USERS];
      static nite::SkeletonState sSkeletonStates[S9_NITE_MAX_USERS];


    };
  }

}


#endif