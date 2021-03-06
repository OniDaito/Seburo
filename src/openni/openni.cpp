/**
* @brief OpenNI Basic helper classes
* @file openni.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 18/09/2013
*
*/

#include "s9/openni/openni.hpp"

using namespace std;
using namespace s9;
using namespace s9::oni;

///\todo add OpenNI add/remove device listeners - update callbacks?


bool OpenNISkeleton::sVisibleUsers[S9_NITE_MAX_USERS] = {false};
nite::SkeletonState OpenNISkeleton::sSkeletonStates[S9_NITE_MAX_USERS] = {nite::SKELETON_NONE};


inline void USER_MESSAGE (const OpenNISkeleton::User& user, std::string msg) {
  cout << "SEBURO Nite - User: " << user.id() << ", " << msg << endl;
}

OpenNIBase::SharedObject::~SharedObject() {

  depth_stream.destroy();
  colour_stream.destroy();

  // this line seems to cause problems with the extended nite shared object :S
  // Also, this seems to be called twice

  openni::OpenNI::shutdown(); // Assuming there is only one base ;)
  
  if (ready){ // we allocated so we must un-allocate
    delete[] tex_buffer_depth;
    delete[] tex_buffer_colour;
  }
}

void OpenNIBase::Shutdown() {

  assert(obj_);

  obj_->depth_stream.destroy();
  obj_->colour_stream.destroy();

  // this line seems to cause problems with the extended nite shared object :S
  // Also, this seems to be called twice

  openni::OpenNI::shutdown(); // Assuming there is only one base ;)
}

// Taken from the OpenNI Libraries

void OpenNIBase::CalculateHistogram(float* pHistogram, int histogramSize, const openni::VideoFrameRef& frame) {
  const openni::DepthPixel* pDepth = (const openni::DepthPixel*)frame.getData();
  // Calculate the accumulative histogram (the yellow display...)
  memset(pHistogram, 0, histogramSize*sizeof(float));
  int restOfRow = frame.getStrideInBytes() / sizeof(openni::DepthPixel) - frame.getWidth();
  int height = frame.getHeight();
  int width = frame.getWidth();

  unsigned int nNumberOfPoints = 0;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x, ++pDepth) {
      if (*pDepth != 0) {
        pHistogram[*pDepth]++;
        nNumberOfPoints++;
      }
    }
    pDepth += restOfRow;
  }
  for (int nIndex=1; nIndex<histogramSize; nIndex++){
    pHistogram[nIndex] += pHistogram[nIndex-1];
  }
  if (nNumberOfPoints) {
    for (int nIndex=1; nIndex<histogramSize; nIndex++) {
      pHistogram[nIndex] = (256 * (1.0f - (pHistogram[nIndex] / nNumberOfPoints)));
    }
  }
}


///\todo dont always assume both streams

