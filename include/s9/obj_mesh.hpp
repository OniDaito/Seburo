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

#include <set>

namespace s9 {

  class ObjMesh  : public s9::Node {
  public:
    ObjMesh(){}
    ObjMesh (const s9::File &file);

    SharedTriMesh mesh_;

  protected:

    void Parse(const s9::File &file);
 

    /// A struct that represents a unique vertex

    struct ObjVert{
      IndicesType p,t,n,idx;

      const bool operator<(const ObjVert& b) const {
        return (p < b.p || (p == b.p &&  t < b.t) || ( p == b.p && t == b.t && n < b.n));
      }

    };

  
    bool CompareObjVert(const ObjVert &a, const ObjVert &b) {
      return (a.p < b.p || (a.p == b.p &&  a.t < b.t) || ( a.p == b.p && a.t == b.t && a.n < b.n));
    }

    // A struct that represents a triangle
    struct ObjFace {
      std::set<ObjVert>::iterator vertices[3];
    };

    /**
     * A Struct that holds all the information on a mesh-in-potentia, split by material. 
     * It also holds any additional vertices, normals etc that we need to create
     */


    struct TempMesh {

      TempMesh() {  }
      
      Material material;
      gl::Texture texture;

      std::string tag;

    };

    // This should still obey the shared object principles more or less as these
    // can all be copied easily and are mostly blank anyway with the exception of
    // soup_ which must remain and is shared (I hope!)

  

    std::vector<TempMesh> temp_;
    bool using_materials_;

    VertexSoup soup_;

  };


}


#endif