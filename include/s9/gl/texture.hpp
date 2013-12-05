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
     * Represents a texture in OpenGL. Use GL_TEXTURE_RECTANGLE
     * was previously a shared object but since it keeps no pointers or data on the CPU it need not be
     * By keeping texture non-shared, we can seperate it from the data allowing us to take copies of the
     * same image with different bind locations
     *   
     * \todo wrap around image a little better perhaps? - Shared object?
     *
     */

    class SEBUROAPI Texture{
    public:
      Texture() {};
      Texture(size_t width, size_t height, ColourComponent format=RGB, ColourType type = UNSIGNED_BYTE, int unit = 0, const byte_t* data = nullptr);
      Texture(const Image &image, int unit = 0);

      size_t width() const {return width_; }
      size_t height() const {return height_; }
      GLuint id() const {return id_; };
      int unit() const { return unit_;}

      void set_unit(int u) {unit_ = u;}

      ColourType colour_type () const  { return colour_type_; }

      void bind();
      void unbind();

      void update(byte_t * data);

    protected: 
    
      GLuint id_;
      GLenum gl_type_;
      size_t width_;
      size_t height_;
      int unit_;
      ColourComponent format_;
      ColourType colour_type_;

    public:
      bool operator == (const Texture &ref) const { return this->id_ == ref.id_; }

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

    class TextureStream : public Texture{
    public:
      TextureStream() {};
      TextureStream(size_t width, size_t height, ColourComponent format=RGB, ColourType type = UNSIGNED_BYTE, int unit = 0, const byte_t* data = nullptr);
      ~TextureStream();

      void update(byte_t *data);
    
    protected:
      struct SharedObj {
        SharedObj() {
          pbo_memory = nullptr;
          tex_data = nullptr;
        }

        byte_t *tex_data;
        GLvoid *pbo_memory;
      };

      GLuint tex_buffer_;
   
      std::shared_ptr <SharedObj> obj_;

    };

  }
}

#endif