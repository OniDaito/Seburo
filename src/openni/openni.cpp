/**
* @brief OpenNI Basic helper classes
* @file openni.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 18/09/2013
*
*/

#include "s9/openni/openni.hpp"

using namespace std;
using namespace boost; 
using namespace s9;
using namespace s9::oni;


OpenNIBase::OpenNIBase() { }

OpenNIBase::OpenNIBase(std::string device_name) {
  openni::Status rc = openni::STATUS_OK;
  openni::Device device;
  openni::VideoStream depth, color;
  const char* deviceURI = openni::ANY_DEVICE;

  // TODO - specify device

  rc = openni::OpenNI::initialize();

  cout << "After initialization: " << openni::OpenNI::getExtendedError() << endl;

  rc = device.open(deviceURI);
  if (rc != openni::STATUS_OK) {
    cout << "Device open failed: " <<  openni::OpenNI::getExtendedError() << endl;
    openni::OpenNI::shutdown();

  }

  rc = depth.create(device, openni::SENSOR_DEPTH);
  if (rc == openni::STATUS_OK) {
    rc = depth.start();
    if (rc != openni::STATUS_OK) {
      cout << "SimpleViewer: Couldn't start depth stream: " << openni::OpenNI::getExtendedError() << endl;
      depth.destroy();
    }
  }
  else {
    cout << "SimpleViewer: Couldn't find depth stream: " << openni::OpenNI::getExtendedError() << endl;
  }

  rc = color.create(device, openni::SENSOR_COLOR);
  if (rc == openni::STATUS_OK) {
    rc = color.start();
    if (rc != openni::STATUS_OK) {
      cout <<  "SimpleViewer: Couldn't start color stream: " << openni::OpenNI::getExtendedError() << endl;
      color.destroy();
    }
  }
  else {
    cout << "SimpleViewer: Couldn't find color stream: " << openni::OpenNI::getExtendedError() << endl;
  }

  if (!depth.isValid() || !color.isValid()) {
    cout << "SimpleViewer: No valid streams. Exiting." << endl;
    openni::OpenNI::shutdown();
  }

  init(device,depth,color);
}



int OpenNIBase::init(openni::Device& d, openni::VideoStream&  ds, openni::VideoStream&  cs ) {

  openni::VideoMode depthVideoMode;
  openni::VideoMode colorVideoMode;

  _obj.reset(new SharedObj(d,ds,cs));

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
      cout << "Error - expect color and depth to be in same resolution: D:" << depthWidth 
        << "x" << depthHeight << " C: " << colorWidth << "x" << depthWidth << endl;
      return openni::STATUS_ERROR;
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
    cout <<  "Error - expects at least one of the streams to be valid." << endl;
    return openni::STATUS_ERROR;
  }

  _obj->mStreams = new openni::VideoStream*[2];
  _obj->mStreams[0] = &(_obj->mDepthStream);
  _obj->mStreams[1] = &(_obj->mColourStream);

  // Texture map init
  //m_nTexMapX = MIN_CHUNKS_SIZE(m_width, TEXTURE_SIZE);
  //m_nTexMapY = MIN_CHUNKS_SIZE(m_height, TEXTURE_SIZE);
  //m_pTexMap = new openni::RGB888Pixel[m_nTexMapX * m_nTexMapY];

}

OpenNIBase::~OpenNIBase() {

}