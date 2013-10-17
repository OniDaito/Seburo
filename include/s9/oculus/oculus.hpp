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
     
    class SEBUROAPI OculusBase : public OVR::MessageHandler {
    
    protected:

      void init();

      struct SharedObj {
        OVR::Ptr<OVR::DeviceManager> manager;
        OVR::Ptr<OVR::HMDDevice> HMD;
        OVR::Ptr<OVR::SensorDevice> sensor; 
        OVR::Ptr<OVR::Profile> profile;

        OVR::Util::Render::StereoConfig config;
        OVR::Util::LatencyTest latency_util;
        OVR::Ptr<OVR::LatencyTestDevice> latency_tester;

        OVR::SensorFusion fusion;
      };

      std::shared_ptr<SharedObj> obj_ = nullptr;
  
    public:
  
      OculusBase();

      void update(double_t dt);

      glm::quat getOrientation() { 
        if (obj_->sensor){
          OVR::Quatf q = obj_->fusion.GetOrientation(); 
          return glm::quat(q.w,q.x,q.y,q.z); 
        }
        return glm::quat();
      }

    };
  }

}


#endif