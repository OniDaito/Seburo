/**
* @brief video solution for OpenGL
* @file video.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 11/07/2012
*
*/

#ifndef S9_VIDEO_HPP
#define S9_VIDEO_HPP

#include "../common.hpp"
#include "common.hpp"
#include "texture.hpp"
#include "utils.hpp"

#ifdef _SEBURO_OPENCV
#include <opencv2/opencv.hpp>
#endif

#ifdef _SEBURO_LINUX
#include "s9/linux/uvc_camera.hpp"
#endif

#ifdef _SEBURO_OSX
#include "s9/osx/quicktime_camera.hpp"
#endif

#ifdef _SEBURO_WIN32
#include "s9/win32/windows_camera.hpp"
#endif


namespace s9 {

	namespace gl {

		/*
		 * Access to a camera device - OS dependent and using OpenGL as the texture method
		 */

		class SEBUROAPI VidCam {
		public:
			VidCam() {};
			VidCam (std::string dev, size_t w, size_t h, size_t fps);
			void Stop();		
			void SetControl(unsigned int id, int value);
			void Update();
			unsigned char* GetBuffer() {return obj_->cam->GetBuffer(); };

			void Bind() {obj_->texture.Bind(); };
			void Unbind() {obj_->texture.Unbind(); };

			TextureStream texture() {return obj_->texture; };

			virtual operator int() const { return obj_.use_count() > 0; };
			
		protected:

			// Previously, I extended the texture shared obj
			// but not now.

			struct SharedObject {
			public:

#ifdef _SEBURO_LINUX
				std::shared_ptr<UVCVideo> cam;
#endif

#ifdef _SEBURO_OSX
				std::shared_ptr<QuicktimeCamera> cam;
#endif

#ifdef _SEBURO_WIN32
				std::shared_ptr<WindowsCamera> cam;
#endif

				size_t _fps;
				TextureStream texture;
			};
			
			std::shared_ptr<SharedObject> obj_;
			
		};


///\todo need to rethink the OpenCV stuff

#ifdef _SEBURO_OPENCV
		namespace compvis{

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
				CVVidCam(VidCam &cam);
					
				CameraParameters& getParams() {return obj_->mP;};
				
				bool loadParameters(std::string filename);
				bool saveParameters(std::string filename);
				
				bool isSecondary() { return  obj_->mSecondary;};
				bool isRectified() { return  obj_->mP.mCalibrated;};
					
				cv::Mat& getImage() { return  obj_->mImage; };
				cv::Mat& getImageRectified() {return  obj_->mImageRectified; };
				glm::vec2 getSize() {return obj_->mCam.getSize(); };
				void computeNormal();
				
				GLuint getRectifiedTexture() {return  obj_->mRectifiedTexID; };
				cv::Mat& getNormal() {return  obj_->mPlaneNormal; };
				
				void bind();
				void bindRectified();
				void bindResult();
				void unbind();
				
				void update();
				
			protected:

				class SharedObj {
				public:

					SharedObj(VidCam cam) {mCam = cam; };
					CameraParameters mP;
					bool mSecondary;
					cv::Mat mPlaneNormal;	// Normal to the camera plane
					cv::Mat mTransform;		// The computed transform to the world
					cv::Mat mImage;
					cv::Mat mImageRectified;
						
					GLuint mRectifiedTexID;
					VidCam mCam;

				};

				std::shared_ptr<SharedObj> obj_;

			};
		}
#endif
	}

}



#endif

