/**
* @brief Texture class for OpenGL and similar
* @file texture.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef S9_TEXTURE_HPP
#define S9_TEXTURE_HPP

#include "s9/common.hpp"
#include "s9/gl/common.hpp"
#include "s9/gl/utils.hpp"
#include "s9/image.hpp"

#ifdef _SEBURO_OPENCV
#include <opencv2/opencv.hpp>
#endif

namespace s9 {

  namespace gl {

    /*
     * Represents a texture in OpenGL. Use GL_TEXTURE_RECTANGLE or GL_TEXTURE depending on power of 2
     * \todo we need to think about this power of two stuff as it affects the shader
     */

    class SEBUROAPI Texture{
    public:
      Texture() {};
      Texture(size_t width, size_t height, ColourComponent format=RGB, ColourType type = UNSIGNED_BYTE, int unit = 0, const byte_t* data = nullptr);
      Texture(const Image &image, int unit = 0);


      size_t width() const {return obj_->width; }
      size_t height() const {return obj_->height; }
      GLuint id() const {return obj_->id; };
      int unit() const { return obj_->unit;}

      void set_unit(int u) {obj_->unit = u;}

      ColourType colour_type () const  { return obj_->colour_type; }

      GLenum gl_type() {return obj_->gl_type; }
      GLenum basic_type() {return obj_->basic_type; }

      void Bind();
      void Unbind();

      void Resize(size_t w, size_t h);

      void Update(byte_t * data);

    protected: 
    
      struct SharedObject {

        SharedObject(size_t w, size_t h, ColourComponent f, ColourType t, int u, const byte_t* d);

        ~SharedObject();

        GLuint id, storage_type, colour_type_a, colour_type_b;
        GLenum gl_type, basic_type;
        size_t width;
        size_t height;
        int unit;
        ColourComponent format;
        ColourType colour_type;

      };

      std::shared_ptr <SharedObject> obj_;

    
    public:
      bool operator == (const Texture &ref) const { return this->obj_ == ref.obj_; }
      typedef std::shared_ptr<SharedObject> Texture::*unspecified_bool_type;
      operator unspecified_bool_type() const { return ( obj_.get() == 0 ) ? 0 : &Texture::obj_; }
      void reset() { obj_.reset(); }

    };

    /**
     * Useful function for OpenCV Mat to Unsigned Char *
     */

#ifdef _SEBURO_OPENCV
    inline unsigned char * MatToGL(cv::Mat &mat) {
      return (unsigned char *)IplImage(mat).imageData; 
    };
#endif

    /*
     * Streaming texture with a PBO. This need to be started,
     * updated and then stopped. Potentially could be merged - inherit above
     */

    class SEBUROAPI TextureStream : public Texture{
    public:
      TextureStream() {};
      TextureStream(size_t w, size_t h, ColourComponent f=RGB, ColourType t = UNSIGNED_BYTE, int u = 0, const byte_t* d = nullptr);

      void Update(byte_t *data);
    
    protected:
      struct SharedObject : public Texture::SharedObject {
        SharedObject(size_t w, size_t h, ColourComponent f, ColourType t, int u, const byte_t* d);
        
        ~SharedObject() {
          //delete pbo_memory;
          if (tex_data != nullptr)
            delete tex_data;
        }

        byte_t *tex_data;
        GLvoid *pbo_memory;
        GLuint tex_buffer;
      
      };


    public:
      typedef std::shared_ptr<SharedObject> TextureStream::*unspecified_bool_type;

    };

  }
}

#endif