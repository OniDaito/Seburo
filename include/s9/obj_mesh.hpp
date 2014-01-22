/**
* @brief OBJ File Format parser
* @file obj_mesh.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 17/01/2014
*
*/

#ifndef S9_OBJ_MESH_HPP
#define S9_OBJ_MESH_HPP

#include "common.hpp"
#include "geometry.hpp"
#include "file.hpp"
#include "math_utils.hpp"
#include "string_utils.hpp"
#include "node.hpp"

namespace s9 {

  class ObjMesh  : public s9::Node {
  public:
    ObjMesh(){}
    ObjMesh (const s9::File &file);

  protected:

    void Parse(const s9::File &file);

    void CreateNodes();
  
    /**
     * A Struct that holds all the information on a mesh-in-potentia, split by material. 
     * It also holds any additional vertices, normals etc that we need to create
     */


    struct TempMesh {

      TempMesh() {  }
      
      // Extra data, created when we need to duplicate. Added on at the end
      std::vector<glm::vec3> vertices;
      std::vector<glm::vec3> normals;
      std::vector<glm::vec2> texcoords;

      // Sizes of the data we would be storing if we werent storing it globally
      IndicesType vertices_size, texcoords_size, normals_size;


      std::vector<IndicesType> indices; // assume triangles for now
      std::vector<IndicesType> texindices;
      std::vector<IndicesType> normalindices;

      Material material;
      gl::Texture texture;

      std::string tag;

  
      
    };

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;

    std::vector<TempMesh> temp_;
    bool using_materials_;

  };


}


#endif