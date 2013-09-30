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


// Taken from the OpenNI Libraries

void OpenNIBase::calculateHistogram(float* pHistogram, int histogramSize, const openni::VideoFrameRef& frame) {
  const openni::DepthPixel* pDepth = (const openni::DepthPixel*)frame.getData();
  // Calculate the accumulative histogram (the yellow display...)
  memset(pHistogram, 0, histogramSize*sizeof(float));
  int restOfRow = frame.getStrideInBytes() / sizeof(openni::DepthPixel) - frame.getWidth();
  int height = frame.getHeight();
  int width = frame.getWidth();

  unsigned int nNumberOfPoints = 0;
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x, ++pDepth)
    {
      if (*pDepth != 0)
      {
        pHistogram[*pDepth]++;
        nNumberOfPoints++;
      }
    }
    pDepth += restOfRow;
  }
  for (int nIndex=1; nIndex<histogramSize; nIndex++)
  {
    pHistogram[nIndex] += pHistogram[nIndex-1];
  }
  if (nNumberOfPoints)
  {
    for (int nIndex=1; nIndex<histogramSize; nIndex++)
    {
      pHistogram[nIndex] = (256 * (1.0f - (pHistogram[nIndex] / nNumberOfPoints)));
    }
  }
}

OpenNIBase::OpenNIBase( const char* deviceURI) {
  openni::Status rc = openni::STATUS_OK;
  openni::Device device;
  openni::VideoStream depth, color;

  rc = openni::OpenNI::initialize();

  cout << "SEBURO OpenNI: After initialization: " << openni::OpenNI::getExtendedError() << endl;

  rc = device.open(deviceURI);
  if (rc != openni::STATUS_OK) {
    cout << "SEBURO OpenNI: Device open failed: " <<  openni::OpenNI::getExtendedError() << endl;
    openni::OpenNI::shutdown();

  }

  rc = depth.create(device, openni::SENSOR_DEPTH);
  if (rc == openni::STATUS_OK) {
    rc = depth.start();
    if (rc != openni::STATUS_OK) {
      cout << "SEBURO OpenNI: Couldn't start depth stream: " << openni::OpenNI::getExtendedError() << endl;
      depth.destroy();
    }
  }
  else {
    cout << "SEBURO OpenNI: Couldn't find depth stream: " << openni::OpenNI::getExtendedError() << endl;
  }

  rc = color.create(device, openni::SENSOR_COLOR);
  if (rc == openni::STATUS_OK) {
    rc = color.start();
    if (rc != openni::STATUS_OK) {
      cout <<  "SEBURO OpenNI: Couldn't start color stream: " << openni::OpenNI::getExtendedError() << endl;
      color.destroy();
    }
  }
  else {
    cout << "SEBURO OpenNI: Couldn't find color stream: " << openni::OpenNI::getExtendedError() << endl;
  }

  if (!depth.isValid() || !color.isValid()) {
    cout << "SEBURO OpenNI: No valid streams. Exiting." << endl;
    openni::OpenNI::shutdown();
  }
  
  _obj.reset(new SharedObj(device,depth,color));

  init(device,depth,color);
}

int OpenNIBase::init(openni::Device& d, openni::VideoStream&  ds, openni::VideoStream&  cs ) {

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

void OpenNIBase::update() {
  int changedIndex;
  openni::Status rc = openni::OpenNI::waitForAnyStream(_obj->mStreams, 2, &changedIndex);
  if (rc != openni::STATUS_OK) {
    cout << "SEBURO OpenNI - Wait failed" << endl;
    return;
  }
  switch (changedIndex){
    case 0:
      _obj->mDepthStream.readFrame(& (_obj->mDepthFrame)); break;
    case 1:
      _obj->mColourStream.readFrame(&(_obj->mColourFrame)); break;
    default:
      cout << "SEBURO OpenNI - Wait error" << endl;
  }

  if (_obj->mDepthFrame.isValid()) {
    calculateHistogram(_obj->mDepthHist, S9_OPENNI_MAX_DEPTH, _obj->mDepthFrame);
  }
  cout << "OpenNI Update" << endl;
}

OpenNIBase::~OpenNIBase() {

}