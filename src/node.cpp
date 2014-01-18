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
size_t NodeShader::bind_count_ = 0;

// If OpenGL was multi-threaded, dang we'd be in trouble
vector<glm::mat4> NodeCamera::projection_matrix_stack_;
vector<glm::mat4> NodeCamera::view_matrix_stack_;

/**
 * Draw function - recursively draws all the children of this node creating a contract
 * as it goes down with whatever shader is bound.
 */


void NodeShape::Draw(GeometryPrimitive overide){

	if (shape_.brewed()) {
		shape_.Draw(overide);
			CXGLERROR
	}
	else{
		shape_.Brew(); ///\todo allow passing of flags
			CXGLERROR
	}
}


/**
 * NodeSkeleton Sign method. We create the clauses on the fly instead
 * This creates a matrix palette, packing the quaternion and position
 * into a matrix4x2
 */

void NodeSkeleton::Collect(gl::ShaderVisitor &v) {

	size_t bsize = skeleton_.Size();
	glm::mat4 bone_data[shader_bone_limit];

	int idx = 0;
	for (Bone * b : skeleton_.bones()) {

		bone_data[idx] =  b->skinned_matrix();
		idx++;
		if (idx >= shader_bone_limit){
			cerr << "SEBURO Shader Clause Error - Number of bones in model exceeds shader limit - " << idx << " vs " <<  shader_bone_limit << endl;
			break;
		}
	}

	gl::ShaderClause<glm::mat4, shader_bone_limit> clause_bones("uBonePalette", bone_data ); 
	v.Sign(clause_bones);

	gl::ShaderClause<float,1> clause_num_bones("uNumBones", bsize ); 
	v.Sign(clause_num_bones);
}



/// Basic Node constructor.
Node::Node() {}

/// Node Constructor with a shape (for convinience)

Node::Node(Shape s) {
	_init();
	Add(s);
}

/// _init - This is internal.  Creates a shared object, adding a NodeMinimal for the matrix and a camera
/// \todo by using init here we have to call it when we make any add call or similar. Is this verbose or even nice?
void Node::_init() {
	obj_ = shared_ptr<SharedObject>(new SharedObject());
	obj_->camera_node = std::shared_ptr<NodeCamera>(new NodeCamera());
	obj_->bases.push_front(obj_->camera_node);
	obj_->matrix_node = std::shared_ptr<NodeMinimal>(new NodeMinimal());
	obj_->bases.push_front(obj_->matrix_node);

	obj_->geometry_cast = NONE;
}


/// Remove a NodeBasePtr explicitly

