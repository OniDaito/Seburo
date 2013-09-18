/**
* @brief Camera Manager that deals with the control of the 8 cameras
* @file camera_manager.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/05/2012
*
*/

#include "s9/gl/video.hpp"

using namespace std;
#ifdef _SEBURO_OPENCV
using namespace cv;
#endif

using namespace boost; 
using namespace s9;
using namespace s9::gl;

#ifdef _SEBURO_OPENCV
using namespace s9::gl::compvis;
#endif

VidCam::VidCam(std::string dev, size_t w, size_t h, size_t fps) {
	_obj.reset(new SharedObj());
	_obj->_texture = TextureStream(glm::vec2(w,h),TEXTURE_RGB);

#ifdef _SEBURO_LINUX
	_obj->_cam.reset(new UVCVideo());
	_obj->_cam->startCapture(dev,w,h,fps);
#endif

	_obj->_texture.setData(_obj->_cam->getBuffer());
	_obj->_texture.start();

	_obj->_fps = fps; 

	CXGLERROR
}

void VidCam::update() {
	_obj->_texture.update();
	//_obj->_texture.update(_obj->_cam->getBuffer());
}

void VidCam::stop(){
#ifdef _SEBURO_LINUX
	_obj->_cam->stop();
#endif	
	_obj->_texture.stop();
}

void VidCam::setControl(unsigned int id, int value) {
#ifdef _SEBURO_LINUX
	_obj->_cam->set_control(id,value);
#endif	
}


#ifdef _SEBURO_OPENCV

/*
 * CVCamera knows its place in the world and is undistorted we hope
 */


CVVidCam::CVVidCam(){}

CVVidCam::CVVidCam(VidCam &cam){
	
	_obj.reset(new SharedObj(cam));

	cv::Size size (_obj->mCam.getSize().x,  _obj->mCam.getSize().y);
	_obj->mImage = Mat(size, CV_8UC3);
	_obj->mImageRectified = Mat(size, CV_8UC3);
	glGenTextures(1, &(_obj->mRectifiedTexID));
	glBindTexture(GL_TEXTURE_RECTANGLE, _obj->mRectifiedTexID);               
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 3,  _obj->mCam.getSize().x,  _obj->mCam.getSize().y,
	 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *) IplImage(_obj->mImageRectified).imageData);

	_obj->mPlaneNormal = Mat(Size(1,3), CV_64FC1);
	_obj->mPlaneNormal.at<double_t>(0,0) = 0.0;
	_obj->mPlaneNormal.at<double_t>(1,0) = 0.0;	
	_obj->mPlaneNormal.at<double_t>(2,0) = 1.0;
	

}


void CVVidCam::computeNormal() {
	
	Mat r (Size(3,3), CV_64FC1);
	Rodrigues(_obj->mP.R,r);

	r = r.inv();
	r *= -1;
	_obj->mPlaneNormal = Mat(Size(1,3), CV_64FC1);
	_obj->mPlaneNormal.at<double_t>(0,0) = 0.0;
	_obj->mPlaneNormal.at<double_t>(1,0) = 0.0;	
	_obj->mPlaneNormal.at<double_t>(2,0) = 1.0;
	_obj->mPlaneNormal = r * _obj->mPlaneNormal;
	
}
		
	
void CVVidCam::bindRectified(){ glBindTexture(GL_TEXTURE_RECTANGLE, _obj->mRectifiedTexID); }
	
	
void CVVidCam::update(){
	_obj->mCam.update();
	
	_obj->mImage = cv::Mat (_obj->mImage.size(), CV_8UC3, _obj->mCam.getBuffer());
	
	if (_obj->mP.mCalibrated){
		undistort(_obj->mImage, _obj->mImageRectified, _obj->mP.M,_obj->mP.D);
		
		bindRectified();
		glTexSubImage2D(GL_TEXTURE_RECTANGLE,0,0,0, _obj->mImageRectified.size().width, 
			_obj->mImageRectified.size().height, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *) IplImage(_obj->mImageRectified).imageData );
		unbind();
		
	}	
}

void CVVidCam::bind(){
	_obj->mCam.bind();
}

void CVVidCam::unbind(){
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
}




/*
 * Load Intrinsic Parameters from disk
 */


bool CVVidCam::loadParameters(string filename) {
	try {
		FileStorage fs(filename.c_str(), CV_STORAGE_READ);
		if(!fs.isOpened()) {
			cout << "SEBURO - Failed to open intrinsic file "  << filename << endl;
			return false;
		}

		fs["M"] >> _obj->mP.M;
		fs["D"] >> _obj->mP.D;

		try {
			fs["R"] >> _obj->mP.R;
			fs["T"] >> _obj->mP.T;
		}
		catch (...) {
			cout << "SEBURO - failed to load world transforms in intrinsics." << endl;
		}
		
		cout << "SEBURO - Loaded camera Parameters " << filename << endl;
		_obj->mP.mCalibrated = true;
		fs.release();
		computeNormal();
		return true;
		
	} catch(...) {
		cout << "SEBURO - failed to load intrinsic variables. You will need to calibrate." << endl;
	}

	return false;
}


/*
 * Save the Intrinsic Parameters to a set of files
 */

bool CVVidCam::saveParameters(string filename) {

    FileStorage fs(filename, CV_STORAGE_WRITE);
    if( fs.isOpened() ) {
        fs << "M" << _obj->mP.M << "D" << _obj->mP.D << "R" << _obj->mP.R << "T" << _obj->mP.T;
        fs.release();
        return true;
    }
    else
        cout << "SEBURO: can not save the intrinsic parameters\n";
    
    return false;
}


#endif
		