OpenNIBase::OpenNIBase( const char* deviceURI, bool use_colour, bool use_depth , bool use_ir ) : obj_( shared_ptr<SharedObject> (new SharedObject())) {
  openni::Status rc = openni::STATUS_OK;

  obj_->width_depth = obj_->height_depth = 0;
  obj_->width_colour = obj_->height_colour = 0;
  obj_->width_ir = obj_->height_ir = 0;

  obj_->ready = false;

  rc = openni::OpenNI::initialize();

  obj_->depth_ready = obj_->colour_ready = obj_->ir_ready = false;
  obj_->num_streams = 0;

  cout << "SEBURO OpenNI: After initialization: " << openni::OpenNI::getExtendedError() << endl;

  rc = obj_->device.open(deviceURI);
  if (rc != openni::STATUS_OK) {
    cerr << "SEBURO OpenNI: Device open failed: " <<  openni::OpenNI::getExtendedError() << endl;
    openni::OpenNI::shutdown();
    return;
  }

  openni::DeviceInfo info = obj_->device.getDeviceInfo();

  // Attempt to open the Depth Stream
  if (use_depth && obj_->device.hasSensor( openni::SENSOR_DEPTH )){

    rc = obj_->depth_stream.create(obj_->device, openni::SENSOR_DEPTH);
    if (rc == openni::STATUS_OK) {
      rc = obj_->depth_stream.start();
      if (rc != openni::STATUS_OK) {
        cerr << "SEBURO OpenNI: Couldn't start depth stream: " << openni::OpenNI::getExtendedError() << endl;
        obj_->depth_stream.destroy();
      } else {
        obj_->depth_ready = true;
        obj_->num_streams++;
      }
    }
    else {
      cerr << "SEBURO OpenNI: Couldn't find depth stream: " << openni::OpenNI::getExtendedError() << endl;
    }
  }

  // Attempt to open the Colour Stream
  if (use_colour && obj_->device.hasSensor( openni::SENSOR_COLOR )){
    rc = obj_->colour_stream.create(obj_->device, openni::SENSOR_COLOR);
    if (rc == openni::STATUS_OK) {
      rc = obj_->colour_stream.start();
      if (rc != openni::STATUS_OK) {
        cerr <<  "SEBURO OpenNI: Couldn't start color stream: " << openni::OpenNI::getExtendedError() << endl;
        obj_->colour_stream.destroy();
      } else{
        obj_->colour_ready = true;
        obj_->num_streams++;
      }
    }
    else {
      cerr << "SEBURO OpenNI: Couldn't find colour stream: " << openni::OpenNI::getExtendedError() << endl;
    }
  }


  // Finally, attempt to open the IR Stream
  if (use_ir && obj_->device.hasSensor( openni::SENSOR_IR )){
    rc = obj_->ir_stream.create(obj_->device, openni::SENSOR_IR);
    if (rc == openni::STATUS_OK) {
      rc = obj_->ir_stream.start();
      if (rc != openni::STATUS_OK) {
        cerr <<  "SEBURO OpenNI: Couldn't start IR stream: " << openni::OpenNI::getExtendedError() << endl;
        obj_->ir_stream.destroy();
      } else {
        obj_->ir_ready = true;
        obj_->num_streams++;
      }
    }
    else {
      cerr << "SEBURO OpenNI: Couldn't find IR stream: " << openni::OpenNI::getExtendedError() << endl;
    }
  }

  if ( obj_->num_streams == 0 ) {
    cerr << "SEBURO OpenNI: No valid streams. Exiting." << endl;
    openni::OpenNI::shutdown();
    return;
  }
 
  // Create a index for our streams

  obj_->streams = new openni::VideoStream*[obj_->num_streams];
  obj_->frames = new openni::VideoFrameRef*[obj_->num_streams];
  
  int stream_idx = 0;

  // Now set the modes on the streams we have found, grabbing sizes and setting texture streams

  if (obj_->depth_ready) {
    openni::VideoMode depthVideoMode = obj_->depth_stream.getVideoMode();
    obj_->width_depth = depthVideoMode.getResolutionX();
    obj_->height_depth = depthVideoMode.getResolutionY();
    obj_->tex_buffer_depth = new byte_t[ obj_->width_depth * obj_->height_depth];
    obj_->texture_depth = gl::TextureStream(obj_->width_depth, obj_->height_depth, GREY);

    obj_->streams[stream_idx] = &(obj_->depth_stream);
    obj_->frames[stream_idx] = &(obj_->depth_frame);
    stream_idx++;

    cout << "SEBURO OpenNI - Allocated Depth stream of " << obj_->width_depth << "x" << obj_->height_depth << "." << endl;

  }

  if (obj_->colour_ready) {
    openni::VideoMode colourVideoMode = obj_->colour_stream.getVideoMode();
    obj_->width_colour = colourVideoMode.getResolutionX();
    obj_->height_colour = colourVideoMode.getResolutionY();
    obj_->tex_buffer_colour = new byte_t[obj_->width_colour * obj_->height_colour * 3];
    obj_->texture_colour = gl::TextureStream(obj_->width_colour, obj_->height_colour);

    obj_->streams[stream_idx] = &(obj_->colour_stream);
    obj_->frames[stream_idx] = &(obj_->colour_frame);
    stream_idx++;

    cout << "SEBURO OpenNI - Allocated Colour stream of " << obj_->width_colour << "x" << obj_->height_colour << "." << endl;
  }

  if (obj_->ir_ready) {
    openni::VideoMode irVideoMode = obj_->ir_stream.getVideoMode();
    obj_->width_ir = irVideoMode.getResolutionX();
    obj_->height_ir = irVideoMode.getResolutionY();
    obj_->tex_buffer_ir = new byte_t[ obj_->width_ir * obj_->height_ir];
    obj_->texture_ir = gl::TextureStream(obj_->width_ir, obj_->height_ir, GREY);

    obj_->streams[stream_idx] = &(obj_->ir_stream);
    obj_->frames[stream_idx] = &(obj_->ir_frame);
    stream_idx++;

    cout << "SEBURO OpenNI - Allocated IR stream of " << obj_->width_ir << "x" << obj_->height_ir << "." << endl;
  }


  obj_->ready = true;


 
}


 
void OpenNIBase::Update() {
  int changedIndex;

  if(!obj_->ready)
    return;

  // Wait and read the appropriate frame
  openni::Status rc = openni::OpenNI::waitForAnyStream(obj_->streams, obj_->num_streams, &changedIndex);
  if (rc != openni::STATUS_OK) {
    cout << "SEBURO OpenNI Error: Wait failed" << endl;
    return;
  }

  obj_->streams[changedIndex]->readFrame ( obj_->frames[changedIndex]);

  // If we have a depth frame, do the histogram conversion

  int buffer_size = obj_->width_colour * obj_->height_colour * sizeof(byte_t) * 3;

 
  // check if we need to draw image frame to texture
  // We dont bother with cropping and all that crap OpenNI likes to put in.
  if (obj_->colour_ready){
    if (obj_->colour_frame.isValid()) {
      memset(obj_->tex_buffer_colour, 0, buffer_size);
      memcpy(obj_->tex_buffer_colour, obj_->colour_frame.getData(), buffer_size);
    }
  }


  if (obj_->depth_ready) {
    if (obj_->depth_frame.isValid()) {
      CalculateHistogram(obj_->depth_hist, S9_OPENNI_MAX_DEPTH, obj_->depth_frame);
    
      buffer_size = obj_->width_depth * obj_->height_depth * sizeof(byte_t);

      // check if we need to draw depth frame to texture
    
      memset(obj_->tex_buffer_depth, 0, buffer_size);

      byte_t *td = obj_->tex_buffer_depth;
      uint16_t *ts = (uint16_t*) obj_->depth_frame.getData(); // This is a 16bit integer for depth data

      for (int i=0; i < obj_->width_depth * obj_->height_depth; ++i){
        *td = obj_->depth_hist[*ts];
        ++td;
        ++ts;
      }
    }
  }



}

