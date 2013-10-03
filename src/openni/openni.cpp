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


OpenNIBase::SharedObj::~SharedObj() {
  mDepthStream.destroy();
  mColourStream.destroy();
  openni::OpenNI::shutdown(); // Assuming there is only one base ;)
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
  
  init();
}

void OpenNIBase::init( ) {

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

  // Texture map init
  //m_nTexMapX = MIN_CHUNKS_SIZE(m_width, TEXTURE_SIZE);
  //m_nTexMapY = MIN_CHUNKS_SIZE(m_height, TEXTURE_SIZE);
  //m_pTexMap = new openni::RGB888Pixel[m_nTexMapX * m_nTexMapY];



}

void OpenNIBase::update() {
  int changedIndex;
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

}