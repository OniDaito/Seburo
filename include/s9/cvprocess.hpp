/**
* @brief A pipeline for OpenCV image processing
* @file cvprocess.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 07/08/2012
*
*/

#ifndef S9_CVPROCESS_HPP
#define S9_CVPROCESS_HPP

#include "common.hpp"
#include <opencv2/opencv.hpp>

namespace s9{

  namespace compvis {


    /*
     * A step in the processing - records a result
     */

    class OPENGLCOURSEAPI ProcessBlock {
    public:

      friend class Process;

      ProcessBlock() {}
      cv::Mat& process(cv::Mat &in);

      template <class T>
      void setValue(std::string tag, T value) {
        std::map<std::string,boost::shared_ptr<void> >::iterator it = _obj->_values.find(tag);
        if(it != _obj->_values.end()){
          boost::shared_ptr<T> tp = boost::static_pointer_cast<T>(_obj->_values[tag]);
          *tp = value;
        }
        else
          _obj->_values[tag] = boost::shared_ptr<T>(new T(value));
      }

      cv::Mat& getResult() { return _obj->_result; };

      ///\todo wrong key should not fall over but warn
      template <class T>
      T getValue(std::string tag) {      
        boost::shared_ptr<void> tp = _obj->_values[tag];
        return *(boost::static_pointer_cast<T>( tp ));
      }

    protected:
      virtual void _init() {_obj.reset(new SharedObj);  std::cout << "INIT" << std::endl; } ;

      virtual cv::Mat& _process(cv::Mat &in) { return _obj->_result; };

      struct SharedObj {
        cv::Mat _result;
        std::map< std::string, boost::shared_ptr<void> > _values;
      };

      boost::shared_ptr<SharedObj> _obj;

    };

    /*
     * A Pipeline of image processing blocks
     */

    class OPENGLCOURSEAPI Process {
    public:
      Process() { };

      virtual operator int() const { return _obj->_processed; };


      cv::Mat process(cv::Mat &in);

      cv::Mat& getResult() { return _obj->_blocks.back()->getResult(); };
    
      template<class T>
        Process& addBlock() {

          if (_obj == boost::shared_ptr<SharedObj>()) {
             _obj.reset(new SharedObj());
             _obj->_processed = false;
           }

          _obj->_blocks.push_back( boost::shared_ptr<T>(new T));
          _obj->_blocks.back()->_init();
          return *this;
      }

      void addBlock(ProcessBlock block, int pos);
      void removeBlock(int pos);
      ProcessBlock getBlock(int pos);

    protected:
      struct SharedObj {
        std::vector<boost::shared_ptr<ProcessBlock> > _blocks;
        bool _processed;
      };

      boost::shared_ptr<SharedObj> _obj;

    };

    // Useful Blocks

    class OPENGLCOURSEAPI BlockGreyscale : public ProcessBlock {
  
    protected:
        cv::Mat& _process(cv::Mat &in);
    };

  
    class BlockThreshold : public ProcessBlock {
    public:
      void setLower(int val);
      void setUpper(int val);

    protected:
      cv::Mat& _process(cv::Mat &in);
      void _init();
    };
  
    class BlockDetectPoint : public ProcessBlock {

    protected:
      cv::Mat& _process(cv::Mat &in);
      void _init();
    };

  
    class BlockContours : public ProcessBlock {

    protected:
      cv::Mat& _process(cv::Mat &in);
      void _init();
    };

    class BlockSmooth : public ProcessBlock {

    protected:
      cv::Mat& _process(cv::Mat &in);
      void _init();
    };

    class BlockBackground : public ProcessBlock {

    protected:
      cv::Mat& _process(cv::Mat &in);
      void _init();

      cv::BackgroundSubtractorMOG _subtractor;
    };

    class BlockHighPass : public ProcessBlock {

    protected:
      cv::Mat& _process(cv::Mat &in);
      void _init();
    };

  }

  // Useful functions
  
  /*
   * Find the Chessboard
   */

  inline bool findChessboard(cv::Mat &cam0, std::vector<cv::Point2f> &corners, cv::Mat &board, cv::Size &size ) {
    if ( cv::findChessboardCorners(cam0, size, corners, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK) ) {
    
      cam0.copyTo(board);
      cv::drawChessboardCorners(board, size, corners, true);
      
      cv::Mat grey = cam0;

      cv::cvtColor( cam0, grey, CV_RGB2GRAY);   
      cv::cornerSubPix(grey, corners, cv::Size(11,11), cv::Size(-1,-1), 
        cv::TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 30, 0.01));

      return true;
    }
    cam0.copyTo(board); 
    return false;
  }


}


#endif