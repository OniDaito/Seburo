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
 * \todo update and resize hooks on all cameras
 * \todo create GL Cameras that extend this so we get viewport and such for free
 * camera movement and othertimes we want selection for example) - bubbling
 */
 
namespace s9{
	 
	class OPENGLCOURSEAPI Camera : public Primitive, public WindowResponder{

	public:

		Camera();
		
		virtual void reset();
		
		virtual void align(Primitive &p);

		virtual void processEvent(ResizeEvent e) { 
			setRatio( static_cast<float>(e.mW) / e.mH);
		}

		void setNear(float n) {mNear = n; compute(); };
		void setFar(float n) {mFar = n; compute(); };
		
		void setRatio(float r);
		void setField(float a) {mField = a; compute(); };
		
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

		float mR,mFar,mNear, mField;
		
		
	};

	/* 
	 * Orbit camera revolves around one point as it were
	 */
	 
	class OPENGLCOURSEAPI OrbitCamera : public Camera{
	public:
		OrbitCamera();
		void zoom(float z);
		void shift(glm::vec2 s);
		void align(Primitive &p){};
		
		void yaw(float a);
		void pitch(float a);
		void roll(float a);
		void reset();

		/*
		 * Basic mouse movement for an orbiting camera
		 */

		virtual void processEvent(MouseEvent e) { 
			if(e.mFlag & MOUSE_LEFT_DOWN){
				glm::vec2 mouse_d = glm::vec2(e.mX,e.mY) - _mouse_pre;
				yaw(static_cast<float>(mouse_d.x) * _sense);
				pitch(static_cast<float>(mouse_d.y) * _sense);
			}

			if (e.mFlag & MOUSE_WHEEL_DOWN){
				zoom(-_sense * 2);
			}
			else if (e.mFlag & MOUSE_WHEEL_UP){
				zoom(_sense * 2);
			}

			if (e.mFlag & MOUSE_MIDDLE_DOWN){
				shift(glm::vec2(static_cast<float>(e.mX - _mouse_pre.x) * _sense * 0.8, 
						static_cast<float>(e.mY - _mouse_pre.y) * _sense * 0.8));
			}

			_mouse_pre = glm::vec2(e.mX,e.mY);
		}



	protected:
		void compute();

		float _sense;
		glm::vec2 _mouse_pre;
		
	};
	
	/*
	 * Screen Camera
	 */
	 
	class OPENGLCOURSEAPI ScreenCamera : public Camera {
	public:
		ScreenCamera() {mW = 800; mH = 600; compute(); };
		void setDim(size_t w, size_t h) {mW = w; mH = h; compute(); };
		void align(Primitive &p){};
		virtual void processEvent(ResizeEvent e) { setDim(e.mW, e.mH); };		
		
	protected:
		void compute();
		size_t mW, mH;
	};

	/*
	 * A template that adds inertia and mouse drag movements
	 * \todo make implicit in the visual app somehow?
	 */

	class OPENGLCOURSEAPI InertiaCam : public OrbitCamera {

	protected:
		glm::vec3 mNow;
		glm::vec3 mPrev;
		float mAngle, mDegrade;
		glm::vec3 mP;
		double_t mDT;
		bool mHeld;
		float mZoom, mShift;

	public:

		InertiaCam() { 
			mNow = glm::vec3(0.0f,0.0f,0.0f); 
		 	mAngle = 50.0;
			mDegrade = 2.0;
			mDT = 0;
			mHeld = false;
			mZoom = 0.5;
			mShift = 0.001;
		}

		void processEvent(ResizeEvent e) { 
			setRatio( static_cast<float>(e.mW) / e.mH);
		}

		void processEvent(MouseEvent e){

			if (e.mFlag & MOUSE_LEFT_DOWN){

				if (glm::length(mP) > 0){

					mNow = glm::vec3(static_cast<float>(e.mX - mP.x), static_cast<float>(e.mY - mP.y),0.0f);
					mHeld = true;
				}

			}
			else if (e.mFlag & MOUSE_LEFT_UP ){
				mP = glm::vec3(0.0f,0.0f,0.0f);
				mHeld = false;
			}
			if (e.mFlag & MOUSE_WHEEL_DOWN){
				this->zoom(-mZoom);
			}
			else if (e.mFlag & MOUSE_WHEEL_UP){
				this->zoom(mZoom);
			}

			if (e.mFlag & MOUSE_MIDDLE_DOWN){
				this->shift(glm::vec2(static_cast<float>(e.mX - mP.x) * mShift, 
						static_cast<float>(e.mY - mP.y) * mShift));
			}

			mP = glm::vec3(static_cast<float>(e.mX), static_cast<float>(e.mY),0.0f);
		}

		/*
		 * Update is called as often as possible to complete inertia
		 * \todo work out how best to do zooming and shifting
		 */ 

		void update(double_t dt){
			
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
	};

 }
	 

#endif

