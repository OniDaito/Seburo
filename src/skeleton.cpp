/**
* @brief Skeleton Class
* @file skeleton.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 29/10/2013
*
*/

#include "s9/skeleton.hpp"

using namespace std;
using namespace s9;

 bool compareBonePtr( Bone* l, Bone* r){
  return l->id() < r->id();
 } 

/// Rotate a bone (as oppose to a joint) by applying a quaternion to its alignment and local position
void Bone::applyRotation(const glm::quat &q) {
  glm::mat4 tm = glm::toMat4(q);

  glm::vec4 tv = tm * glm::vec4(position_relative_.x, position_relative_.y, position_relative_.z, 1.0f) ;
  position_relative_ = glm::vec3(tv.x, tv.y, tv.z);
  rotation_relative_ = glm::normalize( q * rotation_relative_);
}


/// Create a skeleton by providing a skeleton type to the constructor.
Skeleton::Skeleton(SkeletonType type) : obj_ (shared_ptr<SharedObject>( new SharedObject())){

  if (type == OPENNI_SKELETON)
    createOpenNISkeleton();

  obj_->matrix = glm::mat4(1.0f);
}


Skeleton& Skeleton::addBone(Bone* b) { 
  obj_->bones.push_front(b); 
  obj_->bones.sort(compareBonePtr); 
  return *this; 
}

/// Create a skeleton hierarchy based on the OpenNI data - Could be hardcoded - read from XML?
/// This is ordered so all parents come first. That way, we can update the hierarchy quickly

void Skeleton::createOpenNISkeleton() {
  
  obj_->bones.push_front (new Bone("Torso", 0));
  obj_->bones.push_front (new Bone("Neck", 1, bone(0)));
  obj_->bones.push_front (new Bone("Head", 2, bone(1)));
  
  obj_->bones.push_front (new Bone("Left Shoulder", 3, bone(0)));
  obj_->bones.push_front (new Bone("Left Elbow", 4, bone(3)));
  obj_->bones.push_front (new Bone("Left Wrist", 5, bone(4)));

  obj_->bones.push_front (new Bone("Right Shoulder", 6, bone(0)));
  obj_->bones.push_front (new Bone("Right Elbow", 7, bone(6)));
  obj_->bones.push_front (new Bone("Right Wrist", 8, bone(7)));

  obj_->bones.push_front (new Bone("Left Hip", 9, bone(0)));
  obj_->bones.push_front (new Bone("Left Knee", 10, bone(9)));
  obj_->bones.push_front (new Bone("Left Foot", 11, bone(10)));

  obj_->bones.push_front (new Bone("Right Hip", 12, bone(0)));
  obj_->bones.push_front (new Bone("Right Knee", 13, bone(12)));
  obj_->bones.push_front (new Bone("Right Foot", 14, bone(13)));

  obj_->bones.sort(compareBonePtr); 

}


/// Destruction of the skeleton - clean up all bones
Skeleton::SharedObject::~SharedObject() {
  for (Bone* b : bones){
    delete b;
  }
}

/// return a pointer to a bone given an id. id should match its position in the list
Bone* Skeleton::bone(uint id){
  for (Bone* b : obj_->bones){
    if (b->id() == id){
      return b;
    }
  }
  
  return nullptr;
}

/// Get the index of the bone in the array - useful for passing to the shader
int Skeleton::getBoneIndex(Bone* p) {
  int idx = 0;

  for (Bone* b : obj_->bones){
    if (b == p ){
      return idx;
    }
    idx++;
  }
  return -1;
}

/// Return a pointer to a bone given a string name tag
Bone* Skeleton::bone(string tag){
  for (Bone* b : obj_->bones){
    if (b->name().compare(tag) == 0)
      return b;
  }
  return nullptr;
}

/// Update all the bones in the skeleton. Assume an order for the bones (all parents come first)
/// \todo enforce ordering somehow (on the add no doubt)

void Skeleton::update() {
  for (Bone* b : obj_->bones){

    if (b->parent() == nullptr){
      b->rotation_global_ = b->rotation_relative_;
      b->position_global_ = b->position_relative_;
    } else {
      b->rotation_global_ = b->parent()->rotation_global_ * b->rotation_relative_;
      b->position_global_ = b->parent()->position_global_ + b->position_relative_;
    }
  }

  for (Bone* b : obj_->bones){
    b->set_skinned_matrix( glm::translate(glm::mat4(1.0f), b->position_global_ ) * 
      glm::toMat4(b->rotation_global_) * b->inverse_bind_pose() );
  }

}

/// Set all the bones' global matrices
void Skeleton::resetGlobals() {
  ///\todo fill this in :O

}

/// Creates a string for printing

std::string Skeleton::toString() {
  std::stringstream s;
  s << "Skeleton" << endl;
  size_t idx = 0;
  for (Bone* b : obj_->bones){
    s << "  " << idx << " : " <<  b->id() << " - " << b->name() << endl;
    idx++;
  }
  return s.str();
}


Skin::Skin(size_t num_weights) : obj_(std::shared_ptr<SharedObject> (new SharedObject(num_weights))) {
  
}
