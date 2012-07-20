/**
* @brief Video Manager
* @file video.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 11/07/2012
*
*/


#ifndef S9VIDEO_HPP
#define S9VIDEO_HPP


#include "common.hpp"
#include "uvc_camera.hpp"

#ifdef _GEAR_OPENCV
#include <opencv2/opencv.hpp>
#endif

#ifdef _GEAR_X11_GLX
#include "uvc_camera.hpp"
#endif


namespace s9 {

	/*
	 * Access to a camera device - OS dependent
	 */

	class S9VidCam {
	public:
		S9VidCam();
		bool open(std::string dev, size_t w, size_t h, size_t fps);
		void stop();
		glm::vec2 getSize() {return glm::vec2(mObj->mW, mObj->mH);};
		GLuint getTexture() {return mObj->mTexID; };
		
		void bind();	// Texture bind
		void unbind();
		void update();
		unsigned char* getBuffer() {return mObj->pCam->getBuffer(); };
	protected:
		class SharedObj {
		public:
#ifdef _GEAR_X11_GLX
			boost::shared_ptr<UVCVideo> pCam;
#endif
			size_t mW,mH,mFPS;
			GLuint mTexID;

		};
		
		boost::shared_ptr<SharedObj> mObj;
		
	};


#ifdef _GEAR_OPENCV

	/*
	 * OpenCV Based Camera Parameters
	 */

	class CameraParameters {
	public:
		CameraParameters() { M = cv::Mat::eye(3, 3, CV_64F); mCalibrated = false;};
		cv::Mat M, D, R, T; 		// Camera Matrix, Distortion co-efficients, rotation and translation matrices
		std::vector<cv::Mat> Rs;	// Rotations for each view
		std::vector<cv::Mat> Ts;	// Translations for each view
		bool mCalibrated;			// Is this calibrated and distortion free?
	};

	/*
	 * OpenCV style camera with correction - wraps the other camera
	 * \todo decorator pattern?
	 * \todo set as a template so we can pass through to underlying?
	 */

	class CVVidCam {
	public:
		CVVidCam();
		CVVidCam(S9VidCam &cam);
			
		CameraParameters& getParams() {return mObj->mP;};
		
		bool loadParameters(std::string filename);
		bool saveParameters(std::string filename);
		
		bool isSecondary() { return  mObj->mSecondary;};
		bool isRectified() { return  mObj->mP.mCalibrated;};
			
		cv::Mat& getImage() { return  mObj->mImage; };
		cv::Mat& getImageRectified() {return  mObj->mImageRectified; };
		cv::Mat& getResult() {return  mObj->mResult; };
		glm::vec2 getSize() {return mObj->mCam.getSize(); };
		void computeNormal();
		
		GLuint getRectifiedTexture() {return  mObj->mRectifiedTexID; };
		cv::Mat& getNormal() {return  mObj->mPlaneNormal; };
		
		void bind();
		void bindRectified();
		void bindResult();
		void unbind();
	
		void update();
		
	protected:

		class SharedObj {
		public:

			SharedObj(S9VidCam cam) {mCam = cam; };
			CameraParameters mP;
			bool mSecondary;
			cv::Mat mPlaneNormal;	// Normal to the camera plane
			cv::Mat mTransform;		// The computed transform to the world
			cv::Mat mImage;
			cv::Mat mImageRectified;
			cv::Mat mResult;
				
			GLuint mRectifiedTexID;
			GLuint mTexResultID;
			S9VidCam mCam;

		};

		boost::shared_ptr<SharedObj> mObj;

	};
#endif

}



#endif

