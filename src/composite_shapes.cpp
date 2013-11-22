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

  ss->spike = Spike(10,1,1.0f,1.0f);

  // One Cylinder per bone
  for (Bone *b : ss->skeleton.bones() ){
    Node n (ss->spike);
    add(n);
  }
 
}

// Annoyingly we can't override directly so we go through the SharedObject 

void SkeletonShape::SharedObject::update() {

  // Assuming that bones stay in the same order as their spikes - not that it matters too much
  size_t idx = 0;
  for (Bone *b : skeleton.bones() ){

   glm::mat4 mm = glm::mat4(1.0f); 
  
    if (b->parent() != nullptr){
      float l = glm::length( b->position_relative() );

      glm::mat4 flip = glm::rotate(glm::mat4(1.0f), 180.0f, glm::vec3(1.0f,0.0f,0.0f));

      // Figure out the alignment
      glm::vec3 dir = glm::normalize( b->parent()->position_global() - b->position_global() );
      glm::vec4 axis =  glm::vec4(0.0f,1.0f,0.0f,0.0f);
      glm::vec3 axis3 =  glm::normalize(glm::vec3(axis.x, axis.y, axis.z));
      float angle = acos(glm::dot( dir, axis3 ));
      glm::vec3 cross = glm::normalize(glm::cross( axis3, dir));
      glm::quat align_quat = glm::angleAxis(static_cast<float>(radToDeg(angle)),cross);
      glm::mat4 align_mat = glm::toMat4( align_quat);

      glm::vec3 mid_point =  (b->parent()->position_global() + b->position_global()) / 2.0f;

      // Scale for the length
      glm::mat4 scale_mat = skeleton.matrix() * glm::scale(glm::mat4(1.0f),glm::vec3(l/10.0, l, l/10.0));

      // translate, scale, rotate then move to final

      mm = glm::translate( glm::mat4(1.0f), mid_point) * align_mat * scale_mat * flip;

    } else {
      mm = glm::translate( glm::mat4(1.0f), b->position_global());
    }

    children[idx].setMatrix(mm);

    idx++;
  }

  Node::SharedObject::update();
}