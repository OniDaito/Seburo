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

OculusBase::SharedObject::SharedObject(float_t near, float_t far) {
  initialized = false;
  manager = *DeviceManager::Create();
  
  HMD=*(manager->EnumerateDevices<HMDDevice>().CreateDevice());
  
  if (HMD){

    sensor = *(HMD->GetSensor());
    
    if(HMD->GetDeviceInfo(&hmd_info)) {  
      stereo_config.SetHMDInfo(hmd_info);
      // Print out config
      cout << "SEBURO OCULUS - Resolution: " << hmd_info.HResolution << " x " << hmd_info.VResolution << endl;
      cout << "SEBURO OCULUS - Screen Size: " << hmd_info.HScreenSize << " x " << hmd_info.VScreenSize << endl;
      cout << "SEBURO OCULUS - Display Devicename: " << hmd_info.DisplayDeviceName << endl;
      cout << "SEBURO OCULUS - DisplayID: " << hmd_info.DisplayId << endl;

      cout << "SEBURO OCULUS - DistortionK 0: " << hmd_info.DistortionK[0] << endl;
      cout << "SEBURO OCULUS - DistortionK 1: " << hmd_info.DistortionK[1] << endl;
      cout << "SEBURO OCULUS - DistortionK 2: " << hmd_info.DistortionK[2] << endl;
      cout << "SEBURO OCULUS - DistortionK 3: " << hmd_info.DistortionK[3] << endl;
    }

    profile = HMD->GetProfile();

    near_plane = near;
    far_plane = far;

    setupCameras();

  } else{
     sensor = *(manager->EnumerateDevices<SensorDevice>().CreateDevice());
  }


  latency_tester = *(manager->EnumerateDevices<LatencyTestDevice>().CreateDevice());
  if (latency_tester) {
    latency_util.SetDevice(latency_tester);
  }

  // Make the user aware which devices are present. Not an error per-se as we wait for plugging in
  if(HMD == NULL && sensor == NULL){
    cout << "SEBURO OCULUS Startup - No HMD or Sensor detected" << endl;
  } else if(HMD  == NULL) {
    cout << "SEBURO OCULUS Startup - No HMD detected" << endl;
  } else if(sensor == NULL) {
    cout << "SEBURO OCULUS Startup - No Sensor detected" << endl;
  } else {
    initialized = true;
  }
 

  if(sensor) {
    // We need to attach sensor to SensorFusion object for it to receive
    // body frame messages and update orientation. SFusion.GetOrientation()
    // is used in OnIdle() to orient the view.
    fusion.AttachToSensor(sensor);
    fusion.SetDelegateMessageHandler(this);
    fusion.SetPredictionEnabled(true);
    fusion.SetYawCorrectionEnabled(true);

  }

  
    // Configure proper Distortion Fit.
    // For 7" screen, fit to touch left side of the view, leaving a bit of
    // invisible screen on the top (saves on rendering cost).
    // For smaller screens (5.5"), fit to the top.
    if (hmd_info.HScreenSize > 0.0f) {
        if (hmd_info.HScreenSize > 0.140f)  // 7"
            stereo_config.SetDistortionFitPointVP(-1.0f, 0.0f);        
        else        
            stereo_config.SetDistortionFitPointVP(0.0f, 1.0f);        
    }


  // This goes last as we want to attempt full setup before we allow the update thread access
  manager->SetMessageHandler(this);
}

///\todo - pass in a window listener as window events will affect this

OculusBase::OculusBase(float_t near, float_t far) {
  
  System::Init( Log::ConfigureDefaultLog (LogMask_All));  
  obj_ = std::shared_ptr<SharedObject>(new SharedObject(near,far));


}

