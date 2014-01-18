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

      // All these can be copied easily so work as shared objects
      // Note, this class extends a shared object so the obj_ pointer is set to that

    };

}


#endif