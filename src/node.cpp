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

/**
 * Draw function - recursively draws all the children of this node creating a contract
 * as it goes down with whatever shader is bound.
 */

void NodeShape::draw(){
	if (shape_.brewed())
		shape_.draw(); ///\todo pass to shader / contract
	else
		shape_.brew(); ///\todo allow passing of flags

	component_->draw();
}


void NodeShader::draw(){
	shader_.bind();
	component_->draw();
	shader_.unbind();
}

/// Basic Node constructor. Creates a shared object, adding a NodeMinimal for the matrix
Node::Node() : obj_( shared_ptr<SharedObject>(new SharedObject())) {
	obj_->base = std::shared_ptr<NodeMinimal>(new NodeMinimal());
	obj_->node = obj_->base;
}

/// Remove a childnode from this node \todo test this function
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

/// Add a child node to this node
Node& Node::add(Node n) {
	cout << "AHA" << endl;
	obj_->children.push_back(n);
	return *this;
}

/// Add a skin to this node
Node& Node::add(Skin s) { 
	obj_->node = NodeBasePtr(new NodeSkin(s, obj_->node)); 
	return *this;
}

/// Return the matrix for this node
glm::mat4 Node::matrix() {
	return obj_->base->matrix(); 
}

/// set the matrix for this node
void Node::set_matrix(const glm::mat4 &m) { 
	obj_->base->set_matrix(m);
}

/// Add a shader to this node
Node& Node::add(gl::Shader s) {
	obj_->shader = std::shared_ptr<NodeShader>(new NodeShader(s, obj_->node)); 
	return *this;
}



/// Add the drawable for this node - shape (shape being a shared object so we copy) 
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
