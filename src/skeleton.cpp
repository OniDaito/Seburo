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



/// Create a skeleton by providing a skeleton type to the constructor.
Skeleton::Skeleton(SkeletonType type) : obj_ (shared_ptr<SharedObject>( new SharedObject())){

  if (type == OPENNI_SKELETON)
    createOpenNISkeleton();

}

/// Create a skeleton hierarchy based on the OpenNI data - Could be hardcoded - read from XML?

void Skeleton::createOpenNISkeleton() {
  
  obj_->bones.push_back (new Bone("Torso"));
  obj_->bones.push_back (new Bone("Neck", obj_->bones[0]));
  obj_->bones.push_back (new Bone("Head", obj_->bones[1]));
  
  obj_->bones.push_back (new Bone("Left Shoulder", obj_->bones[0]));
  obj_->bones.push_back (new Bone("Left Elbow", obj_->bones[3]));
  obj_->bones.push_back (new Bone("Left Wrist", obj_->bones[4]));

  obj_->bones.push_back (new Bone("Right Shoulder", obj_->bones[0]));
  obj_->bones.push_back (new Bone("Right Elbow", obj_->bones[6]));
  obj_->bones.push_back (new Bone("Right Wrist", obj_->bones[7]));

  obj_->bones.push_back (new Bone("Left Hip", obj_->bones[0]));
  obj_->bones.push_back (new Bone("Left Knee", obj_->bones[9]));
  obj_->bones.push_back (new Bone("Left Foot", obj_->bones[10]));

  obj_->bones.push_back (new Bone("Right Hip", obj_->bones[0]));
  obj_->bones.push_back (new Bone("Right Knee", obj_->bones[12]));
  obj_->bones.push_back (new Bone("Right Foot", obj_->bones[13]));

}



/// Destruction of the skeleton - clean up all bones
Skeleton::SharedObject::~SharedObject() {
  for (Bone* b : bones){
    delete b;
  }
}

/// return a pointer to a bone given an id
Bone* Skeleton::bone(uint id){
  if (id < obj_->bones.size())
    return obj_->bones.at(id);
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
    if (b->name.compare(tag) == 0)
      return b;
  }
  return nullptr;
}


Skin::Skin(size_t num_weights) : obj_(std::shared_ptr<SharedObject> (new SharedObject(num_weights))) {
  
}