void OpenNIBase::UpdateTextures() {
  if (obj_->ready){
    if (obj_->colour_ready)
      obj_->texture_colour.Update(obj_->tex_buffer_colour);
    if (obj_->depth_ready)
      obj_->texture_depth.Update(obj_->tex_buffer_depth);
  }
}


/**
 * The OpenNI Skeleton - Given an OpenNIBase, start tracking skeletons
 */


OpenNISkeleton::OpenNISkeleton(const OpenNIBase &base) : obj_( shared_ptr<SharedObject> (new SharedObject(base))) {

  nite::Status rc;
  rc = nite::NiTE::initialize();

  cout << "SEBURO NITE Status: " << rc << endl;

  if (!base.ready()){
    cerr << "SEBURO NITE Error - Could not initialize tracker: " << rc << endl;
    obj_->ready = false;
    return;
  }

  Init();
  
}


void OpenNISkeleton::Init() {
  // Perform initialisation if device is ready for it
  nite::Status rc;
  rc = (nite::Status) niteInitializeUserTrackerByDevice( (void*)&(obj_->base.device()), &(obj_->user_tracker_handle));
  if (rc != nite::STATUS_OK) {
    cerr << "SEBURO NITE Error - Could not initialize tracker: " << rc << endl;
    obj_->ready = false;
    return;
  } 

  obj_->ready = true;
}

