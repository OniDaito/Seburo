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

void NodeShape::draw(){
	
	if (brewed_)
		shape_.draw(); ///\todo pass to shader / contract
	else{
		shape_.brew(); ///\todo allow passing of flags
		brewed_ = true;
	}

	component_->draw(); 

}


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

Node& Node::add(Shape s) {
	node_ = NodeBasePtr(new NodeShape(s,node_));
	return *this;
}




/**
 * Draw - recursive function that composes a final node that is actually drawn
 */

Node& Node::draw() {
	///\todo make recursive
	node_->draw();
	return *this;
}