void OculusBase::SharedObject::update(double_t dt) {
   // Check if any new devices were connected.

  bool queueIsEmpty = false;
  while (!queueIsEmpty) {
    DeviceStatusNotificationDesc desc;
    {
      Lock::Locker lock(manager->GetHandlerLock());
      if (device_status_notifications_queue.GetSize() == 0)
        break;

      desc = device_status_notifications_queue.Front();
       
      // We can't call Clear under the lock since this may introduce a dead lock:
      // this thread is locked by HandlerLock and the Clear might cause 
      // call of Device->Release, which will use Manager->DeviceLock. The bkg
      // thread is most likely locked by opposite way: 
      // Manager->DeviceLock ==> HandlerLock, therefore - a dead lock.
      // So, just grab the first element, save a copy of it and remove
      // the element (Device->Release won't be called since we made a copy).
      
      device_status_notifications_queue.RemoveAt(0);
      queueIsEmpty = (device_status_notifications_queue.GetSize() == 0);
    }

    bool wasAlreadyCreated = desc.Handle.IsCreated();

    if (desc.Action == Message_DeviceAdded) {
      switch(desc.Handle.GetType()){
        case Device_Sensor:
          if (desc.Handle.IsAvailable() && !desc.Handle.IsCreated()) {
                  
            if (!sensor){
              sensor = *desc.Handle.CreateDeviceTyped<SensorDevice>();
              fusion.AttachToSensor(sensor);
              cout << "SEBURO OCULUS - Sensor Connected" << endl;
            } else if (!wasAlreadyCreated) {
               cout << "SEBURO OCULUS - A new SENSOR has been detected, but it is not currently used." << endl;
            }
          }


          break;
          
        case Device_LatencyTester:
          if (desc.Handle.IsAvailable() && !desc.Handle.IsCreated()) {
            if (!latency_tester) {
              latency_tester = *desc.Handle.CreateDeviceTyped<LatencyTestDevice>();
              latency_util.SetDevice(latency_tester);
              if (!wasAlreadyCreated)
                cout << "SEBURO OCULUS - Latency Tester Connected" << endl;
              }
          }
              
          break;
          
        case Device_HMD: 
          {
            desc.Handle.GetDeviceInfo(&hmd_info);
            // if strlen(hmd_info.DisplayDeviceName) == 0 then
            // this HMD is 'fake' (created using sensor).
          
            if (strlen(hmd_info.DisplayDeviceName) > 0 && (!HMD || !hmd_info.IsSameDisplay(hmd_info))) {
              cout << "SEBURO OCULUS - HMD Connected" << endl;
              
              if (!HMD || !desc.Handle.IsDevice(HMD))
                HMD = *desc.Handle.CreateDeviceTyped<HMDDevice>();
                      
              // update stereo config with new HMDInfo
              if (HMD && HMD->GetDeviceInfo(&hmd_info)) {
                monitor_name = hmd_info.DisplayDeviceName;
                stereo_config.SetHMDInfo(hmd_info);
                stereo_config.SetFullViewport(OVR::Util::Render::Viewport(0,0, hmd_info.HScreenSize, hmd_info.VScreenSize));
                stereo_config.SetStereoMode(OVR::Util::Render::Stereo_LeftRight_Multipass);
                if (hmd_info.HScreenSize > 0.0f) {
                  if (hmd_info.HScreenSize > 0.140f)  // 7"
                    stereo_config.SetDistortionFitPointVP(-1.0f, 0.0f);        
                  else        
                    stereo_config.SetDistortionFitPointVP(0.0f, 1.0f);        
                }
         
                cout << "SEBURO OCULUS - HMD device added." << endl;

                cout << "SEBURO OCULUS - Resolution: " << hmd_info.HResolution << " x " << hmd_info.VResolution << endl;
                cout << "SEBURO OCULUS - Screen Size: " << hmd_info.HScreenSize << " x " << hmd_info.VScreenSize << endl;
                cout << "SEBURO OCULUS - Display Devicename: " << hmd_info.DisplayDeviceName << endl;
                cout << "SEBURO OCULUS - DisplayID: " << hmd_info.DisplayId << endl;

                cout << "SEBURO OCULUS - DistortionK 0: " << hmd_info.DistortionK[0] << endl;
                cout << "SEBURO OCULUS - DistortionK 1: " << hmd_info.DistortionK[1] << endl;
                cout << "SEBURO OCULUS - DistortionK 2: " << hmd_info.DistortionK[2] << endl;
                cout << "SEBURO OCULUS - DistortionK 3: " << hmd_info.DistortionK[3] << endl;

                setupCameras();

                initialized = true;
              }
            }
            break;
          }

        default:;
      }
    } else if (desc.Action == Message_DeviceRemoved) {
      
      initialized = false;
      
      if (desc.Handle.IsDevice(sensor)) {
        cout << "SEBURO OCULUS - Sensor reported device removed." << endl;
        fusion.AttachToSensor(NULL);
        sensor.Clear();
        cout << "SEBURO OCULUS - Sensor disconnected" << endl;
      } else if (desc.Handle.IsDevice(latency_tester)) {
        cout << "SEBURO OCULUS - Latency Tester reported device removed." << endl;
        latency_util.SetDevice(NULL);
        latency_tester.Clear();
        cout << "SEBURO OCULUS - Latency Sensor disconnected" << endl;
      } else if (desc.Handle.IsDevice(HMD)) {
        if (HMD && !HMD->IsDisconnected()) {
          cout << "SEBURO OCULUS - HMD disconnected" << endl;
               
          HMD = HMD->Disconnect(sensor);

          // This will initialize hmd_info with hmd_information about configured IPD,
          // screen size and other variables needed for correct projection.
          // We pass HMD DisplayDeviceName into the renderer to select the
          // correct monitor in full-screen mode.
          if (HMD && HMD->GetDeviceInfo(&hmd_info)) {
            //RenderParams.MonitorName = hmd.DisplayDeviceName;
            stereo_config.SetHMDInfo(hmd_info);
          }
          cout << "SEBURO OCULUS - HMD device removed." << endl;
        }
      }
    } else 
      assert(0);
  }


  // Process latency tester results.
  const char* results = latency_util.GetResultsString();
  if (results != NULL) {
        cout << "SEBURO OCULUS - Latency Tester: " <<  results << endl; 
  }

  // >>> THIS MUST BE PLACED AS CLOSE AS POSSIBLE TO WHERE THE HMD ORIENTATION IS READ <<<
  latency_util.ProcessInputs();

  // Handle Sensor motion.
  // We extract Yaw, Pitch, Roll instead of directly using the orientation
  // to allow "additional" yaw manipulation with mouse/controller.
  if(sensor) {
      Quatf    q = fusion.GetPredictedOrientation();
      //OVR::Quatf q = fusion.GetOrientation(); 
      orientation = glm::quat(q.w,q.x,q.y,q.z); 
      
      //float    yaw = 0.0f;
      //hmdOrient.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &ThePlayer.EyePitch, &ThePlayer.EyeRoll);
  }

 
}