/**
 * Read a frame from NiTE and read the C data into our C++ Vector
 */

nite::Status OpenNISkeleton::ReadFrame() {

  nite::Status rc;

  if (obj_->pNiteFrame != nullptr){
    niteUserTrackerFrameRelease(obj_->user_tracker_handle, obj_->pNiteFrame);
    obj_->pNiteFrame = nullptr;
  
  }

  rc = (nite::Status)niteReadUserTrackerFrame(obj_->user_tracker_handle, &(obj_->pNiteFrame));
  
  if (rc !=  nite::STATUS_OK){
    cerr << "SEBURO NITE Error - Could not read frame: " << rc << endl;
  }

  obj_->floor = obj_->pNiteFrame->floor;
  obj_->user_map = obj_->pNiteFrame->userMap;
  obj_->floor_confidence = obj_->pNiteFrame->floorConfidence;
  obj_->timestamp = obj_->pNiteFrame->timestamp;

  set<NiteUserId> userAround;

  NiteUserData* pv = (NiteUserData*)obj_->pNiteFrame->pUser;

  for (int i = 0; i < obj_->pNiteFrame->userCount; ++i){

    User ud (pv[i]);

    bool found = false;
    for (User &dd : obj_->users){
      if (ud.id() == dd.id()){
        found = true;
        dd = ud;
        userAround.insert(ud.id());
        break;
      }
    }

    if (!found){
      obj_->users.push_back(ud);
      userAround.insert(ud.id());
    }
  }

  // Remove users not around any more - stale data

  for (vector<User>::iterator it = obj_->users.begin(); it != obj_->users.end();) {
    if (userAround.find(it->id()) == userAround.end()){
      it = obj_->users.erase(it);
    } else {
      it++;
    }
  }
  
  return rc;
}

/*void OpenNISkeleton::stopSkeletonTracking(nite::UserId id) {
  niteStopSkeletonTracking(user_tracker_handle_, id);
}

nite::Status OpenNISkeleton::startPoseDetection(nite::UserId user, nite::PoseType type) {
    return (nite::Status)niteStartPoseDetection(user_tracker_handle_, (NiteUserId)user, (NitePoseType)type);
}

void OpenNISkeleton::stopPoseDetection(nite::UserId user, nite::PoseType type){
    niteStopPoseDetection(user_tracker_handle_, (NiteUserId)user, (NitePoseType)type);
}*/

/* TODO - sort shared pointer
void addNewFrameListener(NewFrameListener* pListener) {
  niteRegisterUserTrackerCallbacks(user_tracker_handle_, &pListener->getCallbacks(), pListener);
  pListener->setUserTracker(this);
}

void removeNewFrameListener(NewFrameListener* pListener){
  niteUnregisterUserTrackerCallbacks(user_tracker_handle_, &pListener->getCallbacks());
  pListener->setUserTracker(NULL);
}*/

nite::Status OpenNISkeleton::ConvertJointCoordinatesToDepth(float x, float y, float z, float* pOutX, float* pOutY) const {
  return (nite::Status)niteConvertJointCoordinatesToDepth(obj_->user_tracker_handle, x, y, z, pOutX, pOutY);
}

nite::Status OpenNISkeleton::ConvertDepthCoordinatesToJoint(int x, int y, int z, float* pOutX, float* pOutY) const {
  return (nite::Status)niteConvertDepthCoordinatesToJoint(obj_->user_tracker_handle, x, y, z, pOutX, pOutY);
}

