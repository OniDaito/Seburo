/**
* @brief Oculus Helper Classes
* @file oculus.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 20/09/2013
*
*/



#ifndef OCULUS_OCULUS_HPP
#define OCULUS_OCULUS_HPP

#include "../common.hpp"

#include <OVR.h>
#include <OVRVersion.h>

namespace s9 {


  namespace oculus {

    /**
     * Basic Oculus Entry class that deals with config
     */
     
    class SEBUROAPI OculusBase {
    
    protected:
      struct SharedObj {
 
      };
      boost::shared_ptr<SharedObj> _obj;

  
    public:
      OculusBase();
    };
  }

}


#endif