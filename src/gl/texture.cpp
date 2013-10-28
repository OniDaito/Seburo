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
 * \TODO - Need more options! A lot more options! :O
 */

Texture::Texture(size_t width, size_t height, ColourComponent format, ColourType type, const byte_t* data) {
  
  assert (width > 0);
  assert (height > 0);

  width_ = width;
  height_ = height;
  format_ = format;

  colour_type_ = type;

  glGenTextures(1, &(id_));

  // Automatic detection of power of two.

  if (((width_ & (~width_ + 1)) == width_) && ((height_ & (~height_ + 1)) == height_))
    gl_type_ = GL_TEXTURE_2D;
  else 
    gl_type_ = GL_TEXTURE_RECTANGLE;

  GLenum tt = GL_UNSIGNED_BYTE;

  switch (type){
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


  glBindTexture(gl_type_, id_);
  
  switch(format){
    case RGB:
      glTexImage2D(gl_type_, 0, GL_RGB, width, height, 0, GL_RGB, tt, data);
      break;
    
    case RED:
    case GREY:
      glTexImage2D(gl_type_, 0, GL_RED, width, height, 0, GL_RED, tt, data);
      break;

    case BGR:
      glTexImage2D(gl_type_, 0, GL_RGB, width, height, 0, GL_BGR, tt, data);
    break;

    case BGRA:
      glTexImage2D(gl_type_, 0, GL_RGBA, width, height, 0, GL_BGRA, tt, data);
    break;
    
    case RGBA:
      glTexImage2D(gl_type_, 0, GL_RGBA, width, height, 0, GL_RGBA, tt, data);
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

Texture::Texture(const Image &image){
  assert (image.width() > 0);
  assert (image.height() > 0);

  width_ = image.width();
  height_ = image.height();
  format_ = RGB; ///\todo assuming RGB here :S

  glGenTextures(1, &(id_));
  CXGLERROR

  // Automatic detection of power of two.
  if (((width_ & (~width_ + 1)) == width_) && ((height_ & (~height_ + 1)) == height_))
    gl_type_ = GL_TEXTURE_2D;
  else 
    gl_type_ = GL_TEXTURE_RECTANGLE;

  ///\todo This affects global texture state :S
  glPixelStorei(GL_UNPACK_ROW_LENGTH, width_);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glBindTexture(gl_type_, id_);

  ///\todo BGRA or RGBA or what?
  ///\todo also could do with the datatype as well
  GLenum tc = GL_UNSIGNED_BYTE;
  colour_type_ = image.colour_type();

  switch (image.colour_type()) {
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
  CXGLERROR

  switch (image.component()){
    case RGB:
      glTexImage2D(gl_type_, 0, GL_RGB, width_, height_, 0, GL_RGB, tc, image.image_data());
    break;
    case RGBA:
      glTexImage2D(gl_type_, 0, GL_RGBA, width_, height_, 0, GL_RGBA, tc, image.image_data());
    break;
    case BGR:
      glTexImage2D(gl_type_, 0, GL_RGB, width_, height_, 0, GL_BGR, tc, image.image_data());
    break;
    case BGRA:
      glTexImage2D(gl_type_, 0, GL_RGBA, width_, height_, 0, GL_BGRA, tc, image.image_data());
    break;
    case RED:
    case GREY:
      glTexImage2D(gl_type_, 0, GL_RED, width_, height_, 0, GL_RED, tc, image.image_data());
    break;
  }

  CXGLERROR

}

void Texture::update(byte_t * data) {
 
  bind();

  GLenum tc  = GL_UNSIGNED_BYTE;
  switch (colour_type_) {
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


  switch(format_){
    case RGB:{
      glTexSubImage2D(gl_type_, 0, 0, 0, width_, height_, GL_RGB, tc, data );    
      break;
    }
    case GREY:{
     glTexSubImage2D(gl_type_, 0, 0, 0, width_, height_,  GL_RED, tc, data );    
      break;
    }
    case RGBA: {
     glTexSubImage2D(gl_type_, 0, 0, 0, width_, height_, GL_RGBA, tc, data );    
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
 * Bind to Current Texture Unit. Default: 0
 */

void Texture::bind() { glBindTexture(gl_type_, id_); }

/*
 * Unbind
 */

void Texture::unbind(){ glBindTexture(gl_type_, 0); }


/*
 * Texture Streamer with a pointer to some memory we stream in
 * \todo eventually pass in an image that can change (when images replace byte_t arrays)
 */

TextureStream::TextureStream(size_t width, size_t height, ColourComponent format, 
    ColourType type, const byte_t* data) : Texture(width,height,format,type,data),
    obj_( shared_ptr<SharedObj> (new SharedObj())){

  bind();

  glGenBuffers(1, &(tex_buffer_));
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, tex_buffer_);
  int texsize = width_ * height_;

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

  int texsize = width_ * height_;
  bind();

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, tex_buffer_);

  obj_->pbo_memory = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
  CXGLERROR

  GLenum tc  = GL_UNSIGNED_BYTE;
  switch (colour_type_) {
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


  switch (format_){
    
    case RGBA:
    case BGRA:
      memcpy(obj_->pbo_memory, data, texsize * 4);
      CXGLERROR
      glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
      glTexSubImage2D(gl_type_, 0, 0, 0, width_, height_,GL_RGBA, tc, 0);
      break;
    
    case RGB:
    case BGR:
      memcpy(obj_->pbo_memory, data, texsize * 3);
      CXGLERROR
      glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
      glTexSubImage2D(gl_type_, 0, 0, 0, width_, height_, GL_RGB, tc, 0);

      CXGLERROR
      break;
    
    case GREY:
    case RED:
      memcpy(obj_->pbo_memory, data, texsize);
      glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
      glTexSubImage2D(gl_type_, 0, 0, 0, width_, height_, GL_RED, tc, 0);

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

TextureStream::~TextureStream() {

}

