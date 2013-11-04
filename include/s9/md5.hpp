/**
* @brief MD5 Parsing Class
* @file md5.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 29/10/2013
*
*/



#ifndef S9_MD5_HPP
#define S9_MD5_HPP

#include "s9/primitives.hpp"
#include "s9/node.hpp"
#include "s9/file.hpp"
#include "s9/skeleton.hpp"
#include "common.hpp"


namespace s9 {


  /// Defines a skin weight for vertices
  typedef struct {
    glm::vec3   position;
    float       bias;
    Bone*       bone;
  } SkinWeight;

  /// Given a vertex we want the weight attached
  typedef struct {
    IndicesType index;
    size_t      count;
  } SkinWeightIndex;

  /**
   * MD5 takes a file representing an MD5 and creates an array of s9::nodes we can draw
   */

  class MD5Model : public s9::Node {
  public:

    MD5Model() {}
    MD5Model(const s9::File &file);
    
    friend std::ostream& operator<<(std::ostream& out, const MD5Model& o);

    int version() const {return obj_->version; }
    size_t num_joints() const {return obj_->num_joints; }
    size_t num_meshes() const {return obj_->num_meshes; }
    const std::vector<size_t> num_verts() const {return obj_->num_verts; }

  protected:

    void parse(const s9::File &file);

    struct SharedObject {
      
      std::string           filename;
      int                   version;
      size_t                num_joints;
      size_t                num_meshes;
      std::vector<size_t>   num_verts;
      std::vector<size_t>   num_tris;
      std::vector<size_t>   num_weights;

      s9::Skeleton  skeleton;
    };

    std::shared_ptr<SharedObject> obj_ = nullptr;

  };

  /// friend function that actually prints an MD5 Mesh for debugging or other purposes
  inline std::ostream& operator<<(std::ostream& os, const MD5Model& obj) {
    if (obj.obj_ != nullptr){
      os << "SEBURO MD5Model - Version: " << obj.version() << std::endl;
      os << "   Num Joints: " << obj.num_joints() << std::endl;
      os << "   Num Meshes: " << obj.num_meshes() << std::endl;
      for (int i =0; i < obj.num_meshes(); ++i)
        os << "       Num Verts " << i << ": " << obj.num_verts()[i] << std::endl;
      return os;
    }
     
  
    return  os << "SEBURO MD5Model not Loaded." << std::endl;
  }

}


#endif