void OculusBase::SharedObject::setupCameras(){
    fbo_size = glm::vec2( hmd_info.HResolution * stereo_config.GetDistortionScale(), hmd_info.VResolution * stereo_config.GetDistortionScale());
    
    screen_size_scaled = glm::vec2(hmd_info.HScreenSize,hmd_info.VScreenSize) * stereo_config.GetDistortionScale();

    float aspect = static_cast<float>(fbo_size.x) * 0.5f / static_cast<float>(fbo_size.y);
    float half_screen = screen_size_scaled.y * 0.5f;
    float fov = 2.0f * atan( half_screen / hmd_info.EyeToScreenDistance);

    // Is this the absoulte seperation? Should we be dividing by two?
    // Make sure - is on the right side

    float view_center = screen_size_scaled.x * 0.25f;
    float eye_shift = view_center - (hmd_info.LensSeparationDistance * 0.5f);
    float projection_offset = (4.0f * eye_shift) / screen_size_scaled.x;

    // Concerned with the FOV. I'm halving it twice. Once for sure but twice?
    // This is a bit odd because we loose the stereo but its correct for sintel :S

    glm::mat4 perspective = glm::perspective( radToDeg(fov) * 0.5f , aspect, near_plane, far_plane);

    left_projection = glm::translate(perspective, glm::vec3(projection_offset,0.0f,0.0f));
    right_projection = glm::translate(perspective, glm::vec3(-projection_offset,0.0f,0.0f));

    float dip = hmd_info.InterpupillaryDistance * 0.5f;

    left_inter = glm::translate(glm::mat4(1.0f), glm::vec3(dip,0.0f,0.0f));
    right_inter = glm::translate(glm::mat4(1.0f), glm::vec3(-dip,0.0f,0.0f));
}


void OculusBase::SharedObject::OnMessage(const Message& msg) {

  if (msg.Type == Message_DeviceAdded || msg.Type == Message_DeviceRemoved) {
    if (msg.pDevice == manager) {
      const MessageDeviceStatus& statusMsg = static_cast<const MessageDeviceStatus&>(msg);

      { // limit the scope of the lock
        Lock::Locker lock(manager->GetHandlerLock());
        device_status_notifications_queue.PushBack(
          DeviceStatusNotificationDesc(statusMsg.Type, statusMsg.Handle));
      }

      switch (statusMsg.Type) {
        case OVR::Message_DeviceAdded:
        cout << "SEBURO OCULUS DeviceManager reported device added." << endl;
        break;

        case OVR::Message_DeviceRemoved:
        cout << "SEBURO OCULUS DeviceManager reported device removed." << endl;
        break;

        default: OVR_ASSERT(0); // unexpected type
      }
    }
  }
}