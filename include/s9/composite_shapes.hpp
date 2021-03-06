/**
* @brief Composite Shapes
* @file composite_shapes.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 21/11/2013
*
*/


#ifndef COMPOSITE_SHAPES_HPP
#define COMPOSITE_SHAPES_HPP

#include "common.hpp"
#include "node.hpp"
#include "skeleton.hpp"
#include "shapes.hpp"

namespace s9 {

  /**
   * Skeleton shape plots out a given Skeleton using cylinders / Spikes
   */

  class SkeletonShape : public Node {
  public:

    SkeletonShape() {}
    SkeletonShape(const Skeleton &s);

  protected:

    // Annoyingly we can't override Node's draw method as we dont get a full copy of 
    // Skeleton shape so we need to add another NodeBase that allows pre-draw update of the skeleton
    // This is because Node is a shared object.

    struct SharedObject : public Node::SharedObject {

      SharedObject(const Skeleton &s) : Node::SharedObject(), skeleton(s) {}

      void Update();

      Spike spike;
      const Skeleton &skeleton;

      std::vector<Node> bones;
      std::vector<Node> orients;

      glm::vec4 bone_colour = glm::vec4(1.0,0.0,0.0,1.0);
      glm::vec4 orient_colour_x = glm::vec4(0.0,0.0,1.0,1.0);
      glm::vec4 orient_colour_y = glm::vec4(0.0,1.0,0.0,1.0);
    };


  };

}

#endif