float OpenNISkeleton::GetSkeletonSmoothingFactor() const {
  float factor;
  nite::Status rc = (nite::Status)niteGetSkeletonSmoothing(obj_->user_tracker_handle, &factor);
  if (rc != nite::STATUS_OK) {
    factor = 0;
  }
  return factor;
}

nite::Status OpenNISkeleton::SetSkeletonSmoothingFactor(float factor) {
  return (nite::Status)niteSetSkeletonSmoothing(obj_->user_tracker_handle, factor);
}


/**
 * Copy the values from the NiTE Skeleton to the S9 Skeleton on this user
 */


void SetBone(int &idx, s9::Skeleton &skel, nite::Skeleton &nskel, nite::JointType joint) {

  nite::SkeletonJoint njoint = nskel.getJoint(joint);
  NiteQuaternion orientation = njoint.getOrientation();
  Bone * parent = skel.GetBone(idx)->parent();
  NitePoint3f npoint = njoint.getPosition();

  glm::quat global_rot = glm::quat(orientation.w, orientation.x, orientation.y, orientation.z );
  glm::vec3 global_pos = glm::vec3(npoint.x, npoint.y, npoint.z);

  if (parent != nullptr){
    skel.GetBone(idx)->set_rotation_relative(glm::normalize(glm::inverse(parent->rotation_relative()) * global_rot));
    glm::vec3 tp = global_pos - parent->position_relative();
    tp = tp * glm::toMat3(parent->rotation_relative());
    skel.GetBone(idx)->set_position_relative( tp );

     skel.GetBone(idx)->set_rotation_global(global_rot);

  } else {
    skel.GetBone(idx)->set_rotation_relative(global_rot );
    skel.GetBone(idx)->set_position_relative(global_pos );
  }

  skel.GetBone(idx)->set_rotation_global(global_rot);
  skel.GetBone(idx)->set_position_global(global_pos);
  
  idx++;
}

void OpenNISkeleton::User::CopySkeleton(){
  
  s9::Skeleton skel = skeleton();
  nite::Skeleton nskel = skeleton_nite();

  nite::SkeletonJoint njoint;
  NiteQuaternion orientation;
  NitePoint3f npoint;

  int idx = 0;

  ///\todo confidence checking here? Do we want to add that or deal with it here?

  // We do this in order, creating local transformations from OpenNI's global ones

  SetBone(idx,skel,nskel,nite::JOINT_TORSO);
  SetBone(idx,skel,nskel,nite::JOINT_NECK);
  SetBone(idx,skel,nskel,nite::JOINT_HEAD);
  SetBone(idx,skel,nskel,nite::JOINT_LEFT_SHOULDER);
  SetBone(idx,skel,nskel,nite::JOINT_LEFT_ELBOW);
  SetBone(idx,skel,nskel,nite::JOINT_LEFT_HAND);
  SetBone(idx,skel,nskel,nite::JOINT_RIGHT_SHOULDER);
  SetBone(idx,skel,nskel,nite::JOINT_RIGHT_ELBOW);
  SetBone(idx,skel,nskel,nite::JOINT_RIGHT_HAND);
  SetBone(idx,skel,nskel,nite::JOINT_LEFT_HIP);
  SetBone(idx,skel,nskel,nite::JOINT_LEFT_KNEE);
  SetBone(idx,skel,nskel,nite::JOINT_LEFT_FOOT);
  SetBone(idx,skel,nskel,nite::JOINT_RIGHT_HIP);
  SetBone(idx,skel,nskel,nite::JOINT_RIGHT_KNEE);
  SetBone(idx,skel,nskel,nite::JOINT_RIGHT_FOOT);

  /*
   const nite::SkeletonJoint& head = user.getSkeleton().getJoint(nite::JOINT_HEAD);
      if (head.getPositionConfidence() > .5)
        cout << user.id() << "," << head.getPosition().x << "," << head.getPosition().y << "," << head.getPosition().z << endl;
  */
}


