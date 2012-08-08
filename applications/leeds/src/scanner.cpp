/**
* @brief Body of the Scanner class
* @file scanner.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#include "scanner.hpp"


using namespace std;
using namespace boost;
using namespace boost::assign;
using namespace s9;
using namespace s9::gl;
using namespace s9::compvis;
using namespace cv;
using namespace leeds;


Scanner::Scanner(){
;

  _detect_proc.addBlock<BlockGreyscale>()
    .addBlock<BlockThreshold>()
    .addBlock<BlockDetectPoint>();
}


bool Scanner::detectPoint(CVVidCam cam, cv::Point2f &point){
  cv::Mat result = _detect_proc.process(cam.getImageRectified());

  point = cv::Point2f(_detect_proc.getBlock(2).getValue<cv::Point2f>("point"));
  return _detect_proc.getBlock(2).getValue<bool>("found");
}
