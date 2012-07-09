/**
* @brief Camera Classes
* @file camera.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/


#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "common.hpp"

/* 
 * Camera Class for a camera that pitches and yaws whilst keeping up always parallel with y
 */
 
namespace s9{
	 
	class Camera {

	public:

		Camera();
		
		virtual void yaw(float_t a);
		virtual void pitch(float_t a);
		virtual void roll(float_t a);
		
		void setNear(float_t n) {mNear = n; compute(); };
		void setFar(float_t n) {mFar = n; compute(); };
		
		void setRatio(float_t r);
		
		void setPos(glm::vec3 p) {mPos = p; compute(); };
		void setLook(glm::vec3 l);
		
		virtual void move(glm::vec3 m);
		
		glm::vec3 getPos() {return mPos; };
		glm::vec3 getLook() {return mLook; };
		glm::vec3 getUp() {return mUp; };
		
		glm::mat4 getViewMatrix() { return mViewMatrix; };
		glm::mat4 getProjMatrix() { return mProjectionMatrix; };
		glm::mat4 getMatrix() {return mProjectionMatrix * mViewMatrix; };

	protected:
		virtual void compute();

		
		glm::mat4 mViewMatrix;
		glm::mat4 mProjectionMatrix;
		
		glm::vec3 mPos;
		glm::vec3 mLook;
		glm::vec3 mUp;
		
		float_t mR,mFar,mNear, mField;
		
	};

	/* 
	 * Orbit camera revolves around one point as it were
	 */
	 
	class OrbitCamera : public Camera{
	public:
		OrbitCamera();
		void zoom(float_t z);
		void shift(float_t du, float_t dv);
		
		void yaw(float_t a);
		void pitch(float_t a);
		void roll(float_t a);

	protected:
		void compute();
		
	};
	
	/*
	 * Screen Camera
	 */
	 
	class ScreenCamera : public Camera {
	public:
		ScreenCamera() {mW = 800; mH = 600; compute(); };
		void setDim(size_t w, size_t h) {mW = w; mH = h; compute(); };
		
	protected:
		void compute();
		size_t mW, mH;
	};

 }
	 

#endif

