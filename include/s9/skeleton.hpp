/**
* @brief Representation of a skeleton with bones. Used in animation and skinning
* @file skeleton.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 29/10/2013
*
*/

#ifndef S9_SKELETON_HPP
#define S9_SKELETON_HPP

#include "common.hpp"
#include "geometry.hpp"


namespace s9{

  /// A bone represents a rotation, held as a quaternion in this case
  struct Bone {
    Bone(std::string n, Bone* h = nullptr, glm::quat r = glm::quat(), 
      glm::vec3 p = glm::vec3(1.0f)) : name(n), parent(h), rotation(r), position(p) {} 

    friend std::ostream& operator<<(std::ostream& out, const Bone& o);

    Bone* parent;
    std::string name; /// A useful tag for the bone
    glm::quat rotation;
    glm::vec3 position; // Sort of like a length really.

  };

  inline std::ostream& operator<<(std::ostream& os, const Bone& obj) {
    return os << "Bone - " << obj.name << " : " << obj.rotation.x << ", " 
      << obj.rotation.y << ", " << obj.rotation.z << ", " << obj.rotation.w;
  }

  /// Types for the skeleton. We can have one created if we like.
  typedef enum {
    OPENNI_SKELETON,
    CUSTOM_SKELETON
  } SkeletonType;

  /**
   * A Skeleton made up of bones. A hierarchy of rotations if you like.
   * Influenced heavily by OpenNI and Id Software's MD5
   */

  class Skeleton {
  public:
    Skeleton() {};
    Skeleton(SkeletonType type);

    Bone* bone(std::string tag);
    Bone* bone(uint id);

    Skeleton& addBone(Bone* b) { obj_->bones.push_back(b); return *this; }

    const std::vector<Bone*>& bones() const { return obj_->bones; }
   
    friend std::ostream& operator<<(std::ostream& out, const Skeleton& o);

  protected:

    void createOpenNISkeleton();

    struct SharedObject {
      ~SharedObject();
      Bone* top;
      std::vector<Bone*> bones;

    };

    std::shared_ptr<SharedObject> obj_ = nullptr;

  };

  inline std::ostream& operator<<(std::ostream& os, const Skeleton& obj) {
    os << "Skeleton: " << std::endl;
    for (Bone* b : obj.bones()){
      os << "  " << *b << std::endl;
    }
    return os;
  }



}

#endif