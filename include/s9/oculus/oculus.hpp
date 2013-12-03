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

    class SEBUROAPI OculusBase : public OVR::MessageHandler {
    
    protected:

      void onMessage(const OVR::Message& msg);

      struct DeviceStatusNotificationDesc {
        OVR::DeviceHandle    Handle;
        OVR::MessageType     Action;

        DeviceStatusNotificationDesc():Action(OVR::Message_None) {}
        DeviceStatusNotificationDesc(OVR::MessageType mt, const OVR::DeviceHandle& dev) 
            : Handle(dev), Action(mt) {}
      };
    

      struct SharedObj {

        SharedObj(OculusBase* pb) {
          addHandler(pb);
        }

        void removeHandler(OculusBase* pb) {
          for (std::vector<OculusBase*>::iterator it = handlers.begin(); it != handlers.end(); ){
            if  (*it == pb){
              handlers.erase(it);
            } else {
              ++it;
            } 
          }
        }

        void addHandler(OculusBase* pb) {
          handlers.push_back(pb);
        }
        
        ~SharedObj() {
          for(OculusBase* pb : handlers){
            pb->RemoveHandlerFromDevices();
          }

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
      };

      std::shared_ptr<SharedObj> obj_ = nullptr;

      glm::quat orientation_;
  
    public:
  
      OculusBase() {}
      OculusBase(bool b);
      ~OculusBase() { if (obj_) obj_->removeHandler(this);}

      /*const OculusBase& operator= (const OculusBase & b) {

      }*/

      void update(double_t dt);

      glm::quat orientation() { return orientation_; } 
     

    };
  }

}


#endif