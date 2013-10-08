/**
* @brief Node Classes
* @file primitive.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/


#ifndef S9_NODE_HPP
#define S9_NODE_HPP

#include "common.hpp"
#include "visualapp.hpp"


/*
 * Node represents a *thing* that has a position in space and time (though not a size)
 * Node combines geometry with matrices to represent position in space. It can be matched with a shader
 * to create something that is drawable. It can also have children and thus create a scene graph
 * \todo SharedObj model is not needed here as the parameters can be copied with no real loss but CX this
 */
 
namespace s9 {

	class Node;
	
	typedef std::shared_ptr<Node> NodePtr;

	class SEBUROAPI Node {
		
	protected:
	
		glm::mat4 _getMatrix(NodePtr p, glm::mat4 m);
	
		NodePtr pParent;
		std::vector<NodePtr> vChildren;
		glm::vec3 mPos;
		glm::vec3 mUp;
		glm::vec3 mLook;
		glm::vec3 mScale;
		
		glm::vec4 mColour; // for picking
	
		glm::mat4 mTransMatrix;
		glm::mat4 mRotMatrix;
		glm::mat4 mScaleMatrix;

		
	public:
		Node() {
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

		virtual ~Node(); 
		
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
		
		int addChild(NodePtr p) { vChildren.push_back(p); p->pParent = NodePtr(this); return vChildren.size()-1; };
		void removeChild(NodePtr p){};
		
		virtual void compute();
		
		glm::vec3 getPos() { return mPos;};
		glm::vec3 getLook() { return mLook;};
		glm::vec3 getScale() { return mScale;};
		glm::vec3 getUp() { return mUp;};
		glm::vec4 getColour() { return mColour;};

		virtual void yaw(float a);
		virtual void pitch(float a);
		virtual void roll(float a);
				
	
		NodePtr getParent(){return pParent; };
			
	};

	
}

#endif
