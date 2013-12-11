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

#include <OVR.h>
#include <OVRVersion.h>

namespace s9 {


  namespace oculus {

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

        SharedObject();

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

        OVR::Util::Render::StereoConfig stereo_config;
        OVR::Util::LatencyTest latency_util;
        OVR::Ptr<OVR::LatencyTestDevice> latency_tester;

        OVR::Array<DeviceStatusNotificationDesc> device_status_notifications_queue; 

        OVR::SensorFusion fusion;
        OVR::HMDInfo hmd_info;
        glm::quat orientation;

        OVR::Util::Render::StereoEyeParams left_eye_params;
        OVR::Util::Render::StereoEyeParams right_eye_params;

        float render_scale;
        std::string monitor_name;

        // Distortion parameters
        glm::ivec4 left_eye_viewport;
        glm::ivec4 right_eye_viewport;

        bool initialized;
  

      };

      std::shared_ptr<SharedObject> obj_ = nullptr;

    public:
  
      OculusBase() {}
      OculusBase(bool b);
      ~OculusBase() { }

      void update(double_t dt) { if(obj_ != nullptr) obj_->update(dt); }

      bool connected() { if (obj_ != nullptr) return (obj_->connected() && obj_->initialized); return false; }

      glm::quat orientation() { if(obj_ != nullptr) return obj_->orientation; else return glm::quat(); } 
     
      glm::ivec4 left_eye_viewport() { if(obj_ != nullptr) return obj_->left_eye_viewport; else return glm::ivec4(1.0f); }
      glm::ivec4 right_eye_viewport() { if(obj_ != nullptr) return obj_->right_eye_viewport; else return glm::ivec4(1.0f); }

      std::string monitor_name()  { if(obj_ != nullptr) return obj_->monitor_name; else return ""; }

      float render_scale() {if(obj_ != nullptr) return obj_->render_scale; else return 1.0f; }

      glm::vec4 distortion_parameters() { if(obj_ != nullptr) 
          return glm::vec4(obj_->hmd_info.DistortionK[0], obj_->hmd_info.DistortionK[1],
            obj_->hmd_info.DistortionK[2],obj_->hmd_info.DistortionK[3]);
        else
          return glm::vec4(1.0f);
      }

      /// Return the interpupillary distance if the device exists, else 1.0f
      float interpupillary_distance() {
         if(obj_ != nullptr) return obj_->hmd_info.InterpupillaryDistance; else return 1.0f;
      }

      /// Return the lens separation distance if the device exists, else 1.0f
      float lens_separation_distance() {
        if(obj_ != nullptr) return obj_->hmd_info.LensSeparationDistance; else return 1.0f;
      }

      /// Return the eye to screen distance if the device exists, else 1.0f
      float eye_to_screen_distance() {
        if(obj_ != nullptr) return obj_->hmd_info.EyeToScreenDistance; else return 1.0f;
      }

      /// Return the screen center if the device exists, else 1.0f
      glm::vec2 screen_center() {
        if(obj_ != nullptr) return glm::vec2(obj_->hmd_info.HScreenSize / 2.0, obj_->hmd_info.VScreenCenter ); else return glm::vec2(1.0f);
      }

      /// Return the resoultion in pixels or zero
      glm::ivec2 screen_resolution() {
        if(obj_ != nullptr) return glm::ivec2(obj_->hmd_info.HResolution, obj_->hmd_info.VResolution); else return glm::ivec2(0.0f);

      }

      /// Return the screen size in metres (I think) if the device exists, else 1.0f
      glm::vec2 screen_size() {
        if(obj_ != nullptr) return glm::vec2(obj_->hmd_info.HScreenSize, obj_->hmd_info.VScreenSize); else return glm::vec2(1.0f);
      }
    };
  }

}


#endif