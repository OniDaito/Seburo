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

#include "s9/gl/texture.hpp"

#define S9_OPENNI_MAX_DEPTH 10000
#define S9_NITE_MAX_USERS 10

namespace s9 {

  namespace oni {

    /**
     * Basic OpenNI Entry class that deals with config and middleware
     * \TODO - needs more options! Many more options
     * \todo replace the byte_t buffers with images when we advance the image class
     */
     
    class SEBUROAPI OpenNIBase {
    
    protected:
      class SharedObj {
      public:

        openni::VideoFrameRef   depth_frame_;
        openni::VideoFrameRef   colour_frame_;

        openni::Device          device_;
        openni::VideoStream     depth_stream_;
        openni::VideoStream     colour_stream_;
        openni::VideoStream**   streams_;

        ///\todo - Depth could be a bit less?

        // Seperate buffers as we update OpenGL seperately on a different thread
        byte_t *    tex_buffer_colour_;
        byte_t *    tex_buffer_depth_;

        gl::TextureStream   texture_depth_;
        gl::TextureStream   texture_colour_;

        uint16_t     width_;
        uint16_t     height_;

        float       depth_hist_[S9_OPENNI_MAX_DEPTH];

        bool        ready_ = false;

        ~SharedObj();

      };

      std::shared_ptr<SharedObj> obj_;

    public:
      OpenNIBase() {};
      OpenNIBase(const char * deviceURI); // openni::ANY_DEVICE normally
      
      void update(); // thread safe for update
      void update_textures(); // Update the textures - main thread only
      bool ready() const { return obj_->ready_; }

      const openni::Device& device() const {return obj_->device_; }

      gl::TextureStream texture_depth() {return obj_->texture_depth_; }
      gl::TextureStream texture_colour() {return obj_->texture_colour_; }

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
     * NiTE 2 Tracker that works with the base in order to track skeletons
     * Would have extended the nite UserTracker class but decided to poll instead
     */
     
    class SEBUROAPI OpenNISkeleton {
      
    public:
      
      OpenNISkeleton() {};
      OpenNISkeleton(const OpenNIBase &base);
      ~OpenNISkeleton() {};
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

      struct SharedObject {

        SharedObject(const OpenNIBase &ob) : base(ob) { ready = false; }
        ~SharedObject();

        std::vector<User> vUsers; // Collection of current users

        NitePlane   mFloor;
        float       mFloorConfidence;
        NiteUserMap         mUserMap;
        unsigned long long  mTimeStamp;
        bool  ready;

        NiteUserTrackerHandle user_tracker_handle_ = nullptr;
        NiteUserTrackerFrame *pNiteFrame = nullptr; // Current frame

        const OpenNIBase &base; // reference to the base class

      };


      std::shared_ptr<SharedObject> obj_ = nullptr;


    };
  }

}


#endif