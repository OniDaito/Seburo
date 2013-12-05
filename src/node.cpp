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


void NodeShape::draw(GeometryPrimitive overide){

	if (shape_.brewed()) {
		shape_.draw(overide);
	}
	else
		shape_.brew(); ///\todo allow passing of flags
}


/**
 * NodeSkeleton Sign method. We create the clauses on the fly instead
 * This creates a matrix palette, packing the quaternion and position
 * into a matrix4x2
 */

void NodeSkeleton::sign(gl::ShaderVisitor &v) {

	size_t bsize = skeleton_.size();
	glm::mat4 bone_data[shader_bone_limit];

	int idx = 0;
	for (Bone * b : skeleton_.bones()) {

		bone_data[idx] =  b->skinned_matrix();
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

/// Node Constructor with a shape (for convinience)

Node::Node(Shape s) {
	_init();
	add(s);
}

/// _init - This is internal.  Creates a shared object, adding a NodeMinimal for the matrix
/// \todo by using init here we have to call it when we make any add call or similar. Is this verbose or even nice?
void Node::_init() {
	obj_ = shared_ptr<SharedObject>(new SharedObject());
	obj_->matrix_node = std::shared_ptr<NodeMinimal>(new NodeMinimal());
	obj_->bases.push_front(obj_->matrix_node);
	obj_->geometry_cast = NONE;
}


/// Remove a NodeBasePtr explicitly

Node& Node::remove(NodeBasePtr p){
	if (obj_ != nullptr) {
		for (auto it = obj_->bases.begin(); it != obj_->bases.end();) {
	    if (*it == p)  {
	    	it = obj_->bases.erase(it);
	    }
	    else{
	   		++it;
	    }
	  }
	}
}


/// Remove a childnode from this node \todo test this function
Node& Node::removeChild(Node p) {
	if (obj_ != nullptr){
		for (std::vector<Node>::iterator it = obj_->children.begin(); it != obj_->children.end(); ){
			if  ( (*it).obj_ == p.obj_ ){
				obj_->children.erase(it);
			} else {
				++it;
			}
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

/// Remove a skin from this node. Annoying because we are casting to check a pointer! Messy! ><
Node& Node::remove(Skin s) {
	NodeBasePtr t =  getBase(SKIN_WEIGHTS);
	if (t != nullptr){
		shared_ptr<NodeSkin> p =  std::static_pointer_cast<NodeSkin> (t);
		if (p->skin_ == s){
			remove(t);
		}
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
void Node::setMatrix(const glm::mat4 &m) { 
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

Node& Node::remove(Shader s) {
	NodeBasePtr t =  getBase(SHADER);
	if (t != nullptr){
		shared_ptr<NodeShader> p =  std::static_pointer_cast<NodeShader> (t);
		if (p->shader_ == s){
			remove(t);
		}
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


Node& Node::remove(Camera s) {
	NodeBasePtr t =  getBase(CAMERA);
	if (t != nullptr){
		shared_ptr<NodeCamera> p =  std::static_pointer_cast<NodeCamera> (t);
		if (p->camera_ == s){
			remove(t);
		}
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


Node& Node::remove(Shape s) {
	NodeBasePtr t =  getBase(GEOMETRY);
	if (t != nullptr){
		shared_ptr<NodeShape> p =  std::static_pointer_cast<NodeShape> (t);
		if (p->shape_ == s){
			remove(t);
		}
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

Node& Node::remove(Skeleton s) {
	NodeBasePtr t =  getBase(SKELETON);
	if (t != nullptr){
		shared_ptr<NodeSkeleton> p =  std::static_pointer_cast<NodeSkeleton> (t);
		if (p->skeleton_ == s){
			remove(t);
		}
	}
	return *this;
}

/// Add a texture to this node - we can have multiple ones but one per unit only
Node& Node::add(Texture t) {
	if (obj_ == nullptr) _init();

	// Replace existing texture with unit
	for (NodeBasePtr p : obj_->bases){
		if (p->responsible() == TEXTURE){
			shared_ptr<NodeTexture> q =  std::static_pointer_cast<NodeTexture> (p);
			if (q->texture_.unit() == t.unit()){
				obj_->bases.remove(p);
			}
		}
	}
	
	obj_->bases.push_front( NodeBasePtr(new NodeTexture(t)));
	obj_->bases.sort(compareNodeBasePtr);
	
	return *this;
}


Node& Node::remove(gl::Texture t) {
	if (obj_ != nullptr) {
		for (auto it = obj_->bases.begin(); it != obj_->bases.end();) {
	    if ((*it)->responsible() == TEXTURE)  {
	    	shared_ptr<NodeTexture> p =  std::static_pointer_cast<NodeTexture> (*it);
	    	if (p->texture_ == t)
	    		it = obj_->bases.erase(it);
	    	else
	    		++it;
	    }
	    else{
	   		++it;
	    }
	  }
	}
	return *this;
}



/**
 * Remove all nodebases from this node
 */

Node& Node::clear() {
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
 * \todo where we are passing GP, we should have user overrides or similar?
 */

Node& Node::draw(GeometryPrimitive gp) {

	// Call the shared object update - allows Node subclasses polymorphism
	obj_->update();

	// Allow parental overriding if there is something to overide
	GeometryPrimitive fp = gp;
	if (gp == NONE)
		fp = obj_->geometry_cast;

	for (NodeBasePtr p : obj_->bases){
		p->sign(global_visitor);
		p->draw(fp);

	}

	for (Node p : obj_->children){
		p.draw(fp);
	}

	for (NodeBasePtr p : obj_->bases){
		p->postDraw();
	}


	return *this;
}
