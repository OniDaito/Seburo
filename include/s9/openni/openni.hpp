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
#include "s9/skeleton.hpp"

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
     * This class combines the NiTE skeleton with our more general s9::Skeleton 
     * 
     */
     
    class SEBUROAPI OpenNISkeleton {
      
    public:
      
      OpenNISkeleton() {};
      OpenNISkeleton(const OpenNIBase &base);
      ~OpenNISkeleton() {};
      
      void  update();

      /// A basic typedef that shows the state of the user
      typedef enum {
        BLANK = 0,
        VISIBLE = 1,
        NEW = 2,
        LOST = 4,
      } UserState;
    
    
      /**
       * A Decorator class around the NiteUserData providing convinience funtions
       * and an s9::Skeleton for ease of use.
       */
      class User {

      protected:
        void copySkeleton();

        s9::Skeleton skeleton_; /// The S9 Skeleton - a little nicer than the NiTE one!
        NiteSkeleton skeleton_nite_;
        UserState state_;
        int id_;

      public:

        User() { 
          id_ = -1;
          state_ = BLANK;
        }
        
        User (NiteUserData niteuser){
          id_ = (int)niteuser.id;
          //boundingBox_ = niteuser.boundingBox; ///\todo replace this
          //centerOfMass = niteuser.centerOfMass; ///\todo replace with GLM
          state_ = (UserState)niteuser.state;
          
          skeleton_nite_ = niteuser.skeleton;
          //poses[0] = niteuser.poses[0];
          //poses[1] = niteuser.poses[1];

          skeleton_ = s9::Skeleton(OPENNI_SKELETON);

        }

       /* const User operator= (const User& user) {
          id_ = user.id_;
          state_ = user.state_;
          skeleton_ = user.skeleton_;
          skeleton_nite_ = niteuser.skeleton;
        }*/


        User operator=(NiteUserData niteuser) {
          id_ = (int)niteuser.id;
          state_ = (UserState)niteuser.state;
          skeleton_ = s9::Skeleton(OPENNI_SKELETON);
          skeleton_nite_ = niteuser.skeleton;
        }

        int id() const { return id_; }

        

       // const BoundingBox& getBoundingBox() const {return (const BoundingBox&)boundingBox;}
       // const Point3f& getCenterOfMass() const {return (const Point3f&)centerOfMass;}


        bool isNew() const      {return (state_ & NEW) != 0;}
        bool isVisible() const  {return (state_ & VISIBLE) != 0;}
        bool isLost() const     {return (state_ & LOST) != 0;}
        bool isTracked() const  {return (state_ & VISIBLE) != 0 && skeleton_nite_.state == NITE_SKELETON_TRACKED; }
        
        const nite::Skeleton&   getSkeletonNiTE() const {return (const nite::Skeleton&)skeleton_nite_;}
        //const nite::PoseData&   getPose(nite::PoseType type) const {return (const nite::PoseData&)poses[type];}
        const s9::Skeleton&     skeleton() const {return skeleton_;}

        friend class OpenNISkeleton;
    
      };

      User user(int id); ///\todo a const faster version for direct access?

    protected:
      static bool sVisibleUsers[S9_NITE_MAX_USERS];
      static nite::SkeletonState sSkeletonStates[S9_NITE_MAX_USERS];

      nite::Status  readFrame();
      nite::Status  convertJointCoordinatesToDepth(float x, float y, float z, float* pOutX, float* pOutY) const;
      nite::Status  convertDepthCoordinatesToJoint(int x, int y, int z, float* pOutX, float* pOutY) const;
      float         getSkeletonSmoothingFactor() const;
      nite::Status  setSkeletonSmoothingFactor(float factor);
      void          updateUsersState(const User& user, unsigned long long ts);
      void          setS9Skeleton(const User& user);



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