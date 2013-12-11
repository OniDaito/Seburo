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

      struct SharedObject : public OVR::MessageHandler {

        SharedObject();

        void onMessage(const OVR::Message& msg);
        void update(double_t dt);

        struct DeviceStatusNotificationDesc {
          OVR::DeviceHandle    Handle;
          OVR::MessageType     Action;

          DeviceStatusNotificationDesc():Action(OVR::Message_None) {}
          DeviceStatusNotificationDesc(OVR::MessageType mt, const OVR::DeviceHandle& dev) 
            : Handle(dev), Action(mt) {}
        };
    
           
        ~SharedObject() {
         
          latency_tester.Clear();
          sensor.Clear();
          HMD.Clear();

          std::cout << "SEBURO OCCULUS - Shutting down oculus manager." << std::endl;
        }

        std::vector<OculusBase*> handlers;
       
        OVR::Ptr<OVR::DeviceManager> manager;
        OVR::Ptr<OVR::HMDDevice> HMD;
        OVR::Ptr<OVR::SensorDevice> sensor; 
        OVR::Ptr<OVR::Profile> profile;

        OVR::Util::Render::StereoConfig config;
        OVR::Util::LatencyTest latency_util;
        OVR::Ptr<OVR::LatencyTestDevice> latency_tester;

        OVR::Array<DeviceStatusNotificationDesc> device_status_notifications_queue; 

        OVR::SensorFusion fusion;
        OVR::HMDInfo info;

        glm::quat orientation;
      };

      std::shared_ptr<SharedObject> obj_ = nullptr;

  
    public:
  
      OculusBase() {}
      OculusBase(bool b);
      ~OculusBase() { }

      void update(double_t dt) { if(obj_ != nullptr) obj_->update(dt); }

      glm::quat orientation() { if(obj_ != nullptr) return obj_->orientation; else return glm::quat(); } 
     
    };
  }

}


#endif