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

  assert (width > 0);
  assert (height > 0);

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

  glGenTextures(1, &(id));

  cout << "Tex ID: " << id << endl;

  // Automatic detection of power of two.

  if (((width & (~width + 1)) == width) && ((height & (~height + 1)) == height))
    gl_type = GL_TEXTURE_2D;
  else 
    gl_type = GL_TEXTURE_RECTANGLE;

  GLenum tt = GL_UNSIGNED_BYTE;

  switch (t){
    case UNSIGNED_BYTE:
      tt = GL_UNSIGNED_BYTE;
    break;

    case FLOAT: 
      tt = GL_FLOAT;
    break;

    default:
      tt = GL_UNSIGNED_BYTE;
    break;
  }


  glBindTexture(gl_type, id);
  
  switch(f){
    case RGB:
      glTexImage2D(gl_type, 0, GL_RGB, width, height, 0, GL_RGB, tt, d);
      break;
    
    case RED:
    case GREY:
      glTexImage2D(gl_type, 0, GL_RED, width, height, 0, GL_RED, tt, d);
      break;

    case BGR:
      glTexImage2D(gl_type, 0, GL_RGB, width, height, 0, GL_BGR, tt, d);
    break;

    case BGRA:
      glTexImage2D(gl_type, 0, GL_RGBA, width, height, 0, GL_BGRA, tt, d);
    break;
    
    case RGBA:
      glTexImage2D(gl_type, 0, GL_RGBA, width, height, 0, GL_RGBA, tt, d);
      break;
    
    default:
      cerr << "SEBURO ERROR - No Format specified for texture." << endl;
      assert(false);
      break;

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

void Texture::bind() { glActiveTexture(GL_TEXTURE0 + obj_->unit); glBindTexture(obj_->gl_type, obj_->id); std::cout << "Binding: " << obj_->id << std::endl;}

/*
 * Unbind
 */

void Texture::unbind(){ glActiveTexture(GL_TEXTURE0 + obj_->unit); glBindTexture(obj_->gl_type, 0); }



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

  obj_ = shared_ptr<SharedObject>(new SharedObject(width,height,format,type,unit,data));

  bind();

  glGenBuffers(1, &(obj_->tex_buffer));
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, obj_->tex_buffer);
  int texsize = obj_->width * obj_->height;

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

  int texsize = obj_->width * obj_->height;
  bind();

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, obj_->tex_buffer);

  obj_->pbo_memory = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
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
      memcpy(obj_->pbo_memory, data, texsize * 4);
      CXGLERROR
      glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
      glTexSubImage2D(obj_->gl_type, 0, 0, 0, obj_->width, obj_->height,GL_RGBA, tc, 0);
      break;
    
    case RGB:
    case BGR:
      memcpy(obj_->pbo_memory, data, texsize * 3);
      CXGLERROR
      glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
      glTexSubImage2D(obj_->gl_type, 0, 0, 0, obj_->width, obj_->height, GL_RGB, tc, 0);

      CXGLERROR
      break;
    
    case GREY:
    case RED:
      memcpy(obj_->pbo_memory, data, texsize);
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

