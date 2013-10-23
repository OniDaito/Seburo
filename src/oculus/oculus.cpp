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

///\todo - pass in a window listener as window events will affect this

OculusBase::OculusBase(bool b) : obj_ ( std::shared_ptr<SharedObj>(new SharedObj(this))) {
  
  System::Init( Log::ConfigureDefaultLog (LogMask_All));
  obj_->manager=*DeviceManager::Create();
  obj_->manager->SetMessageHandler(this);

  
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
    /*if (obj_->info.HScreenSize > 0.0f)
    {
        if (obj_->info.HScreenSize > 0.140f)  // 7"
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
   // Check if any new devices were connected.

  bool queueIsEmpty = false;
  while (!queueIsEmpty) {
    DeviceStatusNotificationDesc desc;
    {
      Lock::Locker lock(obj_->manager->GetHandlerLock());
      if (obj_->device_status_notifications_queue.GetSize() == 0)
        break;

      desc = obj_->device_status_notifications_queue.Front();
       
      // We can't call Clear under the lock since this may introduce a dead lock:
      // this thread is locked by HandlerLock and the Clear might cause 
      // call of Device->Release, which will use Manager->DeviceLock. The bkg
      // thread is most likely locked by opposite way: 
      // Manager->DeviceLock ==> HandlerLock, therefore - a dead lock.
      // So, just grab the first element, save a copy of it and remove
      // the element (Device->Release won't be called since we made a copy).
      
      obj_->device_status_notifications_queue.RemoveAt(0);
      queueIsEmpty = (obj_->device_status_notifications_queue.GetSize() == 0);
    }
    cout << "OC" << endl;

    bool wasAlreadyCreated = desc.Handle.IsCreated();

    if (desc.Action == Message_DeviceAdded) {
      switch(desc.Handle.GetType()){
        case Device_Sensor:
          if (desc.Handle.IsAvailable() && !desc.Handle.IsCreated()) {
                  
            if (!obj_->sensor){
              obj_->sensor = *desc.Handle.CreateDeviceTyped<SensorDevice>();
              obj_->fusion.AttachToSensor(obj_->sensor);
              cout << "SEBURO OCULUS - Sensor Connected" << endl;
            } else if (!wasAlreadyCreated) {
               cout << "SEBURO OCULUS - A new SENSOR has been detected, but it is not currently used." << endl;
            }
          }
          break;
          
        case Device_LatencyTester:
          if (desc.Handle.IsAvailable() && !desc.Handle.IsCreated()) {
            if (!obj_->latency_tester) {
              obj_->latency_tester = *desc.Handle.CreateDeviceTyped<LatencyTestDevice>();
              obj_->latency_util.SetDevice(obj_->latency_tester);
              if (!wasAlreadyCreated)
                cout << "SEBURO OCULUS - Latency Tester Connected" << endl;
              }
          }
              
          break;
          
        case Device_HMD: 
          {
            OVR::HMDInfo info;
            desc.Handle.GetDeviceInfo(&info);
            // if strlen(info.DisplayDeviceName) == 0 then
            // this HMD is 'fake' (created using sensor).
          
            if (strlen(info.DisplayDeviceName) > 0 && (!obj_->HMD || !info.IsSameDisplay(obj_->info))) {
              cout << "SEBURO OCULUS - HMD Connected" << endl;
              
              if (!obj_->HMD || !desc.Handle.IsDevice(obj_->HMD))
                obj_->HMD = *desc.Handle.CreateDeviceTyped<HMDDevice>();
                      
              // update stereo config with new HMDInfo
              if (obj_->HMD && obj_->HMD->GetDeviceInfo(&obj_->info)) {
                //RenderParams.MonitorName = hmd.DisplayDeviceName;
                obj_->config.SetHMDInfo(obj_->info);
              }
              cout << "SEBURO OCULUS - HMD device added." << endl;
            }
            break;
          }

        default:;
      }
    } else if (desc.Action == Message_DeviceRemoved) {
      
      if (desc.Handle.IsDevice(obj_->sensor)) {
        cout << "SEBURO OCULUS - Sensor reported device removed." << endl;
        obj_->fusion.AttachToSensor(NULL);
        obj_->sensor.Clear();
        cout << "SEBURO OCULUS - Sensor disconnected" << endl;
      } else if (desc.Handle.IsDevice(obj_->latency_tester)) {
        cout << "SEBURO OCULUS - Latency Tester reported device removed." << endl;
        obj_->latency_util.SetDevice(NULL);
        obj_->latency_tester.Clear();
        cout << "SEBURO OCULUS - Latency Sensor disconnected" << endl;
      } else if (desc.Handle.IsDevice(obj_->HMD)) {
        if (obj_->HMD && !obj_->HMD->IsDisconnected()) {
          cout << "SEBURO OCULUS - HMD disconnected" << endl;
               
          obj_->HMD = obj_->HMD->Disconnect(obj_->sensor);

          // This will initialize obj_->info with information about configured IPD,
          // screen size and other variables needed for correct projection.
          // We pass HMD DisplayDeviceName into the renderer to select the
          // correct monitor in full-screen mode.
          if (obj_->HMD && obj_->HMD->GetDeviceInfo(&obj_->info)) {
            //RenderParams.MonitorName = hmd.DisplayDeviceName;
            obj_->config.SetHMDInfo(obj_->info);
          }
          cout << "SEBURO OCULUS - HMD device removed." << endl;
        }
      }
    } else 
      assert(0);
  }


  // Process latency tester results.
  const char* results = obj_->latency_util.GetResultsString();
  if (results != NULL) {
        cout << "SEBURO OCULUS - Latency Tester: " <<  results << endl; 
  }

  // >>> THIS MUST BE PLACED AS CLOSE AS POSSIBLE TO WHERE THE HMD ORIENTATION IS READ <<<
  obj_->latency_util.ProcessInputs();

  // Handle Sensor motion.
  // We extract Yaw, Pitch, Roll instead of directly using the orientation
  // to allow "additional" yaw manipulation with mouse/controller.
  if(obj_->sensor) {
      Quatf    q = obj_->fusion.GetPredictedOrientation();
      //OVR::Quatf q = obj_->fusion.GetOrientation(); 
      orientation_ = glm::quat(q.w,q.x,q.y,q.z); 
      

      //float    yaw = 0.0f;
      //hmdOrient.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &ThePlayer.EyePitch, &ThePlayer.EyeRoll);
  }


}

void OculusBase::onMessage(const Message& msg) {
    if (msg.Type == Message_DeviceAdded || msg.Type == Message_DeviceRemoved)
    {
        if (msg.pDevice == obj_->manager)
        {
            const MessageDeviceStatus& statusMsg =
                static_cast<const MessageDeviceStatus&>(msg);

            { // limit the scope of the lock
                Lock::Locker lock(obj_->manager->GetHandlerLock());
                obj_->device_status_notifications_queue.PushBack(
                    DeviceStatusNotificationDesc(statusMsg.Type, statusMsg.Handle));
            }

            switch (statusMsg.Type)
            {
                case OVR::Message_DeviceAdded:
                    cout << "DeviceManager reported device added." << endl;
                    break;
                
                case OVR::Message_DeviceRemoved:
                    cout << "DeviceManager reported device removed." << endl;
                    break;
                
                default: OVR_ASSERT(0); // unexpected type
            }
        }
    }
}