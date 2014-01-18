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

      struct SharedObject {

        openni::VideoFrameRef   depth_frame;
        openni::VideoFrameRef   colour_frame;

        openni::Device          device;
        openni::VideoStream     depth_stream;
        openni::VideoStream     colour_stream;
        openni::VideoStream**   streams;

        ///\todo - Depth could be a bit less?

        // Seperate buffers as we update OpenGL seperately on a different thread
        byte_t *    tex_buffer_colour;
        byte_t *    tex_buffer_depth;

        gl::TextureStream   texture_depth;
        gl::TextureStream   texture_colour;

        uint16_t     width;
        uint16_t     height;

        float       depth_hist[S9_OPENNI_MAX_DEPTH];

        bool        ready = false;

        ~SharedObject();

      };

      std::shared_ptr<SharedObject> obj_;

    public:
      OpenNIBase() {};
      OpenNIBase(const char * deviceURI); // openni::ANY_DEVICE normally
      
      void Update(); // thread safe for update
      void UpdateTextures(); // Update the textures - main thread only
      bool ready() const { return obj_->ready; }

      const openni::Device& device() const {return obj_->device; }

      gl::TextureStream texture_depth() {return obj_->texture_depth; }
      gl::TextureStream texture_colour() {return obj_->texture_colour; }

      static void CalculateHistogram(float* pHistogram, int histogramSize, const openni::VideoFrameRef& frame);
   
    };

  
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
      
      void  Update();

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
        void CopySkeleton();

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


        bool IsNew() const      {return (state_ & NEW) != 0;}
        bool IsVisible() const  {return (state_ & VISIBLE) != 0;}
        bool IsLost() const     {return (state_ & LOST) != 0;}
        bool IsTracked() const  {return (state_ & VISIBLE) != 0 && skeleton_nite_.state == NITE_SKELETON_TRACKED; }
        
        const nite::Skeleton&   skeleton_nite() const {return (const nite::Skeleton&)skeleton_nite_;}
        //const nite::PoseData&   getPose(nite::PoseType type) const {return (const nite::PoseData&)poses[type];}
        const s9::Skeleton&     skeleton() const {return skeleton_;}

        friend class OpenNISkeleton;
    
      };

      User GetUserByID(int id); ///\todo a const faster version for direct access?
      void RestartTracking();

    protected:
      static bool sVisibleUsers[S9_NITE_MAX_USERS];
      static nite::SkeletonState sSkeletonStates[S9_NITE_MAX_USERS];

      nite::Status  ReadFrame();
      nite::Status  ConvertJointCoordinatesToDepth(float x, float y, float z, float* pOutX, float* pOutY) const;
      nite::Status  ConvertDepthCoordinatesToJoint(int x, int y, int z, float* pOutX, float* pOutY) const;
      float         GetSkeletonSmoothingFactor() const;
      nite::Status  SetSkeletonSmoothingFactor(float factor);
      void          UpdateUsersState(const User& user, unsigned long long ts);
      void          SetS9Skeleton(const User& user);
      void          Init();



      // This assumes just one stream / frame as you can imagine
      // I.e one timestamp etc

      struct SharedObject {

        SharedObject(const OpenNIBase &ob) : base(ob) { ready = false; }
        ~SharedObject();

        std::vector<User> users; // Collection of current users

        NitePlane  floor;
        float       floor_confidence;
        NiteUserMap         user_map;
        unsigned long long  timestamp;
        bool  ready;

        NiteUserTrackerHandle user_tracker_handle = nullptr;
        NiteUserTrackerFrame *pNiteFrame = nullptr; // Current frame


        const OpenNIBase &base; // reference to the base class

      };


      std::shared_ptr<SharedObject> obj_ = nullptr;


    };
  }

}


#endif