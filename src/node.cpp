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
	if (shape_.brewed())
		shape_.draw(); ///\todo pass to shader / contract
	else
		shape_.brew(); ///\todo allow passing of flags

	component_->draw();
}

Node::Node() : obj_( shared_ptr<SharedObject>(new SharedObject())) {
	obj_->base = std::shared_ptr<NodeMinimal>(new NodeMinimal());
	obj_->node = obj_->base;
}

Node& Node::removeChild(Node p) {
	for (std::vector<Node>::iterator it = obj_->children.begin(); it != obj_->children.end(); ){
		if  ( (*it).obj_ == p.obj_ ){
			obj_->children.erase(it);
		} else {
			++it;
		}
	}
	return *this;
}

Node& Node::add(Node n) {
	obj_->children.push_back(n);
	return *this;
}

Node& Node::add(Skin s) { 
	obj_->node = NodeBasePtr(new NodeSkin(s, obj_->node)); 
	return *this;
}

glm::mat4 Node::matrix() {
	return obj_->base->matrix(); 
}

void Node::set_matrix(const glm::mat4 &m) { 
	obj_->base->set_matrix(m);
}

/**
 * Add the drawable for this node - shape (shape being a shared object so we copy)
 */

Node& Node::add(Shape s) {
	obj_->node = NodeBasePtr(new NodeShape(s, obj_->node));
	return *this;
}


/**
 * Draw - recursive function that composes a final node that is actually drawn
 */

Node& Node::draw() {

	obj_->node->draw();

	for (Node p : obj_->children){
		p.draw();
	}

	return *this;
}
