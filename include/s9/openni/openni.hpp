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
#include <list>

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
     * A reference to a frame caught by the Skeleton tracker
     * Essentially a wrapper class around Nite User Frame
     */


    class OpenNISkeletonFrame {
    public:
 
      OpenNISkeletonFrame() {} 
      ~OpenNISkeletonFrame() {}

 
      bool isValid() const {
        return pNiteFrame != nullptr;
      }


      nite::UserData* getUserById(nite::UserId id) const {
        for (nite::UserData* pid : aUsers){
          if (pid->getId() == id) {
            return pid;
          }
        }
        return aUsers.front(); // Default behaviour
      }

 
      const std::list<nite::UserData*>& getUsers() const {return aUsers;}

 
      float getFloorConfidence() const {return pNiteFrame->floorConfidence;}

      const nite::Plane& getFloor() const {return (const nite::Plane&)pNiteFrame->floor;}


      openni::VideoFrameRef getDepthFrame() {return mDepthFrame;}
 
      uint64_t getTimestamp() const {return pNiteFrame->timestamp;}

      int getFrameIndex() const {return pNiteFrame->frameIndex;}

    private:
      friend class User;
      friend class OpenNISkeleton;

      std::list<nite::UserData*> aUsers; // Nasty - pointers within a memory location

      NiteUserTrackerFrame *pNiteFrame = nullptr;
      openni::VideoFrameRef mDepthFrame;
    };



    /**
     * NiTE 2 Tracker that builds on the base and tracks the skeleton
     * Would have extended the nite UserTracker class but decided to poll instead
     */
     
    class SEBUROAPI OpenNISkeleton : public OpenNIBase {
      
    public:
      
      OpenNISkeleton() {};
      OpenNISkeleton(const char * deviceURI);
      void update();

    protected:
      static bool sVisibleUsers[S9_NITE_MAX_USERS];
      static nite::SkeletonState sSkeletonStates[S9_NITE_MAX_USERS];

      nite::Status readFrame();
      nite::Status startSkeletonTracking(nite::UserId id);
      void stopSkeletonTracking(nite::UserId id);
      nite::Status startPoseDetection(nite::UserId user, nite::PoseType type);
      void stopPoseDetection(nite::UserId user, nite::PoseType type);
      nite::Status convertJointCoordinatesToDepth(float x, float y, float z, float* pOutX, float* pOutY) const;
      nite::Status convertDepthCoordinatesToJoint(int x, int y, int z, float* pOutX, float* pOutY) const;
      float getSkeletonSmoothingFactor() const;
      nite::Status setSkeletonSmoothingFactor(float factor);
      void updateUserState(const nite::UserData& user, unsigned long long ts);


      /**
       * Internal Class that provides callbacks for new frames
       */

      class NewFrameListener {
      public:
     
        NewFrameListener(OpenNISkeleton &tracker) : mUserTracker(tracker) {
          mUserTrackerCallbacks.readyForNextFrame = newFrameCallback;
        }

        void onNewFrame(OpenNISkeleton&);

      private:
        NiteUserTrackerCallbacks mUserTrackerCallbacks;
        NiteUserTrackerCallbacks& getCallbacks() {return mUserTrackerCallbacks;}

        static void ONI_CALLBACK_TYPE newFrameCallback(void* pCookie) {
          NewFrameListener* pListener = (NewFrameListener*)pCookie;
          pListener->onNewFrame(pListener->mUserTracker);
        }

        friend class OpenNISkeleton;

        OpenNISkeleton &mUserTracker;
      };


      std::shared_ptr<NewFrameListener> pFrameListener;
      std::shared_ptr<OpenNISkeletonFrame> pFrame;

  
    };
  }

}


#endif