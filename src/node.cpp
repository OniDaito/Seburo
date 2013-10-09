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

NodePtr Node::removeChild(NodePtr p) {
	for (std::vector<NodePtr>::iterator it = children_.begin(); it != children_.end(); ){
		if  (*it == p){
			children_.erase(it);
		} else {
			++it;
		}
	}

	return std::shared_ptr<Node>(this);
}


NodePtr Node::rotate(glm::vec3 r){
	glm::quat q_rotate;
	
	q_rotate = glm::rotate( q_rotate, r.x, glm::vec3( 1, 0, 0 ) );
	q_rotate = glm::rotate( q_rotate, r.y, glm::vec3( 0, 1, 0 ) );
	q_rotate = glm::rotate( q_rotate, r.z, glm::vec3( 0, 0, 1 ) );

	matrix_ *= glm::toMat4(q_rotate);

	return std::shared_ptr<Node>(this);
}


Node& Node::translate(glm::vec3 p) {
	glm::mat4 trans = glm::mat4(1.0f);
	glm::translate(trans,p);
	matrix_ *= trans;
	return *this;
}



/**
 * Pitch around global x axis
 */

NodePtr Node::pitch(float a){
	rotate(glm::vec3(1.0,0.0,0.0));
	return std::shared_ptr<Node>(this);
}

/**
 * Pitch around global z axis
 */

NodePtr Node::roll(float a){
	rotate(glm::vec3(0.0,0.0,1.0));
	return std::shared_ptr<Node>(this);
}


/**
 * Pitch around global y axis
 */

NodePtr Node::yaw(float a){
	rotate(glm::vec3(0.0,1.0,0.0));
	return std::shared_ptr<Node>(this);
}



/**
 * Draw - recursive function that composes a final node that is actually drawn
 */

void Node::draw() {
	
}