/**
 * Keep a static set of bools and states for the all users here
 */

void OpenNISkeleton::UpdateUsersState(const User& user, unsigned long long ts) {
  if (user.IsNew())
    USER_MESSAGE(user, "New");
  else if (user.IsVisible() && !sVisibleUsers[user.id()])
    USER_MESSAGE(user,"Visible");
  else if (!user.IsVisible() && sVisibleUsers[user.id()])
    USER_MESSAGE(user,"Out of Scene");
  else if (user.IsLost())
    USER_MESSAGE(user,"Lost");

  sVisibleUsers[user.id()] = user.IsVisible();

  if(sSkeletonStates[user.id()] != user.skeleton_nite().getState())  {
    switch(sSkeletonStates[user.id()] = user.skeleton_nite().getState()) {
    case nite::SKELETON_NONE:
      USER_MESSAGE(user,"Stopped tracking.");
      break;
    case nite::SKELETON_CALIBRATING:
      USER_MESSAGE(user,"Calibrating...");
      break;
    case nite::SKELETON_TRACKED:
      USER_MESSAGE(user,"Tracking!");
      break;
    case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
    case nite::SKELETON_CALIBRATION_ERROR_HANDS:
    case nite::SKELETON_CALIBRATION_ERROR_LEGS:
    case nite::SKELETON_CALIBRATION_ERROR_HEAD:
    case nite::SKELETON_CALIBRATION_ERROR_TORSO:
      USER_MESSAGE(user,"Calibration Failed... :-|");
      break;
    }
  }
}

/**
 * Given a User ID, return a copy of that user, if they exist.
 */

OpenNISkeleton::User OpenNISkeleton::GetUserByID(int id){
  for (User user : obj_->users) {
    if (user.id() == id){
      return user;
    }
  }
  return User(); // A blank user
}

void OpenNISkeleton::RestartTracking() {

  nite::Status rc;
  obj_->ready = false;

  for (User &user : obj_->users) {
    if(sSkeletonStates[user.id()] == nite::SKELETON_TRACKED)  {  
      niteStopSkeletonTracking(obj_->user_tracker_handle, user.id());
      cout << "SEBUTO NITE - Stopped tracking user " << user.id() << endl;

    }
  }

  for (size_t i =0; i < S9_NITE_MAX_USERS; ++i) {
    sVisibleUsers[i] = false;
    sSkeletonStates[i] = nite::SKELETON_NONE;
  }


  Init();

}


/**
 * Called as often as possible to grab a new frame and update the user data
 */

void OpenNISkeleton::Update() {

  nite::Status rc;

  if (obj_ == nullptr || !obj_->ready)
    return;

  rc = ReadFrame();
  if (rc != nite::STATUS_OK) {
    cerr << "SEBURO NITE Error - Get next frame failed" << endl;
    return;
  }

  // If we have users, check if they are new and start tracking or continue tracking
  for (User &user : obj_->users) {

    UpdateUsersState(user, obj_->timestamp);
    if (user.IsNew()) {
      rc = (nite::Status)niteStartSkeletonTracking(obj_->user_tracker_handle, user.id());
      if (rc != nite::STATUS_OK) {
        cerr << "SEBURO NITE Error - Cannot start skeleton tracking for userid: " << user.id() << ", Error - " << rc << endl;
      }
    } else if (user.skeleton_nite().getState() == nite::SKELETON_TRACKED) {
      user.CopySkeleton();   
    }
  }
}

OpenNISkeleton::SharedObject::~SharedObject () {
  
  if (pNiteFrame != nullptr)
    delete pNiteFrame;

  if (ready)
    niteShutdownUserTracker(user_tracker_handle);
  
  nite::NiTE::shutdown();
}