Node& Node::Remove(NodeBasePtr p){
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
Node& Node::RemoveChild(Node p) {
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
Node& Node::Add(Node &n) {
	if (obj_ == nullptr) _init();
	if (n.obj_ == nullptr) n._init();

	obj_->children.push_back(n);
	return *this;
}

/// Add a skin to this node
Node& Node::Add(Skin s) {
	if (obj_ == nullptr) _init();
	if ( GetBase(SKIN_WEIGHTS) == nullptr ){
		obj_->bases.push_front( NodeBasePtr(new NodeSkin(s)));
		obj_->bases.sort(CompareNodeBasePtr);
	} else {
		cerr << "SEBURO Node - Trying to add a skin to a node when one already exists." << endl;
	}
	return *this;
}

/// Remove a skin from this node. Annoying because we are casting to check a pointer! Messy! ><
Node& Node::Remove(Skin s) {
	NodeBasePtr t =  GetBase(SKIN_WEIGHTS);
	if (t != nullptr){
		shared_ptr<NodeSkin> p =  std::static_pointer_cast<NodeSkin> (t);
		if (p->skin_ == s){
			Remove(t);
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
void Node::set_matrix(const glm::mat4 &m) { 
	if (obj_ != nullptr)
		obj_->matrix_node->set_matrix(m);
}

/// Add a shader to this node
Node& Node::Add(gl::Shader s) {
	if (obj_ == nullptr) _init();
	if ( GetBase(SHADER) == nullptr ){
		obj_->shader_node = std::shared_ptr<NodeShader>(new NodeShader(s));
		obj_->bases.push_front(obj_->shader_node);
		obj_->bases.sort(CompareNodeBasePtr);
	} else {
		cerr << "SEBURO Node - Trying to add a shader to a node when one already exists." << endl;
	}

	return *this;
}

Node& Node::Remove(Shader s) {
	NodeBasePtr t = GetBase(SHADER);
	if (t != nullptr){
		shared_ptr<NodeShader> p =  std::static_pointer_cast<NodeShader> (t);
		if (p->shader_ == s){
			Remove(t);
		}
	}
	return *this;
}


/// Add a camera to this node. We always have a basic camera node so we can swap the camera in
Node& Node::Add(Camera c) {
	if (obj_ == nullptr) _init();

	// Nasty line
	shared_ptr<NodeCamera> p =  std::static_pointer_cast<NodeCamera> (GetBase(CAMERA));	
	p->camera_ = c;

	return *this;
}


/// Remove a camera from this node, reverting back to the blank camera node
Node& Node::Remove(Camera s) {
	NodeBasePtr t =  GetBase(CAMERA);
	if (t != nullptr){
		shared_ptr<NodeCamera> p =  std::static_pointer_cast<NodeCamera> (t);
		if (p->camera_ == s){
			p->camera_.reset();
		}
	}
	return *this;
}



/// Add the drawable for this node 
Node& Node::Add(Shape s) {
	if (obj_ == nullptr) _init();
	if ( GetBase(GEOMETRY) == nullptr ){
		obj_->bases.push_front( NodeBasePtr(new NodeShape(s)));
		obj_->bases.sort(CompareNodeBasePtr);
	} else {
		cerr << "SEBURO Node - Trying to add a Shape to a node when one already exists." << endl;
	}
	return *this;
}


Node& Node::Remove(Shape s) {
	NodeBasePtr t =  GetBase(GEOMETRY);
	if (t != nullptr){
		shared_ptr<NodeShape> p =  std::static_pointer_cast<NodeShape> (t);
		if (p->shape_ == s){
			Remove(t);
		}
	}
	return *this;
}


/// Add a skeleton to this node 
Node& Node::Add(Skeleton s) {
	if (obj_ == nullptr) _init();
	if ( GetBase(SKELETON) == nullptr ){
		obj_->bases.push_front( NodeBasePtr(new NodeSkeleton(s)));
		obj_->bases.sort(CompareNodeBasePtr);
	} else {
		cerr << "SEBURO Node - Trying to add a Skeleton to a node when one already exists." << endl;
	}
	return *this;
}

Node& Node::Remove(Skeleton s) {
	NodeBasePtr t =  GetBase(SKELETON);
	if (t != nullptr){
		shared_ptr<NodeSkeleton> p =  std::static_pointer_cast<NodeSkeleton> (t);
		if (p->skeleton_ == s){
			Remove(t);
		}
	}
	return *this;
}

/// Add a texture to this node - we can have multiple ones but one per unit only
Node& Node::Add(Texture t) {
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
	obj_->bases.sort(CompareNodeBasePtr);
	
	return *this;
}


Node& Node::Remove(gl::Texture t) {
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

Node& Node::Clear() {
	obj_->bases.clear();
	obj_->matrix_node.reset();
	obj_->shader_node.reset();
	return *this;
}

/**
 * Internal function to grab a pointer to a base with a responsibility
 */

NodeBasePtr Node::GetBase(NodeResponsibility r) {
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

Node& Node::Draw(GeometryPrimitive gp) {
	// Call the shared object update - allows Node subclasses polymorphism
	obj_->Update();

	// Allow parental overriding if there is something to overide
	GeometryPrimitive fp = gp;
	if (gp == NONE)
		fp = obj_->geometry_cast;

	for (NodeBasePtr p : obj_->bases){
		p->PreDraw();
		if (NodeShader::bind_count_ > 0)
			p->Collect(global_visitor);
		p->Draw(fp);

	}

	for (Node p : obj_->children){
		p.Draw(fp);
	}

	for (NodeBasePtr p : obj_->bases){
		p->PostDraw();
	}


	return *this;
}
