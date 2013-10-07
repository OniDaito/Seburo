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


inline void USER_MESSAGE (const nite::UserData& user, std::string msg) {
  cout << "SEBURO Nite - User: " << user.getId() << ", " << msg << endl;
}

OpenNIBase::SharedObj::~SharedObj() {

  niteShutdownUserTracker(mUserTrackerHandle);

  mDepthStream.destroy();
  mColourStream.destroy();

  // this line seems to cause problems with the extended nite shared object :S
  // Also, this seems to be called twice
  nite::NiTE::shutdown();
  openni::OpenNI::shutdown(); // Assuming there is only one base ;)
  
  delete pTexMapBuffer;
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

OpenNIBase::OpenNIBase( const char* deviceURI) : _obj( shared_ptr<SharedObj> (new SharedObj())) {
  openni::Status rc = openni::STATUS_OK;

  rc = openni::OpenNI::initialize();

  cout << "SEBURO OpenNI: After initialization: " << openni::OpenNI::getExtendedError() << endl;

  rc = _obj->mDevice.open(deviceURI);
  if (rc != openni::STATUS_OK) {
    cerr << "SEBURO OpenNI: Device open failed: " <<  openni::OpenNI::getExtendedError() << endl;
    openni::OpenNI::shutdown();
  }

  rc = _obj->mDepthStream.create(_obj->mDevice, openni::SENSOR_DEPTH);
  if (rc == openni::STATUS_OK) {
    rc = _obj->mDepthStream.start();
    if (rc != openni::STATUS_OK) {
      cerr << "SEBURO OpenNI: Couldn't start depth stream: " << openni::OpenNI::getExtendedError() << endl;
      _obj->mDepthStream.destroy();
    }
  }
  else {
    cerr << "SEBURO OpenNI: Couldn't find depth stream: " << openni::OpenNI::getExtendedError() << endl;
  }


  rc = _obj->mColourStream.create(_obj->mDevice, openni::SENSOR_COLOR);
  if (rc == openni::STATUS_OK) {
    rc = _obj->mColourStream.start();
    if (rc != openni::STATUS_OK) {
      cerr <<  "SEBURO OpenNI: Couldn't start color stream: " << openni::OpenNI::getExtendedError() << endl;
      _obj->mColourStream.destroy();
    }
  }
  else {
    cerr << "SEBURO OpenNI: Couldn't find color stream: " << openni::OpenNI::getExtendedError() << endl;
  }

  if (!(_obj->mDepthStream.isValid()) || !(_obj->mColourStream.isValid())) {
    cerr << "SEBURO OpenNI: No valid streams. Exiting." << endl;
    openni::OpenNI::shutdown();
  }
  

  openni::VideoMode depthVideoMode;
  openni::VideoMode colorVideoMode;


  if (_obj->mDepthStream.isValid() && _obj->mColourStream.isValid()) {
    depthVideoMode = _obj->mDepthStream.getVideoMode();
    colorVideoMode = _obj->mColourStream.getVideoMode();

    int depthWidth = depthVideoMode.getResolutionX();
    int depthHeight = depthVideoMode.getResolutionY();
    int colorWidth = colorVideoMode.getResolutionX();
    int colorHeight = colorVideoMode.getResolutionY();

    if (depthWidth == colorWidth && depthHeight == colorHeight) {
      _obj->mWidth = depthWidth;
      _obj->mHeight = depthHeight;
    } else {
      cout << "SEBURO OpenNI Error: expect color and depth to be in same resolution: D:" << depthWidth 
        << "x" << depthHeight << " C: " << colorWidth << "x" << depthWidth << endl;
      return;
    }
  } else if (_obj->mDepthStream.isValid()) {
    depthVideoMode = _obj->mDepthStream.getVideoMode();
    _obj->mWidth = depthVideoMode.getResolutionX();
    _obj->mHeight = depthVideoMode.getResolutionY();
  }
  else if (_obj->mColourStream.isValid()) {
    colorVideoMode = _obj->mColourStream.getVideoMode();
    _obj->mWidth = colorVideoMode.getResolutionX();
    _obj->mHeight = colorVideoMode.getResolutionY();
  }
  else {
    cout <<  "SEBURO OpenNI Error: expects at least one of the streams to be valid." << endl;
    return;
  }

  _obj->mStreams = new openni::VideoStream*[2];
  _obj->mStreams[0] = &(_obj->mDepthStream);
  _obj->mStreams[1] = &(_obj->mColourStream);

  _obj->pTexMapBuffer = new openni::RGB888Pixel[_obj->mWidth * _obj->mHeight];

  _obj->mReady = true;

}



void OpenNIBase::update() {
  int changedIndex;

  if(!_obj->mReady)
    return;


  openni::Status rc = openni::OpenNI::waitForAnyStream(_obj->mStreams, 2, &changedIndex);
  if (rc != openni::STATUS_OK) {
    cout << "SEBURO OpenNI Error: Wait failed" << endl;
    return;
  }
  switch (changedIndex){
    case 0:
      _obj->mDepthStream.readFrame(& (_obj->mDepthFrame)); break;
    case 1:
      _obj->mColourStream.readFrame(&(_obj->mColourFrame)); break;
    default:
      cout << "SEBURO OpenNI Error: Wait error" << endl;
  }

  if (_obj->mDepthFrame.isValid()) {
    calculateHistogram(_obj->mDepthHist, S9_OPENNI_MAX_DEPTH, _obj->mDepthFrame);
  }

  int dim = _obj->mWidth * _obj->mHeight;

  memset(_obj->pTexMapBuffer, 0, dim * sizeof(openni::RGB888Pixel));

  // check if we need to draw image frame to texture
  if (_obj->mColourFrame.isValid()) {
    const openni::RGB888Pixel* pImageRow = (const openni::RGB888Pixel*) _obj->mColourFrame.getData();
    openni::RGB888Pixel* pTexRow = _obj->pTexMapBuffer + _obj->mColourFrame.getCropOriginY() * _obj->mWidth;
    int rowSize = _obj->mColourFrame.getStrideInBytes() / sizeof(openni::RGB888Pixel);

    for (int y = 0; y < _obj->mColourFrame.getHeight(); ++y)
    {
      const openni::RGB888Pixel* pImage = pImageRow;
      openni::RGB888Pixel* pTex = pTexRow + _obj->mColourFrame.getCropOriginX();

      for (int x = 0; x < _obj->mColourFrame.getWidth(); ++x, ++pImage, ++pTex)
      {
        *pTex = *pImage;
      }

      pImageRow += rowSize;
      pTexRow += _obj->mWidth;
    }
  }

  // Buffer is now set to colour
  // TODO - Pass to a texture

  // check if we need to draw depth frame to texture
  if (_obj->mDepthFrame.isValid()) {
    const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)_obj->mDepthFrame.getData();
    openni::RGB888Pixel* pTexRow = _obj->pTexMapBuffer + _obj->mDepthFrame.getCropOriginY() * _obj->mWidth;
    int rowSize = _obj->mDepthFrame.getStrideInBytes() / sizeof(openni::DepthPixel);

    for (int y = 0; y < _obj->mDepthFrame.getHeight(); ++y) {
      const openni::DepthPixel* pDepth = pDepthRow;
      openni::RGB888Pixel* pTex = pTexRow + _obj->mDepthFrame.getCropOriginX();

      for (int x = 0; x < _obj->mDepthFrame.getWidth(); ++x, ++pDepth, ++pTex)
      {
        if (*pDepth != 0)
        {
          int nHistValue = _obj->mDepthHist[*pDepth];
          pTex->r = nHistValue;
          pTex->g = nHistValue;
          pTex->b = 0;
        }
      }

      pDepthRow += rowSize;
      pTexRow +=_obj->mWidth;
    }
  }

  // Buffer is now set to depth

}

