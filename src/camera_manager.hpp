/**
* @brief Camera Manager
* @file camera_manager.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 25/05/2012
*
*/


#ifndef __CAMERA_MANAGER_HPP__
#define __CAMERA_MANAGER_HPP__

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <boost/shared_ptr.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/foreach.hpp>

#include "uvc_camera.hpp"
#include "calibrator.hpp"
#include "config.hpp"
#include "utils.hpp"


/*
 * Camera Wrapper - Adds extra functionality to our uvc cameras- distortion correction and similar
 */
 
class LeedsCam {
public:
	LeedsCam(UVCVideo &cam, cv::Size size);
	
	void setParams(CameraParameters cp) {mP = cp;}; // eventually cx!
	
	CameraParameters& getParams() {return mP;};
	
	bool isSecondary() { return mSecondary;};
	bool isRectified() { return mP.mCalibrated;};
		
	cv::Mat& getImage() { return mImage; };
	cv::Mat& getImageRectified() {return mImageRectified; };
	cv::Mat& getResult() {return mResult; };
	void computeNormal();
	GLuint getTexture() {return mTexID; };
	GLuint getRectifiedTexture() {return mRectifiedTexID; };
	cv::Mat& getNormal() {return mPlaneNormal; };
	VBOData& getVBO(){ return mVBONormal; };
	
	void bind();	// Texture bind
	void bindRectified();
	void bindResult();
	void unbind();
	void update();
	void updateTexture();
	void updateResultTexture();
	
protected:

	UVCVideo &mCam;
	CameraParameters mP;
	bool mSecondary;
	cv::Mat mPlaneNormal;	// Normal to the camera plane
	cv::Mat mTransform;		// The computed transform to the world
	cv::Mat mImage;
	cv::Mat mImageRectified;
	cv::Mat mResult;
	GLuint mTexID;
	GLuint mRectifiedTexID;
	GLuint mTexResultID;
	VBOData mVBONormal;

};
 

/*
 * A shared object class that manages the cameras in the system, providing endpoints
 * buffers and the control of calibrators
 */
 
 ///\todo thread the entire thing or just certain functions?

class CameraManager {
public:

	CameraManager() {};
	void setup(GlobalConfig &config);
	void update();
	void updateTextures();
	void updateResults();
	
	boost::shared_ptr<LeedsCam> addCamera(std::string dev, std::string filename);
	void calibrateCameras();
	void calibrateWorld();
	void setControl(CameraControl c, unsigned int v);
	
	cv::Point3f solveForAll(std::vector< std::pair<cv::Point2f, CameraParameters > > points);
	
	bool isThreading() {return sThreads > 0;};
	
	std::vector<boost::shared_ptr<LeedsCam> >& getCams() { return mObj->mCams; };
	
	bool detectPoint(cv::Mat &data, cv::Mat &result, cv::Point2f &point);
	
	cv::Mat& getResult() { return mObj->mResult; };
	
	void saveSettings(std::vector<std::string> filenames);
	void shutdown();
	
	int waitingOn() {return mObj->mWaitingOn; };
	
	void bind();
	void unbind();

protected:

	///\todo we need some sort of internal state and thread pool here -  time to go and what not

	void _calibrateCameras(); 	// Threaded
	void _calibrateWorld(); 	// Threaded

	static uint8_t sThreads;	// Counts the threads this has launched. Passed to each thread
	
	

	struct SharedObj {
	
		SharedObj(GlobalConfig &config) : mConfig(config) {};

		GlobalConfig &mConfig;
		
		std::vector<boost::shared_ptr<LeedsCam> > mCams;
		std::vector<boost::shared_ptr<UVCVideo> > mDevs;
		
		cv::Mat mResult; // results of any processing
		
		boost::thread *pWorkerThread;
		
		int mWaitingOn; ///\todo remove eventually as this is related to state! :S
		
		GLuint mTexID;	
	
	};

	boost::shared_ptr<SharedObj> mObj;
	
};



#endif

