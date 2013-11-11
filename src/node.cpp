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
using namespace s9::gl;

/**
 * Draw function - recursively draws all the children of this node creating a contract
 * as it goes down with whatever shader is bound.
 */

 bool compareNodeBasePtr( NodeBasePtr l, NodeBasePtr r){
 	return (*l) < (*r);
 } 

void NodeShape::draw(){
	contract_.sign();

	if (shape_.brewed())
		shape_.draw(); ///\todo pass to shader / contract
	else
		shape_.brew(); ///\todo allow passing of flags
	CXGLERROR

}

void NodeShader::draw(){
	shader_.bind();
}

/// Basic Node constructor. Creates a shared object, adding a NodeMinimal for the matrix
Node::Node() : obj_( shared_ptr<SharedObject>(new SharedObject())) {
	obj_->matrix_node = std::shared_ptr<NodeMinimal>(new NodeMinimal());
	obj_->bases.push_front(obj_->matrix_node);

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
	obj_->children.push_back(n);
	return *this;
}

/// Add a skin to this node
Node& Node::add(Skin s) { 
	obj_->bases.push_front( NodeBasePtr(new NodeSkin(s)));
	obj_->bases.sort(compareNodeBasePtr);
	return *this;
}

/// Return the matrix for this node
glm::mat4 Node::matrix() {
	return obj_->matrix_node->matrix(); 
}

/// set the matrix for this node
void Node::set_matrix(const glm::mat4 &m) { 
	obj_->matrix_node->set_matrix(m);
}

/// Add a shader to this node
Node& Node::add(gl::Shader s) {

	obj_->shader_node = std::shared_ptr<NodeShader>(new NodeShader(s));
	obj_->bases.push_front(obj_->shader_node);
	obj_->bases.sort(compareNodeBasePtr);

	return *this;
}

/// Add a shader to this node
Node& Node::add(Camera c) {
	obj_->bases.push_front( NodeBasePtr(new NodeCamera(c)));
	obj_->bases.sort(compareNodeBasePtr);
	return *this;
}

/// Add the drawable for this node - shape (shape being a shared object so we copy) 
Node& Node::add(Shape s) {
	obj_->bases.push_front( NodeBasePtr(new NodeShape(s)));
	obj_->bases.sort(compareNodeBasePtr);
	return *this;
}


/**
 * Draw - recursive function that composes a final node that is actually drawn
 */

Node& Node::draw() {

	for (NodeBasePtr p : obj_->bases)
		p->draw();

	for (Node p : obj_->children)
		p.draw();

	// At this point we can unbind shaders - Ths is because shaders remain in place 
	// as we progress down the tree unless another replaces it.
	//if (obj_->shader_node != nullptr)
	//	obj_->shader_node->shader_.unbind();

	

	return *this;
}
