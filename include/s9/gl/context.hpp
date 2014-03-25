/**
* @brief Context for OpenGL 
* @file context.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 24/03/2014
*
*/

#ifndef GL_CONTEXT_HPP
#define GL_CONTEXT_HPP

#include "common.hpp"
#include <memory>
#include <string>
#include <iostream>

#include "../context.hpp"

namespace s9 {

  namespace gl {

    /**
     * 
     *
     * \TODO - eventually have stuff like depthbits etc
     */

    class OpenGLContext : public Context { 
    public: 

      OpenGLContext() : Context(true)  { 
        
        glGetIntegerv(GL_MAJOR_VERSION, &obj_->major_version);
        glGetIntegerv(GL_MINOR_VERSION, &obj_->minor_version);

        const GLubyte* version = glGetString(GL_VERSION);
        obj_->version_string = version ? (const char*) version : "";

        const GLubyte* shader_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
        obj_->shader_version = shader_version ? (const char*) shader_version : "";

        std::cout << "SEBURO OpenGL Version: " << obj_->major_version << "," << obj_->minor_version << std::endl;
        std::cout << "SEBURO OpenGL Shader Version: " << obj_->shader_version << std::endl;
      }

      void set_major_version(GLint m) { obj_->major_version = m; }
      void set_minor_version(GLint m) { obj_->minor_version = m; }
      void set_shader_version(const GLubyte* b) { obj_->shader_version = std::string( reinterpret_cast<const char *>(b) ); }
    };
  }
}


#endif