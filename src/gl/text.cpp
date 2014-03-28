/**
* @brief Text Rendering in OpenGL
* @file text.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 27/03/2014
*
*/

#include "s9/gl/text.hpp"

using namespace std;
using namespace s9;
using namespace s9::gl;


FT_Library Text::library_;


Text::Text(s9::File font_file) {

  FT_Face face; 
  error_ = FT_Init_FreeType( &library_ ); 
  if ( error_ ) { 
  } 
  
  error_ = FT_New_Face( library_, font_file.final_path().c_str(), 0, &face );
  if ( error_ == FT_Err_Unknown_File_Format ) {
    
  } else if ( error_ ) { 
    
  } 
} 
