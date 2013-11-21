/**
* @brief Composite Shapes
* @file composite_shapes.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 21/11/2013
*
*/

#include "s9/composite_shapes.hpp"

using namespace std;
using namespace s9;

/**
 * \todo potentially we could have a stack of primitive objects already loaded, such as the cylinder
 * and if we suddenly send another one to brew method, we just link to existing geometry? 
 * geometry allocation method at all? Or could we just mess with the constructor? Its tricky really
 */

/// SkeletonShape main constructor
SkeletonShape::SkeletonShape(const Skeleton &s) : Node() {

  // Don't call node::init() - override that here
  obj_ = shared_ptr<SharedObject>(new SkeletonShape::SharedObject(s));

  // Now cast - Which I really dont like ><
  shared_ptr<SkeletonShape::SharedObject> ss = std::static_pointer_cast<SkeletonShape::SharedObject>(obj_);

  ss->matrix_node = std::shared_ptr<NodeMinimal>(new NodeMinimal());
  ss->bases.push_front(obj_->matrix_node);
  ss->geometry_cast = NONE;

  // A unit cylinder that we will stretch

  ss->cylinder = Cylinder(10,1,1.0f,1.0f);

  // One Cylinder per bone
  for (Bone *b : ss->skeleton.bones() ){
    Node n (ss->cylinder);
    add(n);
  }
 
}

// Annoyingly we can't override directly so we go through the SharedObject 

void SkeletonShape::SharedObject::update() {
  
  // Assuming that bones stay in the same order as their cylinders - not that it matters too much
  size_t idx = 0;
  for (Bone *b : skeleton.bones() ){

   children[idx].setMatrix( b->global_matrix() );

   idx++;
  }

   Node::SharedObject::update();
}