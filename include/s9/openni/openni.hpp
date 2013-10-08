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
#include <vector>
#include <set>

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
 * Relevent data structures from NiTE
typedef struct {
  float x, y, z;
} NitePoint3f;


typedef struct  {
  float x, y, z, w;
} NiteQuaternion;

typedef struct  {
  NiteJointType jointType;
  NitePoint3f position;
  float positionConfidence;
  NiteQuaternion orientation;
  float orientationConfidence;
} NiteSkeletonJoint;

typedef struct  {
  NitePoint3f min;
  NitePoint3f max;
} NiteBoundingBox;

typedef struct{
  NitePoseType type;
  int state;
} NitePoseData;

typedef struct  {
  NiteSkeletonJoint joints[NITE_JOINT_COUNT];
  NiteSkeletonState state;
} NiteSkeleton;

typedef struct  {
  NiteUserId id;
  NiteBoundingBox boundingBox;
  NitePoint3f centerOfMass;

  int state;

  NiteSkeleton skeleton;

  NitePoseData poses[NITE_POSE_COUNT];
} NiteUserData;

typedef struct{
  NiteUserId* pixels;

  int width;
  int height;

  int stride;
} NiteUserMap;

typedef struct  {
  NitePoint3f point;
  NitePoint3f normal;
} NitePlane;

typedef struct  {
  int userCount;
  NiteUserData* pUser;
  NiteUserMap userMap;
  OniFrame* pDepthFrame;
  unsigned long long timestamp;
  int frameIndex;
  float floorConfidence;
  NitePlane floor;
} NiteUserTrackerFrame;

*/


    /**
     * NiTE 2 Tracker that builds on the base and tracks the skeleton
     * Would have extended the nite UserTracker class but decided to poll instead
     */
     
    class SEBUROAPI OpenNISkeleton : public OpenNIBase {
      
    public:
      
      OpenNISkeleton() {};
      OpenNISkeleton(const char * deviceURI);
      ~OpenNISkeleton();
      void update();

      /**
       * A Decorator class around the NiteUserData providing convinience funtions
       */
      struct User : public NiteUserData {

        User (NiteUserData niteuser){
          id = niteuser.id;
          boundingBox = niteuser.boundingBox;
          centerOfMass = niteuser.centerOfMass;
          state = niteuser.state;
          skeleton = niteuser.skeleton;
          poses[0] = niteuser.poses[0];
          poses[1] = niteuser.poses[1];      
        }

        User operator=(NiteUserData niteuser) {
          id = niteuser.id;
          boundingBox = niteuser.boundingBox;
          centerOfMass = niteuser.centerOfMass;
          state = niteuser.state;
          skeleton = niteuser.skeleton;
          poses[0] = niteuser.poses[0];
          poses[1] = niteuser.poses[1];    
        }

        nite::UserId getId() const {return id;}

       // const BoundingBox& getBoundingBox() const {return (const BoundingBox&)boundingBox;}

        
       // const Point3f& getCenterOfMass() const {return (const Point3f&)centerOfMass;}


        bool isNew() const {return (state & NITE_USER_STATE_NEW) != 0;}
        bool isVisible() const {return (state & NITE_USER_STATE_VISIBLE) != 0;}
        bool isLost() const {return (state & NITE_USER_STATE_LOST) != 0;}
        
        const nite::Skeleton& getSkeleton() const {return (const nite::Skeleton&)skeleton;}
        const nite::PoseData& getPose(nite::PoseType type) const {return (const nite::PoseData&)poses[type];}
      

      };

    protected:
      static bool sVisibleUsers[S9_NITE_MAX_USERS];
      static nite::SkeletonState sSkeletonStates[S9_NITE_MAX_USERS];

      nite::Status readFrame();
      nite::Status convertJointCoordinatesToDepth(float x, float y, float z, float* pOutX, float* pOutY) const;
      nite::Status convertDepthCoordinatesToJoint(int x, int y, int z, float* pOutX, float* pOutY) const;
      float getSkeletonSmoothingFactor() const;
      nite::Status setSkeletonSmoothingFactor(float factor);
      void updateUsersState(const User& user, unsigned long long ts);


      // This assumes just one stream / frame as you can imagine
      // I.e one timestamp etc

      std::vector<User> vUsers; // Collection of current users

      NitePlane   mFloor;
      float       mFloorConfidence;
      NiteUserMap         mUserMap;
      unsigned long long  mTimeStamp;

      NiteUserTrackerFrame *pNiteFrame = nullptr; // Current frame


    };
  }

}


#endif