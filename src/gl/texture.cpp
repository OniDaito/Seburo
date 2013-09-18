/**
* @brief Basic Textures
* @file texture.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/05/2012
*
*/

#include "s9/gl/texture.hpp"

using namespace std;
#ifdef _SEBURO_OPENCV
using namespace cv;
#endif

using namespace boost; 
using namespace s9;
using namespace s9::gl;


/*
 * Basic Texture Rectangle Creation with data supplied
 * \TODO - Need more options! A lot more options! :O
 */

Texture::Texture(glm::vec2 size, TextureType format, const char* data) {
  _obj.reset(new SharedObj());
  _obj->_size = size;
  _obj->_format = format;

  glGenTextures(1, &(_obj->_id));
  CXGLERROR

  glBindTexture(GL_TEXTURE_RECTANGLE, _obj->_id);
  CXGLERROR
  switch(format){
    case TEXTURE_RGB:{
      glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
      CXGLERROR
      break;
    }
    case TEXTURE_GREY:{
      glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RED, size.x, size.y, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
      break;
    }
    default: {
      assert(false);
      break;
    }

  }
  CXGLERROR
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
    case TEXTURE_GREY:{
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

/*
 * Texture Streamer with a PBO
 */

TextureStream::TextureStream(glm::vec2 size, TextureType format){

  _obj.reset(new SharedObj());
  _obj->_size = size;
  _obj->_format = format;
 
  glGenTextures(1, &(_obj->_id));
  glBindTexture(GL_TEXTURE_RECTANGLE, _obj->_id);

  glGenBuffers(1, &(_obj->_texBuffer));
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _obj->_texBuffer);
  int texsize = _obj->_size.x * _obj->_size.y;

  switch (_obj->_format){
    case TEXTURE_RGBA:{
     
      glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 4, size.x, size.y, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, NULL);

      glBufferData(GL_PIXEL_UNPACK_BUFFER, texsize * 4, NULL,
          GL_STREAM_DRAW);
      break;
    }
    case TEXTURE_RGB:{
     
      glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 3, size.x, size.y, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, NULL);

      glBufferData(GL_PIXEL_UNPACK_BUFFER, texsize * 3, NULL,
          GL_STREAM_DRAW);
      break;
    }
    case TEXTURE_GREY: {
      glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 1, size.x, size.y, 0,
                     GL_RED, GL_UNSIGNED_BYTE, NULL);

      glBufferData(GL_PIXEL_UNPACK_BUFFER, texsize, NULL, GL_STREAM_DRAW);
      break;
    }
    default:{
      assert(false);
      break;
    }
  }
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  glBindTexture(GL_TEXTURE_RECTANGLE, 0);

  CXGLERROR

}

void TextureStream::setData(void *data){
  _obj->_texData = data;
}

void TextureStream::start() {
  CXGLERROR
}

void TextureStream::stop() {
   CXGLERROR
}

void TextureStream::update(){
  int texsize = _obj->_size.x * _obj->_size.y;
  bind();
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _obj->_texBuffer);

  _obj->_pboMemory = glMapBuffer(GL_PIXEL_UNPACK_BUFFER,
                          GL_WRITE_ONLY);
  CXGLERROR
  switch (_obj->_format){
    case TEXTURE_RGBA:{
      memcpy(_obj->_pboMemory, _obj->_texData, texsize * 4);
      CXGLERROR
      glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
      glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, 0, 
        _obj->_size.x, 
        _obj->_size.y,
        GL_RGBA, GL_UNSIGNED_BYTE, 0);
      break;
    }
    case TEXTURE_RGB:{
      memcpy(_obj->_pboMemory, _obj->_texData, texsize * 3);
      CXGLERROR
      glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
      glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, 0, 
        _obj->_size.x, 
        _obj->_size.y,
        GL_RGB, GL_UNSIGNED_BYTE, 0);

      CXGLERROR
      break;
    }
    case TEXTURE_GREY: {
      memcpy(_obj->_pboMemory, _obj->_texData, texsize);
      glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
      glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, 0, 
        _obj->_size.x, 
        _obj->_size.y,
        GL_RED, GL_UNSIGNED_BYTE, 0);

      CXGLERROR
      break;
    }
    default:{
      assert(false);
      break;
    }
  }

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  unbind();
  CXGLERROR
}

TextureStream::~TextureStream() {

}


void TextureStream::bind() { glBindTexture(GL_TEXTURE_RECTANGLE, _obj->_id); }

void TextureStream::unbind(){ glBindTexture(GL_TEXTURE_RECTANGLE, 0); }