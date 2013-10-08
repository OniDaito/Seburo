/**
* @brief Node Class
* @file node.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 08/10/2013
*
*/

#include "s9/node.hpp"

using namespace std;
using namespace s9;



Node::~Node() {}


void Node::rotate(glm::vec3 r){
	glm::quat q_rotate;
	
	q_rotate = glm::rotate( q_rotate, r.x, glm::vec3( 1, 0, 0 ) );
	q_rotate = glm::rotate( q_rotate, r.y, glm::vec3( 0, 1, 0 ) );
	q_rotate = glm::rotate( q_rotate, r.z, glm::vec3( 0, 0, 1 ) );

	mLook = q_rotate * mLook;
	mUp = q_rotate * mUp;

	///\todo the maths here may not be correct
	mRotMatrix = glm::axisAngleMatrix(mLook, glm::dot(glm::vec3(0,1,0), mUp) );
}


/*
 * Recompute the matrices on the primitive
 */
 
void Node::compute() {
	mTransMatrix = glm::translate(glm::mat4(1.0f), mPos);
	mScaleMatrix = glm::scale(glm::mat4(1.0f), mScale);
}


/*
 * Get Matrix - a recursive call all the way up the hierarchy
 */
 
glm::mat4 Node::getMatrix() {
	glm::mat4 m = getLocalMatrix();
	if (getParent())
		m = _getMatrix(getParent(),m);
	return m;
}

glm::mat4 Node::_getMatrix(PrimPtr p, glm::mat4 m) {
	m = p->getLocalMatrix() * m;
	if (p->getParent()){
		_getMatrix(p->getParent(),m);
	}
	return m;
}
 


void Node::pitch(float a){
	glm::quat q_rotate;
	
	glm::vec3 right = glm::cross(mUp,mLook);
	
	q_rotate = glm::rotate( q_rotate, a, right );
	mLook = q_rotate * mLook;
	mUp = q_rotate * mUp;
	
	compute();
}

void Node::roll(float a){
	glm::quat q_rotate;
	q_rotate = glm::rotate( q_rotate, a, mLook);
	mLook = q_rotate * mLook;
	mLook = glm::normalize(mLook);
	mUp = q_rotate * mUp;
	
	compute();
}


void Node::yaw(float a){
	glm::quat q_rotate;
	q_rotate = glm::rotate( q_rotate, a, mUp );
	mLook = q_rotate * mLook;
	mUp = q_rotate * mUp;
	compute();
}
