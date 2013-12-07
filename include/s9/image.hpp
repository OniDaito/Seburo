/**
* @brief Underlying Image loading code - relies on OS libs (GDI+, CoreImage, SDL_Image? )
* @file image.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 21/10/2013
*
*/

///\todo Potentially the GLM Chap has a small lib for enhanced texture loading for OpenGL speedy! :D

#ifndef S9_IMAGE
#define S9_IMAGE

#include "s9/common.hpp"
#include "s9/file.hpp"

#ifdef _SEBURO_OSX
#include "QuartzCore/QuartzCore.h"
#endif

#ifdef _SEBURO_LINUX
#include "soil/SOIL.h"
#endif


namespace s9{

  // http://www.opengl.org/sdk/docs/man/xhtml/glTexImage2D.xml
  // Lots of types :)

  typedef enum {
    RED,
    GREY,
    RGB,
    BGR,
    RGBA,
    BGRA,
    // DEPTH ///\todo GL_DEPTH_COMPONENT and GL_DEPTH_STENCIL?
  }ColourComponent;

  typedef enum {
    UNSIGNED_BYTE,
    FLOAT
  } ColourType;


  class Image {
  public:
    Image() {}


    Image(const File &f) : obj_( std::shared_ptr<SharedObj> (new SharedObj())){
      obj_->image_data = imageReference( f.path().c_str() );
    }

    byte_t * image_data() const { return obj_->image_data;}

    size_t width() const {return width_;}
    size_t height() const {return height_;}
    size_t bits_per_component () const {return bits_per_component_;}
    size_t bits_per_pixel() const {return bits_per_pixel_;}
    size_t bytes_per_row() const {return bytes_per_row_;}
    ColourComponent component() const { return component_; }
    ColourType colour_type() const {return colour_type_;}

  protected:

    byte_t * imageReference (const char *imageName);

    struct SharedObj{
      SharedObj() {
        image_data = nullptr;
      }

      ~SharedObj() {
        delete[] image_data;
      }

      byte_t * image_data;
    };

    std::shared_ptr<SharedObj> obj_;

    // Outside of the shared pointer as they are easy copy. Good idea?
    size_t width_, height_;
    size_t bits_per_component_;
    size_t bits_per_pixel_;
    size_t bytes_per_row_;
    ColourComponent component_;
    ColourType colour_type_;

  };



}

#endif