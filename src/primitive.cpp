/**
* @brief Primitive Classes
* @file primitive.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/

#include "primitive.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign; 
using namespace s9;



Primitive::~Primitive() {}


void Primitive::rotate(glm::vec3 r){
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
 
void Primitive::compute() {
	mTransMatrix = glm::translate(glm::mat4(1.0f), mPos);
	mScaleMatrix = glm::scale(glm::mat4(1.0f), mScale);
}


/*
 * Get Matrix - a recursive call all the way up the hierarchy
 */
 
glm::mat4 Primitive::getMatrix() {
	glm::mat4 m = getLocalMatrix();
	if (getParent())
		m = _getMatrix(getParent(),m);
	return m;
}

glm::mat4 Primitive::_getMatrix(PrimPtr p, glm::mat4 m) {
	m = p->getLocalMatrix() * m;
	if (p->getParent()){
		_getMatrix(p->getParent(),m);
	}
	return m;
}
 

