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

vector<glm::mat4> NodeMinimal::matrix_stack_;
static ShaderVisitor global_visitor;

/**
 * Draw function - recursively draws all the children of this node creating a contract
 * as it goes down with whatever shader is bound.
 */

 bool compareNodeBasePtr( NodeBasePtr l, NodeBasePtr r){
 	return (*l) < (*r);
 } 

void NodeShape::draw(){

	if (shape_.brewed())
		shape_.draw(); ///\todo pass to shader / contract
	else
		shape_.brew(); ///\todo allow passing of flags

	//CXGLERROR
}




/**
 * NodeSkeleton Sign method. We create the clauses on the fly instead
 * This creates a matrix palette, packing the quaternion and position
 * into a matrix4x2
 */

void NodeSkeleton::sign(gl::ShaderVisitor &v) {

	size_t bsize = skeleton_.bones().size();
	glm::mat4 bone_data[shader_bone_limit];

	
	int idx = 0;
	for (Bone * b : skeleton_.bones()) {
		glm::mat4 boneTranslation = glm::translate( glm::mat4(1.0f), b->position);
		glm::mat4 boneRotation = glm::toMat4(b->rotation);  
		bone_data[idx] =  boneTranslation * boneRotation;

		idx++;
		if (idx >= shader_bone_limit){
			cerr << "SEBURO Shader Clause Error - Number of bones in model exceeds shader limit." << endl;
			break;
		}
	}

	gl::ShaderClause<glm::mat4, shader_bone_limit> clause_bones("uBonePalette", bone_data ); 
	v.sign(clause_bones);

	gl::ShaderClause<float,1> clause_num_bones("uNumBones", bsize ); 
	v.sign(clause_num_bones);
}



/// Basic Node constructor.
Node::Node() {}

/// _init - This is internal.  Creates a shared object, adding a NodeMinimal for the matrix
/// \todo by using init here we have to call it when we make any add call or similar. Is this verbose or even nice?
void Node::_init() {
	obj_ = shared_ptr<SharedObject>(new SharedObject());
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

/// Add a child node to this node. If the node being added is not initialised, make it so
Node& Node::add(Node &n) {
	if (obj_ == nullptr) _init();
	if (n.obj_ == nullptr) n._init();

	obj_->children.push_back(n);
	return *this;
}

/// Add a skin to this node
Node& Node::add(Skin s) {
	if (obj_ == nullptr) _init();
	if ( getBase(SKIN_WEIGHTS) == nullptr ){
		obj_->bases.push_front( NodeBasePtr(new NodeSkin(s)));
		obj_->bases.sort(compareNodeBasePtr);
	} else {
		cerr << "SEBURO Node - Trying to add a skin to a node when one already exists." << endl;
	}
	return *this;
}

/// Return the matrix for this node
glm::mat4 Node::matrix() {
	if (obj_ == nullptr)
			return glm::mat4(1.0f);
	return obj_->matrix_node->matrix(); 
}

/// set the matrix for this node
void Node::set_matrix(const glm::mat4 &m) { 
	if (obj_ != nullptr)
		obj_->matrix_node->set_matrix(m);
}

/// Add a shader to this node
Node& Node::add(gl::Shader s) {
	if (obj_ == nullptr) _init();
	if ( getBase(SHADER) == nullptr ){
		obj_->shader_node = std::shared_ptr<NodeShader>(new NodeShader(s));
		obj_->bases.push_front(obj_->shader_node);
		obj_->bases.sort(compareNodeBasePtr);
	} else {
		cerr << "SEBURO Node - Trying to add a shader to a node when one already exists." << endl;
	}

	return *this;
}

/// Add a shader to this node
Node& Node::add(Camera c) {
	if (obj_ == nullptr) _init();
	if ( getBase(CAMERA) == nullptr ){
		obj_->bases.push_front( NodeBasePtr(new NodeCamera(c)));
		obj_->bases.sort(compareNodeBasePtr);
	} else {
		cerr << "SEBURO Node - Trying to add a camera to a node when one already exists." << endl;
	}
	return *this;
}

/// Add the drawable for this node 
Node& Node::add(Shape s) {
	if (obj_ == nullptr) _init();
	if ( getBase(GEOMETRY) == nullptr ){
		obj_->bases.push_front( NodeBasePtr(new NodeShape(s)));
		obj_->bases.sort(compareNodeBasePtr);
	} else {
		cerr << "SEBURO Node - Trying to add a Shape to a node when one already exists." << endl;
	}
	return *this;
}


/// Add a skeleton to this node 
Node& Node::add(Skeleton s) {
	if (obj_ == nullptr) _init();
	if ( getBase(SKELETON) == nullptr ){
		obj_->bases.push_front( NodeBasePtr(new NodeSkeleton(s)));
		obj_->bases.sort(compareNodeBasePtr);
	} else {
		cerr << "SEBURO Node - Trying to add a Skeleton to a node when one already exists." << endl;
	}
	return *this;
}


/**
 * Remove all nodebases from this node
 */

Node& Node::reset() {
	obj_->bases.clear();
	obj_->matrix_node.reset();
	obj_->shader_node.reset();
	return *this;
}

/**
 * Internal function to grab a pointer to a base with a responsibility
 */

NodeBasePtr Node::getBase(NodeResponsibility r) {
	for (NodeBasePtr p : obj_->bases){
		if (p->responsible() == r)
			return p;
	}
	return nullptr;
}

/**
 * Draw call for this node. Recursive at present
 */

Node& Node::draw() {

	for (NodeBasePtr p : obj_->bases){
		p->sign(global_visitor);
		p->draw();
	}

	for (Node p : obj_->children){
		p.draw();
	}


	for (NodeBasePtr p : obj_->bases){
		p->postDraw();
	}


	return *this;
}
