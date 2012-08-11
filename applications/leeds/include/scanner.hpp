/*
* @brief Scanner to recreate our mesh from lasers
* @file scanner.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef LEEDS_SCANNER_HPP
#define LEEDS_SCANNER_HPP

#include "s9/common.hpp"

#include "s9/cvprocess.hpp"
#include "s9/gl/video.hpp"
 
namespace leeds {
    
    /*
     * Detect Laser Light and Scan for us
     */

    class Scanner {
    public:
      Scanner();
      bool detectPoint(s9::gl::compvis::CVVidCam cam, cv::Point2f &point, cv::Mat &result);
      cv::Point3f solveForAll( std::vector< std::pair< cv::Point2f, s9::gl::compvis::CameraParameters > > points);


    protected:
      s9::compvis::Process _detect_proc;
    };


    class Calibrator {

    };

    class Mesher {

    };

}


#endif