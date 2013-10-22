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
     *   
     * \todo binding unit
     *
     */

    class SEBUROAPI Texture{
    public:
      Texture() {};
      Texture(size_t width, size_t height, ColourComponent format=RGB, ColourType type = UNSIGNED_BYTE, const char* data = NULL);
      Texture(const Image &image);

      size_t width() const {return width_; }
      size_t height() const {return height_; }
      GLuint id() const {return id_; };

      ColourType colour_type () const  { return colour_type_; }

      void bind();
      void unbind();

      void update(unsigned char * data);

    protected: 
    
      GLuint id_;
      GLenum gl_type_;
      size_t width_;
      size_t height_;
      ColourComponent format_;
      ColourType colour_type_;

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

    class TextureStream {
    public:
      TextureStream() {};
      TextureStream(size_t width, size_t height, ColourComponent format);
      ~TextureStream();

      GLuint id() const { return id_; };

      void bind();
      void unbind();

      size_t width() const { return width_; }
      size_t height() const { return height_; }


      void update();
      void set_tex_data(void *data);

      void start();
      void stop();

    protected:
      struct SharedObj {
        void *pbo_memory, *tex_data;
      };

      GLuint id_, tex_buffer_;
      size_t width_, height_;
      ColourType colour_type_;
      ColourComponent format_;

      std::shared_ptr <SharedObj> obj_;

    };

  }
}

#endif