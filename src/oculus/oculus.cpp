/**
* @brief Oculus Basic helper classes
* @file openni.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 20/09/2013
*
*/

#include "s9/oculus/oculus.hpp"

using namespace std;
using namespace s9;
using namespace s9::oculus;
using namespace OVR;

///\todo - we should allow polling and removal - adding of the device so  

OculusBase::OculusBase() : obj_ ( std::shared_ptr<SharedObj>(new SharedObj())) {

}

void OculusBase::init() {
  
  System::Init( Log::ConfigureDefaultLog (LogMask_All));

  obj_->manager=*DeviceManager::Create();
  obj_->HMD=*(obj_->manager->EnumerateDevices<HMDDevice>().CreateDevice());
  
  if (obj_->HMD){

    obj_->sensor = *(obj_->HMD->GetSensor());

    HMDInfo hmd;
    
    if(obj_->HMD->GetDeviceInfo(&hmd)) {  
      obj_->config.SetHMDInfo(hmd);
      // Print out config
      cout << "SEBURO OCULUS - Resolution: " << hmd.HResolution << " x " << hmd.VResolution << endl;
      cout << "SEBURO OCULUS - Screen Size: " << hmd.HScreenSize << " x " << hmd.VScreenSize << endl;
      cout << "SEBURO OCULUS - Display Devicename: " << hmd.DisplayDeviceName << endl;
      cout << "SEBURO OCULUS - DisplayID: " << hmd.DisplayId << endl;

      cout << "SEBURO OCULUS - DistortionK 0: " << hmd.DistortionK[0] << endl;
      cout << "SEBURO OCULUS - DistortionK 1: " << hmd.DistortionK[1] << endl;
      cout << "SEBURO OCULUS - DistortionK 2: " << hmd.DistortionK[2] << endl;
      cout << "SEBURO OCULUS - DistortionK 3: " << hmd.DistortionK[3] << endl;
    }

    obj_->profile = obj_->HMD->GetProfile();
  
    /*if (obj_->profile){
      
    }*/

  } else{
     obj_->sensor = *(obj_->manager->EnumerateDevices<SensorDevice>().CreateDevice());
  }


  obj_->latency_tester = *(obj_->manager->EnumerateDevices<LatencyTestDevice>().CreateDevice());
  if (obj_->latency_tester) {
    obj_->latency_util.SetDevice(obj_->latency_tester);
  }

  // Make the user aware which devices are present.
  if(obj_->HMD == NULL && obj_->sensor == NULL){
    cerr << "SEBURO OCULUS ERROR - No HMD or Sensor detected" << endl;
  } else if(obj_->HMD  == NULL) {
    cerr << "SEBURO OCULUS ERROR - No HMD detected" << endl;
  } else if(obj_->sensor == NULL) {
    cerr << "SEBURO OCULUS ERROR - No Sensor detected" << endl;
  }
 

  if(obj_->sensor) {
    // We need to attach sensor to SensorFusion object for it to receive
    // body frame messages and update orientation. SFusion.GetOrientation()
    // is used in OnIdle() to orient the view.
    obj_->fusion.AttachToSensor(obj_->sensor);
    obj_->fusion.SetDelegateMessageHandler(this);
    obj_->fusion.SetPredictionEnabled(true);

  }

  
    // Configure proper Distortion Fit.
    // For 7" screen, fit to touch left side of the view, leaving a bit of
    // invisible screen on the top (saves on rendering cost).
    // For smaller screens (5.5"), fit to the top.
    /*if (TheHMDInfo.HScreenSize > 0.0f)
    {
        if (TheHMDInfo.HScreenSize > 0.140f)  // 7"
            SConfig.SetDistortionFitPointVP(-1.0f, 0.0f);        
        else        
            SConfig.SetDistortionFitPointVP(0.0f, 1.0f);        
    }

    pRender->SetSceneRenderScale(SConfig.GetDistortionScale());
    //pRender->SetSceneRenderScale(1.0f);

    SConfig.Set2DAreaFov(DegreeToRad(85.0f));

    */
}

void OculusBase::update(double_t dt) {

}
