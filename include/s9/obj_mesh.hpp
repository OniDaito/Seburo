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

    void AddObjMesh( std::vector<glm::vec3> &vertices,
      std::vector<glm::vec3> &normals,
      std::vector<glm::vec2> &texcoords,
      std::vector<IndicesType> &indices,
      std::vector<IndicesType> &texindices,
      std::vector<IndicesType> &normalindices,
      std::vector<Material> &materials,
      std::vector<gl::Texture> &textures,
      int mesh_material,
      int mesh_texture,
      size_t &vstart,
      size_t &tstart);
  };
}


#endif