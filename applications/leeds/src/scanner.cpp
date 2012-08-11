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
using namespace s9::gl::compvis;
using namespace cv;
using namespace leeds;


Scanner::Scanner(){
;

  _detect_proc.addBlock<BlockGreyscale>()
    .addBlock<BlockThreshold>()
    .addBlock<BlockDetectPoint>();
}


bool Scanner::detectPoint(CVVidCam cam, cv::Point2f &point, cv::Mat &result){
  result = _detect_proc.process(cam.getImageRectified());
  point = cv::Point2f(_detect_proc.getBlock(2).getValue<cv::Point2f>("point"));
  return _detect_proc.getBlock(2).getValue<bool>("found");
}


cv::Point3f Scanner::solveForAll( std::vector< std::pair< cv::Point2f, CameraParameters > > points) {
  
  int idx = 0;
  int idy = 0;
  
  vector<Point2f> results;
  
  typedef std::pair<cv::Point2f, CameraParameters > CamPoint;

  Mat m (Size(3 + points.size(), 3 * points.size()), CV_64FC1);
  Mat s (Size(1,4), CV_64FC1);
  Mat c (Size(1,3 * points.size()), CV_64FC1);
  
  // Now create our matrices and solve
  idy = 0;
  
  BOOST_FOREACH ( CamPoint p, points) { 
    CameraParameters in = p.second;
      
    vector<Point2f> tpoints;
    tpoints.push_back(p.first);
    undistortPoints(tpoints,results, in.M, in.D);
  
    Mat r (Size(3,3), CV_64FC1);
    Rodrigues(in.R,r);
    
    // now create the solving matrices
    
    m.at<double_t>(idx,0) = r.at<double_t>(0,0);
    m.at<double_t>(idx,1) = r.at<double_t>(0,1);
    m.at<double_t>(idx,2) = r.at<double_t>(0,2);
    
    m.at<double_t>(idx + 1,0) = r.at<double_t>(1,0);
    m.at<double_t>(idx + 1,1) = r.at<double_t>(1,1);
    m.at<double_t>(idx + 1,2) = r.at<double_t>(1,2);
    
    m.at<double_t>(idx + 2,0) = r.at<double_t>(2,0);
    m.at<double_t>(idx + 2,1) = r.at<double_t>(2,1);
    m.at<double_t>(idx + 2,2) = r.at<double_t>(2,2);
    
    // special values depending on idx
    
    for (int i = 0; i < points.size(); i ++) {
      
      if (i == idy) {
        m.at<double_t>(idx,3 + i) = -results[0].x;
        m.at<double_t>(idx+1,3 + i) = -results[0].y;
        m.at<double_t>(idx+2,3 + i) = -1.0;
      }
      else {
        m.at<double_t>(idx,3+ i) = 0;
        m.at<double_t>(idx+1,3+ i) = 0;
        m.at<double_t>(idx+2,3+ i) = 0;
      }
    }
    
    c.at<double_t>(idx,0) = -in.T.at<double_t>(0,0);
    c.at<double_t>(idx+1,0) = -in.T.at<double_t>(1,0);
    c.at<double_t>(idx+2,0) = -in.T.at<double_t>(2,0);
  
    idx += 3;
    idy++;

  }
  
  // Should only be one. Now solve and send back 

  solve(m,c,s,DECOMP_QR);
  
  Point3f p(s.at<double_t>(0,0),s.at<double_t>(1,0),s.at<double_t>(2,0));
  
  cerr << "Leeds - Computed depth point: " << p << endl;
  
  return p;
}