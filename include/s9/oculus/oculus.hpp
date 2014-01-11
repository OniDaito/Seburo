/**
* @brief Oculus Helper Classes
* @file oculus.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 20/09/2013
*
*/

#ifndef S9_OCULUS_OCULUS_HPP
#define S9_OCULUS_OCULUS_HPP

#include "../common.hpp"

#include "../math_utils.hpp"
#include "../string_utils.hpp"

#include <OVR.h>
#include <OVRVersion.h>

namespace s9 {


  namespace oculus {

    glm::mat4 ToGLMmatrix(OVR::Matrix4f m) {
      return glm::mat4(m.M[0][0], m.M[1][0], m.M[2][0], m.M[3][0],
        m.M[0][1], m.M[1][1], m.M[2][1], m.M[3][1],
        m.M[0][2], m.M[1][2], m.M[2][2], m.M[3][2],
        m.M[0][3], m.M[1][3], m.M[2][3], m.M[3][3]);
    }

    /**
     * Basic Oculus Entry class that deals with config
     * it listens for messages also.
     */
     
    ///\todo we need to pass out the messages recieved - create some kind of handler mechanism
    // Oculus Base can probably extend for window listening or similar and pass on from its shared object

    class SEBUROAPI OculusBase  {
    
    protected:


      /// Internal object that can be shared around other instances. Watches after an oculus rift.
      struct SharedObject : public OVR::MessageHandler {

        ///\todo we need better parameters to pass in I think since we dont use the planes anymore :(
        SharedObject(float_t near, float_t far);

        void OnMessage(const OVR::Message& msg);
        void update(double_t dt);
        bool connected() { return IsHandlerInstalled(); }


        ~SharedObject() {
          RemoveHandlerFromDevices();
          latency_tester.Clear();
          sensor.Clear();
          HMD.Clear();

          std::cout << "SEBURO OCCULUS - Shutting down oculus manager." << std::endl;
        }


        struct DeviceStatusNotificationDesc {
          OVR::DeviceHandle    Handle;
          OVR::MessageType     Action;

          DeviceStatusNotificationDesc():Action(OVR::Message_None) {}
          DeviceStatusNotificationDesc(OVR::MessageType mt, const OVR::DeviceHandle& dev) 
            : Handle(dev), Action(mt) {}
        };
    
      
        std::vector<OculusBase*> handlers;
       
        OVR::Ptr<OVR::DeviceManager> manager;
        OVR::Ptr<OVR::HMDDevice> HMD;
        OVR::Ptr<OVR::SensorDevice> sensor; 
        OVR::Ptr<OVR::Profile> profile;

        OVR::Util::Render::StereoConfig     stereo_config;
        OVR::Util::LatencyTest              latency_util;
        OVR::Ptr<OVR::LatencyTestDevice>    latency_tester;
        OVR::Util::MagCalibration           mag_cal;

        OVR::Array<DeviceStatusNotificationDesc> device_status_notifications_queue; 

        OVR::SensorFusion fusion;
        OVR::HMDInfo hmd_info;
        glm::quat orientation;


        std::string monitor_name;

        // Distortion parameters
        glm::ivec4 left_eye_viewport;
        glm::ivec4 right_eye_viewport;

        bool initialized;

        // Screen parameters - scaled for final output
        glm::vec2 fbo_size;
        glm::vec2 screen_size_scaled;

        // Cameras and matrices for our final output

        float_t near_plane, far_plane;

      };

      std::shared_ptr<SharedObject> obj_ = nullptr;

    public:
  
      OculusBase() {}
      OculusBase(float_t near, float_t far);
      ~OculusBase() { }

      bool operator == (const OculusBase &ref) const { return this->obj_ == ref.obj_; }
      typedef std::shared_ptr<SharedObject> OculusBase::*unspecified_bool_type;
      operator unspecified_bool_type() const { return ( obj_.get() == 0 ) ? 0 : &OculusBase::obj_; }
      void reset() { obj_.reset(); }

      void update(double_t dt) { CXSHARED obj_->update(dt); }

