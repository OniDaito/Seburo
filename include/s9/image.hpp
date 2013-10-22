/**
* @brief Underlying Image loading code - relies on OS libs (GDI+, CoreImage, SDL_Image? )
* @file image.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 21/10/2013
*
*/

#ifndef S9_IMAGE
#define S9_IMAGE

#include "s9/common.hpp"
#include "s9/file.hpp"

#ifdef _SEBURO_OSX
#include "QuartzCore/QuartzCore.h"
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

#ifdef _SEBURO_OSX
      obj_->image_data = imageReference( f.path().c_str() );
#endif

    }

    unsigned char * image_data() const { return obj_->image_data;}

    size_t width() const {return width_;}
    size_t height() const {return height_;}
    size_t bits_per_component () const {return bits_per_component_;}
    size_t bits_per_pixel() const {return bits_per_pixel_;}
    size_t bytes_per_row() const {return bytes_per_row_;}
    ColourComponent component() const { return component_; }
    ColourType colour_type() const {return colour_type_;}

  protected:

#ifdef _SEBURO_OSX
    unsigned char * imageReference (const char *imageName);
#endif

    struct SharedObj{
      SharedObj() {
        image_data = nullptr;
      }

      ~SharedObj() {
        delete[] image_data;
      }

      unsigned char * image_data;
    };

    std::shared_ptr<SharedObj> obj_;

    size_t width_, height_;
    size_t bits_per_component_;
    size_t bits_per_pixel_;
    size_t bytes_per_row_;
    ColourComponent component_;
    ColourType colour_type_;

  };



}

#endif