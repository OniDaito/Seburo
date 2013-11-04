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


Node& Node::removeChild(NodePtr p) {
	for (std::vector<NodePtr>::iterator it = children_.begin(); it != children_.end(); ){
		if  (*it == p){
			children_.erase(it);
		} else {
			++it;
		}
	}

	return *this;
}


/**
 * Add the drawable for this node - shape (shape being a shared object so we copy)
 */

Node& Node::add(Shape shape) {
	node_ = NodeBasePtr(new NodeShape(node_, shape));
	return *this;
}




/**
 * Draw - recursive function that composes a final node that is actually drawn
 */

void Node::draw() {

	if (shape_.drawable()){
		if (shape_.brewed()){
			shape_.draw(); ///\todo - allow the settings of flags here
		} else {
			shape_.brew(); ///\todo - allow brew flags here
		}
	}
}