      bool connected() { CXSHARED return (obj_->connected() && obj_->initialized);  }

      glm::quat orientation() {CXSHARED return obj_->orientation;  } 
     
      glm::ivec4 left_eye_viewport() { CXSHARED return obj_->left_eye_viewport; }
      glm::ivec4 right_eye_viewport() { CXSHARED return obj_->right_eye_viewport;  }

      std::string monitor_name()  { CXSHARED return obj_->monitor_name;  }


      glm::vec4 distortion_parameters() {
        CXSHARED
        return glm::vec4(obj_->hmd_info.DistortionK[0], obj_->hmd_info.DistortionK[1],
          obj_->hmd_info.DistortionK[2],obj_->hmd_info.DistortionK[3]);
    
      }

      glm::vec4 chromatic_abberation() { 
       CXSHARED
        OVR::Util::Render::DistortionConfig dc = obj_->stereo_config.GetDistortionConfig();
        return glm::vec4(dc.ChromaticAberration[0], dc.ChromaticAberration[1],
            dc.ChromaticAberration[2],dc.ChromaticAberration[3]);
        
        
      }

      float distortion_xcenter_offset() { 
        CXSHARED 
        return obj_->stereo_config.GetDistortionConfig().XCenterOffset;
      }

      float distortion_scale() { 
        CXSHARED
        return obj_->stereo_config.GetDistortionScale();
      }

      /// Return the interpupillary distance if the device exists
      float interpupillary_distance() {
        CXSHARED
        return obj_->hmd_info.InterpupillaryDistance;
      }

      /// Return the lens separation distance if the device exists
      float lens_separation_distance() {
        CXSHARED
        return obj_->hmd_info.LensSeparationDistance;
      }

      /// Return the eye to screen distance if the device exists
      float eye_to_screen_distance() {
        CXSHARED
        return obj_->hmd_info.EyeToScreenDistance;
      }

      /// Return the screen center if the device exists
      glm::vec2 screen_center() {
        CXSHARED
        return glm::vec2(obj_->hmd_info.HScreenSize / 2.0, obj_->hmd_info.VScreenCenter );
      }

      /// Return the resoultion in pixels
      glm::ivec2 screen_resolution() {
        CXSHARED
        return glm::ivec2(obj_->hmd_info.HResolution, obj_->hmd_info.VResolution);
      }

      /// Return the screen size in metres (I think)
      glm::vec2 screen_size() {
        CXSHARED
        return glm::vec2(obj_->hmd_info.HScreenSize, obj_->hmd_info.VScreenSize);
      }

     
      /// Return the final FBO rendering size. This is scaled up
      glm::vec2 fbo_size(){ CXSHARED return obj_->fbo_size; }
      
      /// Return the screen size, scaled by rendering scale
      glm::vec2 screen_size_scaled(){ CXSHARED return obj_->screen_size_scaled; }

 
      glm::mat4 left_inter() {  
        CXSHARED
        OVR::Util::Render::StereoEyeParams leftEye = obj_->stereo_config.GetEyeRenderParams (OVR::Util::Render::StereoEye_Left); 
        return ToGLMmatrix(leftEye.ViewAdjust);  
      }

      glm::mat4 right_inter() {   
        CXSHARED
        OVR::Util::Render::StereoEyeParams rightEye = obj_->stereo_config.GetEyeRenderParams (OVR::Util::Render::StereoEye_Right); 
        return ToGLMmatrix(rightEye.ViewAdjust);  
      }


      glm::mat4 left_projection() {  
        CXSHARED
        OVR::Util::Render::StereoEyeParams leftEye = obj_->stereo_config.GetEyeRenderParams (OVR::Util::Render::StereoEye_Left); 
        return ToGLMmatrix(leftEye.Projection);  
      }

      glm::mat4 right_projection() {   
        CXSHARED
        OVR::Util::Render::StereoEyeParams rightEye = obj_->stereo_config.GetEyeRenderParams (OVR::Util::Render::StereoEye_Right); 
        return ToGLMmatrix(rightEye.Projection);  
      }
    };
  }

}


#endif