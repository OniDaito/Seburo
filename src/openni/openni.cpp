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


bool OpenNISkeleton::sVisibleUsers[S9_NITE_MAX_USERS] = {false};
nite::SkeletonState OpenNISkeleton::sSkeletonStates[S9_NITE_MAX_USERS] = {nite::SKELETON_NONE};


inline void USER_MESSAGE (const OpenNISkeleton::User& user, std::string msg) {
  cout << "SEBURO Nite - User: " << user.id() << ", " << msg << endl;
}

OpenNIBase::SharedObj::~SharedObj() {

  depth_stream_.destroy();
  colour_stream_.destroy();

  // this line seems to cause problems with the extended nite shared object :S
  // Also, this seems to be called twice

  openni::OpenNI::shutdown(); // Assuming there is only one base ;)
  
  if (ready_){ // we allocated so we must un-allocate
    delete[] tex_buffer_depth_;
    delete[] tex_buffer_colour_;
  }
 
}


// Taken from the OpenNI Libraries

void OpenNIBase::calculateHistogram(float* pHistogram, int histogramSize, const openni::VideoFrameRef& frame) {
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

OpenNIBase::OpenNIBase( const char* deviceURI) : obj_( shared_ptr<SharedObj> (new SharedObj())) {
  openni::Status rc = openni::STATUS_OK;

  rc = openni::OpenNI::initialize();

  cout << "SEBURO OpenNI: After initialization: " << openni::OpenNI::getExtendedError() << endl;

  rc = obj_->device_.open(deviceURI);
  if (rc != openni::STATUS_OK) {
    cerr << "SEBURO OpenNI: Device open failed: " <<  openni::OpenNI::getExtendedError() << endl;
    openni::OpenNI::shutdown();
  }

  rc = obj_->depth_stream_.create(obj_->device_, openni::SENSOR_DEPTH);
  if (rc == openni::STATUS_OK) {
    rc = obj_->depth_stream_.start();
    if (rc != openni::STATUS_OK) {
      cerr << "SEBURO OpenNI: Couldn't start depth stream: " << openni::OpenNI::getExtendedError() << endl;
      obj_->depth_stream_.destroy();
    }
  }
  else {
    cerr << "SEBURO OpenNI: Couldn't find depth stream: " << openni::OpenNI::getExtendedError() << endl;
  }


  rc = obj_->colour_stream_.create(obj_->device_, openni::SENSOR_COLOR);
  if (rc == openni::STATUS_OK) {
    rc = obj_->colour_stream_.start();
    if (rc != openni::STATUS_OK) {
      cerr <<  "SEBURO OpenNI: Couldn't start color stream: " << openni::OpenNI::getExtendedError() << endl;
      obj_->colour_stream_.destroy();
    }
  }
  else {
    cerr << "SEBURO OpenNI: Couldn't find color stream: " << openni::OpenNI::getExtendedError() << endl;
  }

  if (!(obj_->depth_stream_.isValid()) || !(obj_->colour_stream_.isValid())) {
    cerr << "SEBURO OpenNI: No valid streams. Exiting." << endl;
    openni::OpenNI::shutdown();
  }
 

  openni::VideoMode depthVideoMode;
  openni::VideoMode colorVideoMode;


  if (obj_->depth_stream_.isValid() && obj_->colour_stream_.isValid()) {
    depthVideoMode = obj_->depth_stream_.getVideoMode();
    colorVideoMode = obj_->colour_stream_.getVideoMode();

    int depthWidth = depthVideoMode.getResolutionX();
    int depthHeight = depthVideoMode.getResolutionY();
    int colorWidth = colorVideoMode.getResolutionX();
    int colorHeight = colorVideoMode.getResolutionY();

    if (depthWidth == colorWidth && depthHeight == colorHeight) {
      obj_->width_ = depthWidth;
      obj_->height_ = depthHeight;
    } else {
      cout << "SEBURO OpenNI Error: expect color and depth to be in same resolution: D:" << depthWidth 
        << "x" << depthHeight << " C: " << colorWidth << "x" << depthWidth << endl;
      return;
    }
  } else if (obj_->depth_stream_.isValid()) {
    depthVideoMode = obj_->depth_stream_.getVideoMode();
    obj_->width_ = depthVideoMode.getResolutionX();
    obj_->height_ = depthVideoMode.getResolutionY();
  }
  else if (obj_->colour_stream_.isValid()) {
    colorVideoMode = obj_->colour_stream_.getVideoMode();
    obj_->width_ = colorVideoMode.getResolutionX();
    obj_->height_ = colorVideoMode.getResolutionY();
  }
  else {
    cout <<  "SEBURO OpenNI Error: expects at least one of the streams to be valid." << endl;
    return;
  }

  obj_->streams_ = new openni::VideoStream*[2];
  obj_->streams_[0] = &(obj_->depth_stream_);
  obj_->streams_[1] = &(obj_->colour_stream_);

  obj_->tex_buffer_depth_ = new byte_t[obj_->width_ * obj_->height_];
  obj_->tex_buffer_colour_ = new byte_t[obj_->width_ * obj_->height_ * 3];

  // Allocate GL Texture
  obj_->texture_depth_ = gl::TextureStream(obj_->width_, obj_->height_, GREY);
  obj_->texture_colour_ = gl::TextureStream(obj_->width_, obj_->height_);

  obj_->ready_ = true;

  cout << "SEBURO OpenNI - Allocated Depth / Colour streams of " << obj_->width_ << "x" << obj_->height_ << "." << endl;
 
}



void OpenNIBase::update() {
  int changedIndex;

  if(!obj_->ready_)
    return;

  openni::Status rc = openni::OpenNI::waitForAnyStream(obj_->streams_, 2, &changedIndex);
  if (rc != openni::STATUS_OK) {
    cout << "SEBURO OpenNI Error: Wait failed" << endl;
    return;
  }
  switch (changedIndex){
    case 0:
      obj_->depth_stream_.readFrame(& (obj_->depth_frame_)); break;
    case 1:
      obj_->colour_stream_.readFrame(&(obj_->colour_frame_)); break;
    default:
      cout << "SEBURO OpenNI Error: Wait error" << endl;
  }

  if (obj_->depth_frame_.isValid()) {
    calculateHistogram(obj_->depth_hist_, S9_OPENNI_MAX_DEPTH, obj_->depth_frame_);
  }

  int buffer_size = obj_->width_ * obj_->height_ * sizeof(byte_t) * 3;

 
  // check if we need to draw image frame to texture
  // We dont bother with cropping and all that crap OpenNI likes to put in.
  if (obj_->colour_frame_.isValid()) {

    memset(obj_->tex_buffer_colour_, 0, buffer_size);
    memcpy(obj_->tex_buffer_colour_, obj_->colour_frame_.getData(), buffer_size);

  }

  // Buffer is now set to colour

  buffer_size = obj_->width_ * obj_->height_ * sizeof(byte_t);

  // check if we need to draw depth frame to texture
  if (obj_->depth_frame_.isValid()) {
   
    memset(obj_->tex_buffer_depth_, 0, buffer_size);

    byte_t *td = obj_->tex_buffer_depth_;
    uint16_t *ts = (uint16_t*) obj_->depth_frame_.getData(); // This is a 16bit integer for depth data

    for (int i=0; i < obj_->width_ * obj_->height_; ++i){
      *td = obj_->depth_hist_[*ts];
      ++td;
      ++ts;
    }


  }

  // Buffer is now set to depth

}

void OpenNIBase::update_textures() {
  if (obj_->ready_){
    obj_->texture_colour_.update(obj_->tex_buffer_colour_);
    obj_->texture_depth_.update(obj_->tex_buffer_depth_);
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

  // Perform initialisation if device is ready for it

  rc = (nite::Status) niteInitializeUserTrackerByDevice( (void*)&(base.device()), &(obj_->user_tracker_handle_));
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

nite::Status OpenNISkeleton::readFrame() {

  nite::Status rc;

  if (obj_->pNiteFrame != nullptr){
    niteUserTrackerFrameRelease(obj_->user_tracker_handle_, obj_->pNiteFrame);
    obj_->pNiteFrame = nullptr;
  
  }

  rc = (nite::Status)niteReadUserTrackerFrame(obj_->user_tracker_handle_, &(obj_->pNiteFrame));
  
  if (rc !=  nite::STATUS_OK){
    cerr << "SEBURO NITE Error - Could not read frame: " << rc << endl;
    return rc;
  }

  obj_->mFloor = obj_->pNiteFrame->floor;
  obj_->mUserMap = obj_->pNiteFrame->userMap;
  obj_->mFloorConfidence = obj_->pNiteFrame->floorConfidence;
  obj_->mTimeStamp = obj_->pNiteFrame->timestamp;

  set<NiteUserId> userAround;

  NiteUserData* pv = (NiteUserData*)obj_->pNiteFrame->pUser;

  for (int i = 0; i < obj_->pNiteFrame->userCount; ++i){

    User ud (pv[i]);

    bool found = false;
    for (User &dd : obj_->vUsers){
      if (ud.id() == dd.id()){
        found = true;
        dd = ud;
        userAround.insert(ud.id());
        break;
      }
    }

    if (!found){
      obj_->vUsers.push_back(ud);
      userAround.insert(ud.id());
    }
  }

  // Remove users not around any more - stale data

  for (vector<User>::iterator it = obj_->vUsers.begin(); it != obj_->vUsers.end();) {
    if (userAround.find(it->id()) == userAround.end()){
      it = obj_->vUsers.erase(it);
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

nite::Status OpenNISkeleton::convertJointCoordinatesToDepth(float x, float y, float z, float* pOutX, float* pOutY) const {
  return (nite::Status)niteConvertJointCoordinatesToDepth(obj_->user_tracker_handle_, x, y, z, pOutX, pOutY);
}

nite::Status OpenNISkeleton::convertDepthCoordinatesToJoint(int x, int y, int z, float* pOutX, float* pOutY) const {
  return (nite::Status)niteConvertDepthCoordinatesToJoint(obj_->user_tracker_handle_, x, y, z, pOutX, pOutY);
}

float OpenNISkeleton::getSkeletonSmoothingFactor() const {
  float factor;
  nite::Status rc = (nite::Status)niteGetSkeletonSmoothing(obj_->user_tracker_handle_, &factor);
  if (rc != nite::STATUS_OK) {
    factor = 0;
  }
  return factor;
}

nite::Status OpenNISkeleton::setSkeletonSmoothingFactor(float factor) {
  return (nite::Status)niteSetSkeletonSmoothing(obj_->user_tracker_handle_, factor);
}

/**
 * Copy the values from the NiTE Skeleton to the S9 Skeleton on this user
 */

void OpenNISkeleton::User::copySkeleton(){
  
  s9::Skeleton skel = skeleton();
  nite::Skeleton nskel = getSkeletonNiTE();

  nite::SkeletonJoint njoint;
  NiteQuaternion orientation;

  int idx = 0;

  ///\todo confidence checking here? Do we want to add that or deal with it here?

  njoint = nskel.getJoint(nite::JOINT_TORSO);
  orientation = njoint.getOrientation();
  skel.bone(idx++)->rotation = glm::quat(orientation.x, orientation.y, orientation.z, orientation.w );

  njoint = nskel.getJoint(nite::JOINT_NECK);
  orientation = njoint.getOrientation();
  skel.bone(idx++)->rotation = glm::quat(orientation.x, orientation.y, orientation.z, orientation.w );

  njoint = nskel.getJoint(nite::JOINT_HEAD);
  orientation = njoint.getOrientation();
  skel.bone(idx++)->rotation = glm::quat(orientation.x, orientation.y, orientation.z, orientation.w );

  njoint = nskel.getJoint(nite::JOINT_LEFT_SHOULDER);
  orientation = njoint.getOrientation();
  skel.bone(idx++)->rotation = glm::quat(orientation.x, orientation.y, orientation.z, orientation.w );

  njoint = nskel.getJoint(nite::JOINT_LEFT_ELBOW);
  orientation = njoint.getOrientation();
  skel.bone(idx++)->rotation = glm::quat(orientation.x, orientation.y, orientation.z, orientation.w );

  njoint = nskel.getJoint(nite::JOINT_LEFT_HAND);
  orientation = njoint.getOrientation();
  skel.bone(idx++)->rotation = glm::quat(orientation.x, orientation.y, orientation.z, orientation.w );

  njoint = nskel.getJoint(nite::JOINT_RIGHT_SHOULDER);
  orientation = njoint.getOrientation();
  skel.bone(idx++)->rotation = glm::quat(orientation.x, orientation.y, orientation.z, orientation.w );

  njoint = nskel.getJoint(nite::JOINT_RIGHT_ELBOW);
  orientation = njoint.getOrientation();
  skel.bone(idx++)->rotation = glm::quat(orientation.x, orientation.y, orientation.z, orientation.w );

  njoint = nskel.getJoint(nite::JOINT_RIGHT_HAND);
  orientation = njoint.getOrientation();
  skel.bone(idx++)->rotation = glm::quat(orientation.x, orientation.y, orientation.z, orientation.w );

  njoint = nskel.getJoint(nite::JOINT_LEFT_HIP);
  orientation = njoint.getOrientation();
  skel.bone(idx++)->rotation = glm::quat(orientation.x, orientation.y, orientation.z, orientation.w );

  njoint = nskel.getJoint(nite::JOINT_LEFT_KNEE);
  orientation = njoint.getOrientation();
  skel.bone(idx++)->rotation = glm::quat(orientation.x, orientation.y, orientation.z, orientation.w );

  njoint = nskel.getJoint(nite::JOINT_LEFT_FOOT);
  orientation = njoint.getOrientation();
  skel.bone(idx++)->rotation = glm::quat(orientation.x, orientation.y, orientation.z, orientation.w );

  njoint = nskel.getJoint(nite::JOINT_RIGHT_HIP);
  orientation = njoint.getOrientation();
  skel.bone(idx++)->rotation = glm::quat(orientation.x, orientation.y, orientation.z, orientation.w );

  njoint = nskel.getJoint(nite::JOINT_RIGHT_KNEE);
  orientation = njoint.getOrientation();
  skel.bone(idx++)->rotation = glm::quat(orientation.x, orientation.y, orientation.z, orientation.w );

  njoint = nskel.getJoint(nite::JOINT_RIGHT_FOOT);
  orientation = njoint.getOrientation();
  skel.bone(idx++)->rotation = glm::quat(orientation.x, orientation.y, orientation.z, orientation.w );

  /*
   const nite::SkeletonJoint& head = user.getSkeleton().getJoint(nite::JOINT_HEAD);
      if (head.getPositionConfidence() > .5)
        cout << user.id() << "," << head.getPosition().x << "," << head.getPosition().y << "," << head.getPosition().z << endl;
  */
}


/**
 * Keep a static set of bools and states for the all users here
 */

void OpenNISkeleton::updateUsersState(const User& user, unsigned long long ts) {
  if (user.isNew())
    USER_MESSAGE(user, "New");
  else if (user.isVisible() && !sVisibleUsers[user.id()])
    USER_MESSAGE(user,"Visible");
  else if (!user.isVisible() && sVisibleUsers[user.id()])
    USER_MESSAGE(user,"Out of Scene");
  else if (user.isLost())
    USER_MESSAGE(user,"Lost");

  sVisibleUsers[user.id()] = user.isVisible();

  if(sSkeletonStates[user.id()] != user.getSkeletonNiTE().getState())  {
    switch(sSkeletonStates[user.id()] = user.getSkeletonNiTE().getState()) {
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

OpenNISkeleton::User OpenNISkeleton::user(int id){
  for (User user : obj_->vUsers) {
    if (user.id() == id){
      return user;
    }
  }
  return User(); // A blank user
}

/**
 * Called as often as possible to grab a new frame and update the user data
 */

void OpenNISkeleton::update() {

  nite::Status rc;

  if (obj_ == nullptr || !obj_->ready)
    return;

  rc = readFrame();
  if (rc != nite::STATUS_OK) {
    cerr << "SEBURO NITE Error - Get next frame failed" << endl;
    return;
  }

  // If we have users, check if they are new and start tracking or continue tracking
  for (User &user : obj_->vUsers) {

    updateUsersState(user, obj_->mTimeStamp);
    if (user.isNew()) {
      rc = (nite::Status)niteStartSkeletonTracking(obj_->user_tracker_handle_, user.id());
      if (rc != nite::STATUS_OK) {
        cerr << "SEBURO NITE Error - Cannot start skeleton tracking for userid: " << user.id() << ", Error - " << rc << endl;
      }
    } else if (user.getSkeletonNiTE().getState() == nite::SKELETON_TRACKED) {
      user.copySkeleton();   
    }
  }
}

OpenNISkeleton::SharedObject::~SharedObject () {
  
  if (pNiteFrame != nullptr)
    delete pNiteFrame;

  if (ready)
    niteShutdownUserTracker(user_tracker_handle_);
  
  nite::NiTE::shutdown();
}