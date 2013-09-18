/**
* @brief Primitive Classes
* @file primitive.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/


#ifndef S9_PRIMITIVE_HPP
#define S9_PRIMITIVE_HPP

#include "common.hpp"
#include "visualapp.hpp"


/*
 * Primitive represents a *thing* that has a position in space and time (though not a size)
 * it also has a colour for picking and several subclasses
 * \todo SharedObj model is not needed here as the parameters can be copied with no real loss but CX this
 */
 
namespace s9 {

	class Primitive;
	
	typedef boost::shared_ptr<Primitive> PrimPtr;

	class OPENGLCOURSEAPI Primitive {
		
	protected:
	
		glm::mat4 _getMatrix(PrimPtr p, glm::mat4 m);
	
		PrimPtr pParent;
		std::vector<PrimPtr> vChildren;
		glm::vec3 mPos;
		glm::vec3 mUp;
		glm::vec3 mLook;
		glm::vec3 mScale;
		
		glm::vec4 mColour; // for picking
	
		glm::mat4 mTransMatrix;
		glm::mat4 mRotMatrix;
		glm::mat4 mScaleMatrix;

		
	public:
		Primitive() {
			// Initial state of the world
			mPos = glm::vec3(0,0,0);
			mLook = glm::vec3(0,0,-1);
			mUp = glm::vec3(0,1,0);
			
			mRotMatrix = glm::mat4(1.0f);
			mTransMatrix = glm::mat4(1.0f);
			mScaleMatrix = glm::mat4(1.0f);

			mScale = glm::vec3(1.0f,1.0f,1.0f);

			mColour = glm::vec4(1.0,0.0,1.0,1.0);

		}

	//	virtual operator int() const { return mObj.use_count() > 0; };

		virtual ~Primitive(); 
		
		virtual void move(glm::vec3 p) { mPos += p; compute(); };
		virtual void rotate(glm::vec3 r);
		glm::mat4 getMatrix();
		glm::mat4 getLocalMatrix() { return mTransMatrix * mRotMatrix * mScaleMatrix; };
		
		glm::mat4 getTransMatrix() { return mTransMatrix; };
		glm::mat4 getRotMatrix() { return mRotMatrix; };
		glm::mat4 getScaleMatrix() { return mScaleMatrix; };
		
		virtual void setLook(glm::vec3 v) {mLook = v; glm::normalize(v); compute(); };
		virtual void setPos(glm::vec3 v) {mPos = v;compute(); };
		virtual void setScale(glm::vec3 v) {mScale = v; compute(); };
		virtual void setColour(glm::vec4 v) {mColour = v; };
		
		int addChild(PrimPtr p) { vChildren.push_back(p); p->pParent = PrimPtr(this); return vChildren.size()-1; };
		void removeChild(PrimPtr p){};
		
		virtual void compute();
		
		glm::vec3 getPos() { return mPos;};
		glm::vec3 getLook() { return mLook;};
		glm::vec3 getScale() { return mScale;};
		glm::vec3 getUp() { return mUp;};
		glm::vec4 getColour() { return mColour;};

		virtual void yaw(float a);
		virtual void pitch(float a);
		virtual void roll(float a);
				
	
		PrimPtr getParent(){return pParent; };
			
	};

	
}

#endif
