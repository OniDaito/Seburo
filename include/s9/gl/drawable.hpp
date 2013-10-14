/**
* @brief DrawableClass. Takes geometry and creates OpenGL
* @file drawable.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/10/2013
*
*/

#ifndef S9_GL_DRAWABLE_HPP
#define S9_GL_DRAWABLE_HPP

#include "../common.hpp"
#include "common.hpp"
#include "utils.hpp"
#include "../geometry.hpp"


namespace s9{
  
  namespace gl {

      /// BufferRole - what each buffer actually represents. Per Vertex buffers.
      typedef enum {
        INTERLEAVED_BUFFER,
        INDEX_BUFFER,
        POSITION_BUFFER,
        COLOUR_BUFFER,
        NORMAL_BUFFER,
        UV_BUFFER,
        TANGENT_BUFFER
      } BufferRole;


    /**
     * DrawableBase - this provides an interface for other classes to play with DrawableT
     * It is composed with Node to make the Drawable Node (DNode)
     */

    class Drawable {
    public:

      /// Options for brewing that may need to be specified
      struct BrewFlags {
        bool interleaved = true;
        GLint access = GL_STATIC_DRAW;
      };

      virtual void draw() {};
      virtual void brew(BrewFlags b) {};
      virtual void test() {};
    };

  }

}


#endif