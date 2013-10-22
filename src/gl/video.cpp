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

using namespace s9;
using namespace s9::gl;

#ifdef _SEBURO_OPENCV
using namespace s9::gl::compvis;
#endif

VidCam::VidCam(std::string dev, size_t w, size_t h, size_t fps) : obj_( shared_ptr<SharedObj> (new SharedObj())) {

	obj_->texture_ = TextureStream(w,h,RGB);

#ifdef _SEBURO_LINUX
	obj_->cam_.reset(new UVCVideo());
	obj_->cam_->startCapture(dev,w,h,fps);
#endif

	obj_->texture_.set_tex_data(obj_->cam_->getBuffer());
	obj_->texture_.start();

	obj_->_fps = fps; 

	CXGLERROR
}

void VidCam::update() {
	obj_->texture_.update();
	//obj_->texture_.update(obj_->cam_->getBuffer());
}

void VidCam::stop(){
#ifdef _SEBURO_LINUX
	obj_->cam_->stop();
#endif	
	obj_->texture_.stop();
}

void VidCam::setControl(unsigned int id, int value) {
#ifdef _SEBURO_LINUX
	obj_->cam_->set_control(id,value);
#endif	
}


#ifdef _SEBURO_OPENCV

/*
 * CVCamera knows its place in the world and is undistorted we hope
 */


CVVidCam::CVVidCam(){}

CVVidCam::CVVidCam(VidCam &cam) : obj_( shared_ptr<SharedObj> (new SharedObj(cam))){
	
	cv::Size size (obj_->mCam.getSize().x,  obj_->mCam.getSize().y);
	obj_->mImage = Mat(size, CV_8UC3);
	obj_->mImageRectified = Mat(size, CV_8UC3);
	glGenTextures(1, &(obj_->mRectifiedTexID));
	glBindTexture(GL_TEXTURE_RECTANGLE, obj_->mRectifiedTexID);               
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 3,  obj_->mCam.getSize().x,  obj_->mCam.getSize().y,
	 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *) IplImage(obj_->mImageRectified).imageData);

	obj_->mPlaneNormal = Mat(Size(1,3), CV_64FC1);
	obj_->mPlaneNormal.at<double_t>(0,0) = 0.0;
	obj_->mPlaneNormal.at<double_t>(1,0) = 0.0;	
	obj_->mPlaneNormal.at<double_t>(2,0) = 1.0;
	

}


void CVVidCam::computeNormal() {
	
	Mat r (Size(3,3), CV_64FC1);
	Rodrigues(obj_->mP.R,r);

	r = r.inv();
	r *= -1;
	obj_->mPlaneNormal = Mat(Size(1,3), CV_64FC1);
	obj_->mPlaneNormal.at<double_t>(0,0) = 0.0;
	obj_->mPlaneNormal.at<double_t>(1,0) = 0.0;	
	obj_->mPlaneNormal.at<double_t>(2,0) = 1.0;
	obj_->mPlaneNormal = r * obj_->mPlaneNormal;
	
}
		
	
void CVVidCam::bindRectified(){ glBindTexture(GL_TEXTURE_RECTANGLE, obj_->mRectifiedTexID); }
	
	
void CVVidCam::update(){
	obj_->mCam.update();
	
	obj_->mImage = cv::Mat (obj_->mImage.size(), CV_8UC3, obj_->mCam.getBuffer());
	
	if (obj_->mP.mCalibrated){
		undistort(obj_->mImage, obj_->mImageRectified, obj_->mP.M,obj_->mP.D);
		
		bindRectified();
		glTexSubImage2D(GL_TEXTURE_RECTANGLE,0,0,0, obj_->mImageRectified.size().width, 
			obj_->mImageRectified.size().height, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *) IplImage(obj_->mImageRectified).imageData );
		unbind();
		
	}	
}

void CVVidCam::bind(){
	obj_->mCam.bind();
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

		fs["M"] >> obj_->mP.M;
		fs["D"] >> obj_->mP.D;

		try {
			fs["R"] >> obj_->mP.R;
			fs["T"] >> obj_->mP.T;
		}
		catch (...) {
			cout << "SEBURO - failed to load world transforms in intrinsics." << endl;
		}
		
		cout << "SEBURO - Loaded camera Parameters " << filename << endl;
		obj_->mP.mCalibrated = true;
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
        fs << "M" << obj_->mP.M << "D" << obj_->mP.D << "R" << obj_->mP.R << "T" << obj_->mP.T;
        fs.release();
        return true;
    }
    else
        cout << "SEBURO: can not save the intrinsic parameters\n";
    
    return false;
}


#endif
		
