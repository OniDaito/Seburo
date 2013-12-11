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

using namespace s9;
using namespace s9::gl;


/*
 * Basic Texture Rectangle Creation with data supplied
 * \todo - Need more options! A lot more options! :O
 */

Texture::Texture(size_t width, size_t height, ColourComponent format, ColourType type, int unit, const byte_t* data) 
  : obj_ (shared_ptr<SharedObject>(new SharedObject(width, height, format, type, unit, data))){
 
}

Texture::SharedObject::SharedObject(size_t w, size_t h, ColourComponent f, ColourType t, int u, const byte_t* d) {

  assert (w > 0);
  assert (h > 0);

  width = w;
  height = h;
  format = f;
  unit = u;

  /*GLint ogl = 0;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &ogl);
  if (unit_ > ogl){
    cerr << "SEBURO TEXTURE Error - Unit is greater than GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS." << endl;
  }*/

  colour_type = t;
  
  glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(1, &(id));
  
  // Automatic detection of power of two.

  if (((width & (~width + 1)) == width) && ((height & (~height + 1)) == height))
    gl_type = GL_TEXTURE_2D;
  else 
    gl_type = GL_TEXTURE_RECTANGLE;

  basic_type = GL_UNSIGNED_BYTE;

  glBindTexture(gl_type, id);

  ///\todo we need a really nice way to pass these parameters
  glTexParameterf(gl_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(gl_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(gl_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(gl_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  switch (t){
    case UNSIGNED_BYTE:
      basic_type = GL_UNSIGNED_BYTE;
    break;

    case FLOAT: 
      basic_type = GL_FLOAT;
    break;

    default:
      basic_type = GL_UNSIGNED_BYTE;
    break;
  }
  
  colour_type_a = GL_RGB;
  colour_type_b = GL_RGB;

  storage_type = GL_RGB8;



  switch(f){
    case RGB:
      colour_type_a = colour_type_b = GL_RGB;
      if (t == UNSIGNED_BYTE)
        storage_type = GL_RGB8; ///\todo signed or unsigned here ? :S
      if (t == FLOAT)
        storage_type = GL_RGB32F; ///\todo is FLOAT always 32 bit? Cx that
      break;
    
    case RED:
    case GREY:
      colour_type_a = colour_type_b = GL_RED;
      if (t == UNSIGNED_BYTE)
        storage_type = GL_R8;
      if (t == FLOAT)
        storage_type = GL_R32F; 
      break;

    case BGR:
      colour_type_b = GL_BGR;
      if (t == UNSIGNED_BYTE)
        storage_type = GL_RGB8;
      if (t == FLOAT)
        storage_type = GL_RGB32F; 
      break;

    case BGRA:
      colour_type_a = GL_RGBA; colour_type_b = GL_BGRA;
      if (t == UNSIGNED_BYTE)
        storage_type = GL_RGBA8;
      if (t == FLOAT)
        storage_type = GL_RGBA32F; 
      break;
    
    case RGBA:
      colour_type_a = colour_type_b = GL_RGBA;
      if (t == UNSIGNED_BYTE)
        storage_type = GL_RGBA8;
      if (t == FLOAT)
        storage_type = GL_RGBA32F; 
      break;
    
    default:
      cerr << "SEBURO ERROR - No Format specified for texture." << endl;
      assert(false);
      break;
  }

  // Copy data if no nullptr
  // Specify storage if a 2D type.
  ///\ todo potential for passing mipmap levels?
  

  if (d != nullptr) {
    glTexStorage2D(gl_type, 1, storage_type, width, height);
    glTexSubImage2D(gl_type, 0, 0, 0, width, height, colour_type_b, basic_type, d);
  } else {
    glTexImage2D(gl_type, 0, storage_type, width, height, 0, colour_type_b, basic_type, NULL);
  }
  CXGLERROR
}

/*
 * Create a texture from an image
 */

Texture::Texture(const Image &image, int unit){
  assert (image.width() > 0);
  assert (image.height() > 0);

  obj_ = shared_ptr<SharedObject>(new SharedObject(image.width(), image.height(), 
      image.component(), image.colour_type(), unit, image.image_data()));

  CXGLERROR

}

Texture::SharedObject::~SharedObject() {
  if (id != 0) glDeleteTextures( 1, &id );
}


void Texture::update(byte_t * data) {
 
  bind();

  GLenum tc  = GL_UNSIGNED_BYTE;
  switch (obj_->colour_type) {
    case UNSIGNED_BYTE:
      tc = GL_UNSIGNED_BYTE;
    break;

    case FLOAT:
      tc = GL_FLOAT;
    break;

    default:
      tc = GL_UNSIGNED_BYTE;
    break;
  }


  switch(obj_->format){
    case RGB:{
      glTexSubImage2D(obj_->gl_type, 0, 0, 0, obj_->width, obj_->height, GL_RGB, tc, data );    
      break;
    }
    case GREY:{
     glTexSubImage2D(obj_->gl_type, 0, 0, 0, obj_->width, obj_->height,  GL_RED, tc, data );    
      break;
    }
    case RGBA: {
     glTexSubImage2D(obj_->gl_type, 0, 0, 0, obj_->width, obj_->height, GL_RGBA, tc, data );    
      break;
    }
    default: {
      cerr << "SEBURO ERROR - No Format specified for texture update." << endl;
      assert(false);
      break;
    }
  }
  
  unbind();
}


/*
 * Bind to texture unit.
 */

void Texture::bind() { glActiveTexture(GL_TEXTURE0 + obj_->unit); glBindTexture(obj_->gl_type, obj_->id);}


/*
 * Unbind
 */

void Texture::unbind(){ glActiveTexture(GL_TEXTURE0 + obj_->unit); glBindTexture(obj_->gl_type, 0); }

/// Resize a texture in memory - used mostly with FBOs. Clears all data

void Texture::resize(size_t w, size_t h) {
  if (obj_ == nullptr) return;

  obj_->width = w;
  obj_->height = h;

  bind();
  glTexImage2D(obj_->gl_type, 0, obj_->storage_type, obj_->width, obj_->height, 0, obj_->colour_type_b, obj_->basic_type, NULL);

  unbind();

  CXGLERROR
}


TextureStream::SharedObject::SharedObject(size_t w, size_t h, ColourComponent f, 
    ColourType t, int u, const byte_t* d) : Texture::SharedObject(w, h, f, t, u, d) {
  pbo_memory = nullptr;
  tex_data = nullptr;
}

/*
 * Texture Streamer with a pointer to some memory we stream in
 * \todo eventually pass in an image that can change (when images replace byte_t arrays)
 */


TextureStream::TextureStream(size_t width, size_t height, ColourComponent format, 
    ColourType type, int unit, const byte_t* data) : Texture() {

  obj_ = shared_ptr<SharedObject>(new TextureStream::SharedObject(width,height,format,type,unit,data));

  shared_ptr<TextureStream::SharedObject> tsobj = std::static_pointer_cast<TextureStream::SharedObject>(obj_);

  bind();

  glGenBuffers(1, &(tsobj->tex_buffer));
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, tsobj->tex_buffer);
  int texsize = tsobj->width * tsobj->height;

  ///\todo assuming unsigned byte?
  switch (format){
    case RGBA:
    case BGRA:
      glBufferData(GL_PIXEL_UNPACK_BUFFER, texsize * 4, NULL, GL_STREAM_DRAW);
      break;
    
    case RGB:
    case BGR:
      glBufferData(GL_PIXEL_UNPACK_BUFFER, texsize * 3, NULL, GL_STREAM_DRAW);
      break;

    case GREY: 
    case RED:
      glBufferData(GL_PIXEL_UNPACK_BUFFER, texsize, NULL, GL_STREAM_DRAW);
      break;
  
    default:
      cerr << "SEBURO ERROR - No Format specified for texturestream." << endl;
      assert(false);
      break;

  }

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

  unbind();

  CXGLERROR

}


void TextureStream::update(byte_t *data){

  if (data == nullptr)
    return;

  shared_ptr<TextureStream::SharedObject> tsobj = std::static_pointer_cast<TextureStream::SharedObject>(obj_);
  int texsize = obj_->width * obj_->height;
  
  bind();

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, tsobj->tex_buffer);

  tsobj->pbo_memory = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
  CXGLERROR

  GLenum tc  = GL_UNSIGNED_BYTE;
  switch (obj_->colour_type) {
    case UNSIGNED_BYTE:
      tc = GL_UNSIGNED_BYTE;
    break;

    case FLOAT:
      tc = GL_FLOAT;
    break;

    default:
      tc = GL_UNSIGNED_BYTE;
    break;
  }

  switch (obj_->format){
    
    case RGBA:
    case BGRA:
      memcpy(tsobj->pbo_memory, data, texsize * 4);
      CXGLERROR
      glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
      glTexSubImage2D(obj_->gl_type, 0, 0, 0, obj_->width, obj_->height,GL_RGBA, tc, 0);
      break;
    
    case RGB:
    case BGR:
      memcpy(tsobj->pbo_memory, data, texsize * 3);
      CXGLERROR
      glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
      glTexSubImage2D(obj_->gl_type, 0, 0, 0, obj_->width, obj_->height, GL_RGB, tc, 0);

      CXGLERROR
      break;
    
    case GREY:
    case RED:
      memcpy(tsobj->pbo_memory, data, texsize);
      glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
      glTexSubImage2D(obj_->gl_type, 0, 0, 0, obj_->width, obj_->height, GL_RED, tc, 0);

      CXGLERROR
      break;

    default:
      assert(false);
      break;
    
  }

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  unbind();
  CXGLERROR
}

