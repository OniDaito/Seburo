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

  /** 
   * A skin is a collection of weights and indices. It is a many to many relationship
   * between vertices and joints with a weight between them.
   * Skin follows the MD5 Approach by having an index and a range.
   */

  class Skin {
  public:
    Skin() {};
    Skin(size_t num_weights);

    /**
     * A struct that indexes our weights in the Id MD5 Style
     * This index MUST match the vertices it is attached to.
     * \todo alignment on boundaries?
     */

    struct SkinIndex {
      IndicesType   idx;
      size_t      count;
    };
    
    /**
     * A weight for a skin. This must link to the skeleton in the current context
     */

    struct SkinWeight {
      Bone*       bone;
      glm::vec3   position; ///\todo Id has this but not sure why :S
      float       bias;
    };

  
  protected:
 
    struct SharedObject {
      SharedObject(size_t n) { num_weights = n; }
      ~SharedObject() {};

      std::vector<SkinIndex>  indices;
      std::vector<SkinWeight> weights;
      size_t                  num_weights;
    };

    std::shared_ptr<SharedObject> obj_ = nullptr;

  public:
    void addIndex (SkinIndex s) { obj_->indices.push_back(s); }
    void addWeight (SkinWeight w) { obj_->weights.push_back(w); }

    std::vector<SkinIndex>& indices() { return obj_->indices; }
    std::vector<SkinWeight>& weights() { return obj_->weights; }

  };

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