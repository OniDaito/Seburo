/**
* @brief Camera Manager that deals with the control of the 8 cameras
* @file camera_manager.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/05/2012
*
*/


#ifdef _GEAR_OPENCV

#include "s9/cvprocess.hpp"

using namespace std;
using namespace cv;
using namespace boost; 
using namespace s9;
using namespace s9::compvis;

/*
 * Perform the processing and copying of the matrix
 */

cv::Mat& ProcessBlock::process(cv::Mat &in) {
  _obj->_result = Mat(in);
  return _process(_obj->_result);
}




/*
 * Perform the routine
 */

cv::Mat Process::process(cv::Mat &in){
  Mat result(in);
  for (int i = 0; i < _obj->_blocks.size(); ++i){
    result = _obj->_blocks[i]->process(result);
  }
  return result;
}

/*
 * Add a block to a specific position
 */

    
void Process::addBlock(ProcessBlock block, int pos) {

}

/*
 * Remove a block from a specific position
 */

void Process::removeBlock(int pos){

}

/*
 * Return a block
 */

ProcessBlock Process::getBlock(int pos){
  return *(_obj->_blocks[pos]);
}

/*
 * Process an image, turning it grey
 */


cv::Mat&  BlockGreyscale::_process(cv::Mat &in){
  _obj->_result = Mat(in.size(), CV_8UC1);
  cvtColor( in, _obj->_result, CV_RGB2GRAY );
  return _obj->_result;
}

/*
 * Binary Thresholding process
 */

cv::Mat&  BlockThreshold::_process(cv::Mat &in){
  _obj->_result = Mat(in.size(), CV_8UC1);
  threshold(in, _obj->_result, 
    getValue<int>("bottom"),  
    getValue<int>("top"), 
    THRESH_BINARY);
  
}

/*
 * Set the lower limits of the threshold
 */

void BlockThreshold::setLower(int val){
  setValue<int>("bottom",val);
}

/*
 * Set the upper limit of the threshold
 */

void BlockThreshold::setUpper(int val){
  setValue<int>("top",val);
}

/*
 * Set threshold initial variables
 */

void BlockThreshold::_init(){
  _obj.reset(new SharedObj());
  setLower(10);
  setUpper(250);
}


cv::Mat& BlockDetectPoint::_process(cv::Mat &in){

  _obj->_result = Mat(in.size(), CV_8UC3);

  for (int i=0; i < in.rows; i++){
  
    for (int j=0; j < in.cols; j++){
      if (in.ptr<uint8_t>(i)[j] >= 220){
        float score = in.ptr<uint8_t>(i)[j];
        // We have a local area so search it  
        for (int k =-3; k <4; k ++){
          for (int l = -3; l < 4; l++){
            score += in.ptr<uint8_t>(i + k)[j + l];
          }
        }
        
        if ( score / 49.0 > 50) {
          setValue<bool>("found",true);
          cv::Point2f point = getValue<cv::Point2f>("point");

          circle(_obj->_result, point, 20, Scalar(255,0,0),2);
          return _obj->_result;
        }     
      }
    }
    return _obj->_result;
  }
}

void BlockDetectPoint::_init(){
  _obj.reset(new SharedObj());
  cout << "INIT DETECT" << endl;
  setValue<bool>("found",false);
  _obj->_values["point"] = shared_ptr<Point2f>(new Point2f(0,0));
}


#endif