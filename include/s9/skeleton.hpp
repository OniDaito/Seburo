/**
* @brief Representation of a skeleton with bones. Used in animation and skinning
* @file skeleton.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 29/10/2013
*
*/

#ifndef S9_SKELETON_HPP
#define S9_SKELETON_HPP

#include <forward_list>
#include "common.hpp"
#include "geometry.hpp"


namespace s9{

  class Skeleton;

  /**
   *  A bone represents a rotation, held as a quaternion in this case. 
   *  The initial rotation and translation values form the bind pose matrix.
   *  We invert these matrices so we can get the inverse_bind_pose_ matrix and 
   *  perform skinning normally.
   */ 

  class Bone {

  public:

    /**
     * This constructor assumes the parent bone is already setup.
     * It creates an inverse bind pose matrix from the information given 
     * and creates a position and rotation relative to the parent node 
     */

    Bone(std::string n, int idx,  Bone* h = nullptr, glm::quat r = glm::quat(), 
      glm::vec3 p = glm::vec3(1.0f)) : name_(n), id_(idx), parent_(h), 
      position_global_(p), rotation_global_(r) { 
      
      glm::mat4 global_matrix =  glm::translate( glm::mat4(1.0f), p) * glm::toMat4(r);
      inverse_bind_pose_ = glm::inverse(global_matrix);
   
      // Create relative positions
      if (parent_ != nullptr){
        rotation_relative_ = glm::inverse(parent_->rotation_global_) * r;
        position_relative_ = p - parent_->position_global_;
      } else {
        rotation_relative_ = r;
        position_relative_ = p;
      }
  
    } 

    friend std::ostream& operator<<(std::ostream& out, const Bone& o);

    const std::string name () const {return name_;}
    glm::quat rotation_relative() const{ return rotation_relative_; }
    glm::vec3 position_relative() const { return position_relative_; }

    glm::quat rotation() const { return rotation_relative(); }
    glm::vec3 position() const  { return position_relative(); }

    glm::quat rotation_global() const{ return rotation_global_; }
    glm::vec3 position_global() const { return position_global_; }

    const int id() const {return id_; }

    const Bone* parent() {return parent_; }
 
    void applyRotation(const glm::quat &q);

    glm::mat4 skinned_matrix() const { return skinned_matrix_; }
    glm::mat4 inverse_bind_pose() const { return inverse_bind_pose_; }

  protected:

    void set_skinned_matrix(const glm::mat4 &m) {skinned_matrix_ = m; } 

    Bone* parent_;
    std::string name_;   /// A useful tag for the bone
    int id_;             /// An ID for the bone - parents have lower numbers than children
    glm::quat rotation_relative_; /// rotation relative to the parent 
    glm::vec3 position_relative_; /// position relative to the parent


    glm::quat rotation_global_;
    glm::vec3 position_global_;

    glm::mat4 inverse_bind_pose_;
    glm::mat4 skinned_matrix_; /// The final skinned matrix, sent to the shader

    friend class Skeleton;

  };

  inline std::ostream& operator<<(std::ostream& os, const Bone& obj) {
    return os << "Bone - " << obj.name() << " : " << obj.rotation().x << ", " 
      << obj.rotation().y << ", " << obj.rotation().z << ", " << obj.rotation().w;
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
      IndicesType index;
      size_t      count;
    };
    
    /**
     * A weight for a skin. This must link to the skeleton in the current context
     * ID's MD5 uses an offset which we will not use
     */

    struct SkinWeight {
      Bone*       bone;
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
   * 
   */

  class Skeleton {
  public:
    Skeleton() {};
    Skeleton(SkeletonType type);

    Bone* bone(std::string tag);
    Bone* bone(uint id);

    Skeleton& addBone(Bone* b);

    int getBoneIndex(Bone* b);

    /// Get the matrix that corresponds to the skeleton transform.
    glm::mat4 matrix() const {return obj_->matrix; }

    /// Set the world to skeleton transform here
    void set_matrix (const glm::mat4 &m) const {obj_->matrix = m; }

    void update();

    size_t size() {
      std::forward_list<Bone*>::iterator first = obj_->bones.begin();
      std::forward_list<Bone*>::iterator last = obj_->bones.end();
      return std::distance(first,last);
    }

    void resetGlobals();

    std::string toString();

    const std::forward_list<Bone*>& bones() const { return obj_->bones; }
   
    friend std::ostream& operator<<(std::ostream& out, const Skeleton& o);

  protected:

    void createOpenNISkeleton();

    struct SharedObject {
      ~SharedObject();
      Bone* top;
      std::forward_list<Bone*> bones;
      glm::mat4 matrix;

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