OpenNISkeleton::OpenNISkeleton(const char * deviceURI) : OpenNIBase(deviceURI) {

  nite::Status niteRc;
  niteRc = nite::NiTE::initialize();
  cout << "SEBURO NITE Status: " << niteRc << endl;

  niteRc = (nite::Status) niteInitializeUserTrackerByDevice(&(_obj->mDevice), &(_obj->mUserTrackerHandle));
  if (niteRc != nite::STATUS_OK) {
    cerr << "SEBURO NITE Error - Could not initialize tracker: " << niteRc << endl;
    return;
  }

  pFrameListener = std::shared_ptr<NewFrameListener>(new NewFrameListener(*this));
  pFrame = std::shared_ptr<OpenNISkeletonFrame>(new OpenNISkeletonFrame());

}

// Listener class internal callback function

void  OpenNISkeleton::NewFrameListener::onNewFrame( OpenNISkeleton &skeleton) {

}

/**
 * Read a frame from NiTE and slot it into our member frame class
 */

nite::Status OpenNISkeleton::readFrame() {
  NiteUserTrackerFrame *pNiteFrame = NULL;
  nite::Status rc = (nite::Status)niteReadUserTrackerFrame(_obj->mUserTrackerHandle, &pNiteFrame);
  
  /*// Set our frame class
  pFrame->pNiteFrame = pNiteFrame;
  pFrame->mDepthFrame._setFrame(pNiteFrame->pDepthFrame);
  pFrame->aUsers.clear();

  nite::UserData* pv = (nite::UserData*)pNiteFrame->pUser;
  for (uint16_t i = 0; i < pNiteFrame->userCount; i++){
    pFrame->aUsers.push_back(pv);
    pv += sizeof(nite::UserData);
  }*/

  if (pNiteFrame->userCount > 0)
    cout << "Users " << pNiteFrame->userCount << endl;


  //pFrame->aUsers.setData(pNiteFrame->userCount, (nite::UserData*)pNiteFrame->pUser);


  return rc;
}

