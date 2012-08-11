/**
* @brief Camera Manager that deals with the control of the 8 cameras
* @file camera_manager.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/05/2012
*
*/

#include "s9/gl/texture.hpp"

using namespace std;
#ifdef _GEAR_OPENCV
using namespace cv;
#endif

using namespace boost; 
using namespace s9;
using namespace s9::gl;


/*
 * Basic Texture Rectangle Creation with data supplied
 */

Texture::Texture(glm::vec2 size, TextureType format, const char* data) {
  _obj.reset(new SharedObj());
  _obj->_size = size;
  _obj->_format = format;
  glGenTextures(1, &(_obj->_id));
  glBindTexture(GL_TEXTURE_RECTANGLE, _obj->_id);

  switch(format){
    case TEXTURE_RGB:{
      glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 3, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
      break;
    }
    case TEXTURE_INTENSITY:{
      glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 1, size.x, size.y, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
      break;
    }
    default: {
      assert(false);
      break;
    }

  }
}

/*
 * Create a texture from an image
 */

Texture::Texture(Image image){

}

void Texture::update(unsigned char * data) {
 
  bind();

  switch(_obj->_format){
    case TEXTURE_RGB:{
      glTexSubImage2D(GL_TEXTURE_RECTANGLE,0,0,0,
      _obj->_size.x, _obj->_size.y, 
      GL_RGB, GL_UNSIGNED_BYTE, data );    
      break;
    }
    case TEXTURE_INTENSITY:{
     glTexSubImage2D(GL_TEXTURE_RECTANGLE,0,0,0,
      _obj->_size.x, _obj->_size.y, 
      GL_RED, GL_UNSIGNED_BYTE, data );    
      break;
    }
    default: {
      assert(false);
      break;
    }
  }

  
  unbind();
}


/*
 * Bind to Current Texture Unit. Default: 0
 */

void Texture::bind() { glBindTexture(GL_TEXTURE_RECTANGLE, _obj->_id); }

/*
 * Unbind
 */

void Texture::unbind(){ glBindTexture(GL_TEXTURE_RECTANGLE, 0); }


TextureTwo::TextureTwo(glm::vec2 size, TextureType format){

}


TextureTwo::TextureTwo(Image image) {
  
}