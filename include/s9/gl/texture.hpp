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
#include "s9/image.hpp"

#ifdef _GEAR_OPENCV
#include <opencv2/opencv.hpp>
#endif

namespace s9 {

  namespace gl {

    typedef enum {
      TEXTURE_RGB,
      TEXTURE_INTENSITY,
      TEXTURE_RGBA
    }TextureType;

    /*
     * Represents a texture in OpenGL. Use GL_TEXTURE_RECTANGLE
     */

    class Texture{
    public:
      Texture() {};
      Texture(glm::vec2 size, TextureType format=TEXTURE_RGB, const char* data = NULL);
      Texture(uint32_t w, uint32_t h, TextureType format=TEXTURE_RGB) { Texture(glm::vec2(w,h),format); };
      Texture(Image image);

      GLuint id() { return _obj->_id; };

      void bind();
      void unbind();

      void update(unsigned char * data);

    protected: 
      struct SharedObj {
        GLuint _id;
        glm::vec2 _size;
        TextureType _format;
      };

      boost::shared_ptr <SharedObj> _obj;

    public:
      glm::vec2 getSize() {return _obj->_size;};
      glm::vec2 size() {return getSize(); };
      GLuint getTexture() {return _obj->_id; };
    };

    /*
     * Represents a power of two texture with assert checks
     */

    class TextureTwo : public Texture{
    public:
      TextureTwo() {};
      TextureTwo(glm::vec2 size,TextureType format=TEXTURE_RGB);
      TextureTwo(uint32_t w, uint32_t h,TextureType format=TEXTURE_RGB) { TextureTwo(glm::vec2(w,h),format); };
      TextureTwo(Image image);       
    };

#ifdef _GEAR_OPENCV
    inline unsigned char * MatToGL(cv::Mat &mat) {
      return (unsigned char *)IplImage(mat).imageData; 
    };

#endif
  }
}

#endif