nite::Status OpenNISkeleton::startSkeletonTracking(nite::UserId id) {
    return (nite::Status)niteStartSkeletonTracking(_obj->mUserTrackerHandle, id);
}

void OpenNISkeleton::stopSkeletonTracking(nite::UserId id) {
  niteStopSkeletonTracking(_obj->mUserTrackerHandle, id);
}

nite::Status OpenNISkeleton::startPoseDetection(nite::UserId user, nite::PoseType type) {
    return (nite::Status)niteStartPoseDetection(_obj->mUserTrackerHandle, (NiteUserId)user, (NitePoseType)type);
}

void OpenNISkeleton::stopPoseDetection(nite::UserId user, nite::PoseType type){
    niteStopPoseDetection(_obj->mUserTrackerHandle, (NiteUserId)user, (NitePoseType)type);
}

/* TODO - sort shared pointer
void addNewFrameListener(NewFrameListener* pListener) {
  niteRegisterUserTrackerCallbacks(_obj->mUserTrackerHandle, &pListener->getCallbacks(), pListener);
  pListener->setUserTracker(this);
}

void removeNewFrameListener(NewFrameListener* pListener){
  niteUnregisterUserTrackerCallbacks(_obj->mUserTrackerHandle, &pListener->getCallbacks());
  pListener->setUserTracker(NULL);
}*/

nite::Status OpenNISkeleton::convertJointCoordinatesToDepth(float x, float y, float z, float* pOutX, float* pOutY) const {
  return (nite::Status)niteConvertJointCoordinatesToDepth(_obj->mUserTrackerHandle, x, y, z, pOutX, pOutY);
}

nite::Status OpenNISkeleton::convertDepthCoordinatesToJoint(int x, int y, int z, float* pOutX, float* pOutY) const {
  return (nite::Status)niteConvertDepthCoordinatesToJoint(_obj->mUserTrackerHandle, x, y, z, pOutX, pOutY);
}

float OpenNISkeleton::getSkeletonSmoothingFactor() const {
  float factor;
  nite::Status rc = (nite::Status)niteGetSkeletonSmoothing(_obj->mUserTrackerHandle, &factor);
  if (rc != nite::STATUS_OK) {
    factor = 0;
  }
  return factor;
}

nite::Status OpenNISkeleton::setSkeletonSmoothingFactor(float factor) {
  return (nite::Status)niteSetSkeletonSmoothing(_obj->mUserTrackerHandle, factor);
}

void OpenNISkeleton::updateUserState(const nite::UserData& user, unsigned long long ts) {
  if (user.isNew())
    USER_MESSAGE(user, "New");
  else if (user.isVisible() && !sVisibleUsers[user.getId()])
    USER_MESSAGE(user,"Visible");
  else if (!user.isVisible() && sVisibleUsers[user.getId()])
    USER_MESSAGE(user,"Out of Scene");
  else if (user.isLost())
    USER_MESSAGE(user,"Lost");

  sVisibleUsers[user.getId()] = user.isVisible();


  if(sSkeletonStates[user.getId()] != user.getSkeleton().getState())  {
    switch(sSkeletonStates[user.getId()] = user.getSkeleton().getState()) {
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
 * Called as often as possible to grab a new frame and update the user data
 */

void OpenNISkeleton::update() {

  nite::Status niteRc = readFrame();
  if (niteRc != nite::STATUS_OK) {
    cerr << "SEBURO NITE Error - Get next frame failed" << endl;
    return;
  }

 /* const std::list<nite::UserData*>& users = pFrame->getUsers();

  for (nite::UserData* user : users) {

    updateUserState(*user,pFrame->getTimestamp());
    if (user->isNew()) {
      startSkeletonTracking(user->getId());
    }
    else if (user->getSkeleton().getState() == nite::SKELETON_TRACKED) {
      const nite::SkeletonJoint& head = user->getSkeleton().getJoint(nite::JOINT_HEAD);
      if (head.getPositionConfidence() > .5)
        cout << user->getId() << "," << head.getPosition().x << "," << head.getPosition().y << "," << head.getPosition().z << endl;
    }
  }*/
}