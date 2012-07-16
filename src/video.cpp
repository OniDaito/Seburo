/**
* @brief Camera Manager that deals with the control of the 8 cameras
* @file camera_manager.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/05/2012
*
*/

#include "video.hpp"

using namespace std;
#ifdef _GEAR_OPENCV
using namespace cv;
#endif
using namespace boost; 
using namespace s9;

S9VidCam::S9VidCam() {}

bool S9VidCam::open(std::string dev, size_t w, size_t h, size_t fps) {
	mObj.reset(new SharedObj());

#ifdef _GEAR_X11_GLX
	mObj->pCam.reset(new UVCVideo());
	mObj->pCam->startCapture(dev,w,h,fps);
#endif
	
	mObj->mW = w; mObj->mH = h; mObj->mFPS = fps; 

	// Initialise texture - using GL_TEXTURE_RECTANGLE
	glGenTextures(1, &(mObj->mTexID));
	glBindTexture(GL_TEXTURE_RECTANGLE, mObj->mTexID);              
	glTexParameterf(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, mObj->pCam->getBuffer());
	
}


void S9VidCam::bind(){
	glBindTexture(GL_TEXTURE_RECTANGLE, mObj->mTexID);
}

void S9VidCam::unbind(){
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
}


void S9VidCam::update() {
	bind();
	glTexSubImage2D(GL_TEXTURE_RECTANGLE,0,0,0,mObj->mW, mObj->mH, GL_RGB, GL_UNSIGNED_BYTE, mObj->pCam->getBuffer() );		
	unbind();
}

void S9VidCam::stop(){
#ifdef _GEAR_X11_GLX
	mObj->pCam->stop();
#endif	
	
}

#ifdef _GEAR_OPENCV

/*
 * CVCamera knows its place in the world and is undistorted we hope
 */


CVVidCam::CVVidCam(){}

CVVidCam::CVVidCam(S9VidCam &cam){
	
	mObj.reset(new SharedObj(cam));

	cv::Size size (mObj->mCam.getSize().x,  mObj->mCam.getSize().y);
	mObj->mImage = Mat(size, CV_8UC3);
	mObj->mImageRectified = Mat(size, CV_8UC3);
	mObj->mResult = Mat(size,CV_8UC3);
	glGenTextures(1, &(mObj->mTexResultID));
	glBindTexture(GL_TEXTURE_RECTANGLE, mObj->mTexResultID);
	glTexParameterf(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 3, mObj->mCam.getSize().x,  mObj->mCam.getSize().y,
		0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *) IplImage(mObj->mResult).imageData);
	glGenTextures(1, &(mObj->mRectifiedTexID));
	glBindTexture(GL_TEXTURE_RECTANGLE, mObj->mRectifiedTexID);               
	glTexParameterf(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 3,  mObj->mCam.getSize().x,  mObj->mCam.getSize().y,
	 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *) IplImage(mObj->mImageRectified).imageData);

	mObj->mPlaneNormal = Mat(Size(1,3), CV_64FC1);
	mObj->mPlaneNormal.at<double_t>(0,0) = 0.0;
	mObj->mPlaneNormal.at<double_t>(1,0) = 0.0;	
	mObj->mPlaneNormal.at<double_t>(2,0) = 1.0;
	

}


void CVVidCam::computeNormal() {
	
	Mat r (Size(3,3), CV_64FC1);
	Rodrigues(mObj->mP.R,r);

	r = r.inv();
	r *= -1;
	mObj->mPlaneNormal = Mat(Size(1,3), CV_64FC1);
	mObj->mPlaneNormal.at<double_t>(0,0) = 0.0;
	mObj->mPlaneNormal.at<double_t>(1,0) = 0.0;	
	mObj->mPlaneNormal.at<double_t>(2,0) = 1.0;
	mObj->mPlaneNormal = r * mObj->mPlaneNormal;
	
}
		
	
void CVVidCam::bindRectified(){ glBindTexture(GL_TEXTURE_RECTANGLE, mObj->mRectifiedTexID); }
void CVVidCam::bindResult(){ glBindTexture(GL_TEXTURE_RECTANGLE, mObj->mTexResultID); }
	
	
void CVVidCam::update(){
	mObj->mCam.update();
	
	mObj->mImage = cv::Mat (mObj->mImage.size(), CV_8UC3, mObj->mCam.getBuffer());
	
	if (mObj->mP.mCalibrated){
		undistort(mObj->mImage, mObj->mImageRectified, mObj->mP.M,mObj->mP.D);
		
		bindRectified();
		glTexSubImage2D(GL_TEXTURE_RECTANGLE,0,0,0, mObj->mImageRectified.size().width, 
			mObj->mImageRectified.size().height, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *) IplImage(mObj->mImageRectified).imageData );
		unbind();
		
		bindResult();
		glTexSubImage2D(GL_TEXTURE_RECTANGLE,0,0,0, mObj->mResult.size().width, 
			mObj->mResult.size().height, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *) IplImage(mObj->mResult).imageData );
		unbind();
	}	
}

void CVVidCam::bind(){
	mObj->mCam.bind();
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
			cout << "S9Gear - Failed to open intrinsic file "  << filename << endl;
			return false;
		}

		fs["M"] >> mObj->mP.M;
		fs["D"] >> mObj->mP.D;

		try {
			fs["R"] >> mObj->mP.R;
			fs["T"] >> mObj->mP.T;
		}
		catch (...) {
			cout << "S9Gear - failed to load world transforms in intrinsics." << endl;
		}
		
		cout << "S9Gear - Loaded camera Parameters " << filename << endl;
		mObj->mP.mCalibrated = true;
		fs.release();
		computeNormal();
		return true;
		
	} catch(...) {
		cout << "S9Gear - failed to load intrinsic variables. You will need to calibrate." << endl;
	}

	return false;
}


/*
 * Save the Intrinsic Parameters to a set of files
 */

bool CVVidCam::saveParameters(string filename) {

    FileStorage fs(filename, CV_STORAGE_WRITE);
    if( fs.isOpened() ) {
        fs << "M" << mObj->mP.M << "D" << mObj->mP.D << "R" << mObj->mP.R << "T" << mObj->mP.T;
        fs.release();
        return true;
    }
    else
        cout << "S9Gear: can not save the intrinsic parameters\n";
    
    return false;
}


#endif
		
