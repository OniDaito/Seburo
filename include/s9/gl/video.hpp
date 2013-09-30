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
			void stop();		
			void setControl(unsigned int id, int value);
			void update();
			unsigned char* getBuffer() {return _obj->_cam->getBuffer(); };

			void bind() {_obj->_texture.bind(); };
			void unbind() {_obj->_texture.unbind(); };

			TextureStream getTexture() {return _obj->_texture; };

			glm::vec2 size() {return _obj->_texture.size(); }
			glm::vec2 getSize() {return size(); }

			virtual operator int() const { return _obj.use_count() > 0; };
			
		protected:

			// Previously, I extended the texture shared obj
			// but not now.

			struct SharedObj {
			public:

#ifdef _SEBURO_LINUX
				std::shared_ptr<UVCVideo> _cam;
#endif

#ifdef _SEBURO_OSX
				std::shared_ptr<QuicktimeCamera> _cam;
#endif

#ifdef _SEBURO_WIN32
				std::shared_ptr<WindowsCamera> _cam;
#endif

				size_t _fps;
				TextureStream _texture;
			};
			
			std::shared_ptr<SharedObj> _obj;
			
		};


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
					
				CameraParameters& getParams() {return _obj->mP;};
				
				bool loadParameters(std::string filename);
				bool saveParameters(std::string filename);
				
				bool isSecondary() { return  _obj->mSecondary;};
				bool isRectified() { return  _obj->mP.mCalibrated;};
					
				cv::Mat& getImage() { return  _obj->mImage; };
				cv::Mat& getImageRectified() {return  _obj->mImageRectified; };
				glm::vec2 getSize() {return _obj->mCam.getSize(); };
				void computeNormal();
				
				GLuint getRectifiedTexture() {return  _obj->mRectifiedTexID; };
				cv::Mat& getNormal() {return  _obj->mPlaneNormal; };
				
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

				std::shared_ptr<SharedObj> _obj;

			};
		}
#endif
	}

}



#endif

