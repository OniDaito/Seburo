 /**
* @brief OpenGL Text rendering wih bitmap fonts and freetype
* @file text.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 27/03/2014
*
*/


#ifndef S9_GL_TEXT_HPP
#define S9_GL_TEXT_HPP


#include <ft2build.h> 
#include FT_FREETYPE_H

#include "common.hpp"
#include "../node.hpp"
#include "../file.hpp"

// https://axfive.wordpress.com/2012/01/17/freetype-texture-fonts-in-opengl/

namespace s9 {

  namespace gl {

    class Text : public s9::Node {

    public:
      Text() : Node() {}
      Text(s9::File font_file);

    protected:

      static FT_Library library_;

      struct SharedObject : public Node::SharedObject {
        
      };

      // All Freetype - lets hope they copy easy enough! :S
      FT_Face face_;
      FT_GlyphSlot slot_;
      FT_UInt glyph_index_;
      FT_Vector pen_;
      FT_Error error_;

    };

  }
}
  
#endif