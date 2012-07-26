/**
* @brief Camera Classes
* @file camera.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/


#ifndef S9_CAMERA_HPP
#define S9_CAMERA_HPP

#include "common.hpp"
#include "primitive.hpp"
#include "events.hpp"

/* 
 * Camera Class for a camera that pitches and yaws whilst keeping up always parallel with y
 * \todo certain camera classes could listen for events but we need to set a precidence (as sometimes we want
 * \todo  * Camera can extend primitve! *
 * camera movement and othertimes we want selection for example) - bubbling
 */
 
namespace s9{
	 
	class Camera {

	public:

		Camera();
		
		virtual void yaw(float_t a);
		virtual void pitch(float_t a);
		virtual void roll(float_t a);
		
		virtual void align(Primitive &p);
		
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
		void shift(glm::vec2 s);
		void align(Primitive &p){};
		
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
		void align(Primitive &p){};
		
	protected:
		void compute();
		size_t mW, mH;
	};

	/*
	 * A template that adds inertia and mouse drag movements
	 * \todo make implicit in the visual app somehow?
	 * \todo zooming
	 */

	template <class T>
	class InertiaCam : public T {

	protected:
		glm::vec3 mNow;
		glm::vec3 mPrev;
		float_t mAngle, mDegrade;
		glm::vec3 mP;
		double_t mDT;
		bool mHeld;

	public:

		InertiaCam() { 
			mNow = glm::vec3(0.0f,0.0f,0.0f); 
		 	mAngle = 50.0;
			mDegrade = 2.0;
			mDT = 0;
			mHeld = false;

		}

		void update(double_t dt){ }

		void passEvent(MouseEvent &e){

			if (e.mFlag & MOUSE_LEFT_DOWN){

				if (glm::length(mP) > 0){

					mNow = glm::vec3(static_cast<float_t>(e.mX - mP.x), static_cast<float_t>(e.mY - mP.y),0.0f);
					mHeld = true;
				}
			
				mP = glm::vec3(static_cast<float_t>(e.mX), static_cast<float_t>(e.mY),0.0f);

			}
			else if (e.mFlag & MOUSE_LEFT_UP ){
				mP = glm::vec3(0.0f,0.0f,0.0f);
				mHeld = false;
			}
		}
	};


	template <>	
	inline void InertiaCam<OrbitCamera>::update(double_t dt){
		
		double_t ds = mAngle * dt;

		if (glm::length(mNow) < 0.01){
			 mNow = glm::vec3(0.0f,0.0f,0.0f);
		}	
		else {
			this->yaw(mNow.x * ds);
			this->pitch(mNow.y * ds);
		}

		if (!mHeld) {
			mNow *= 1.0 - (dt * mDegrade);
		}
	}

 }
	 

#endif

