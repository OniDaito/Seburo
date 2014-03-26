/**
* @brief Shader Library for creating shaders on the fly
* @file shader_library.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 24/03/2014
*
*/

#ifndef GL_SHADER_LIBRARY_HPP
#define GL_SHADER_LIBRARY_HPP

#include "common.hpp"
#include "utils.hpp"
#include <map>

namespace s9 {

  namespace gl {

    typedef enum {
      VERTEX_SNIPPET,
      GEOMETRY_SNIPPET,
      FRAGMENT_SNIPPET,
      VERTEX_MAIN,  // MAIN appears in the main func of the shader first thing
      GEOMETRY_MAIN,
      FRAGMENT_MAIN
    } SnippetType;

    typedef struct {
      SnippetType type;
      std::string label;
      std::string text;
    } ShaderSnippet;


    // Class created via the GLFWApp or similar to hold the various libraries. These are loaded
    // from snippets included in the code (or from files should that be needed)
    class ShaderLibrary {

    public:

      ShaderLibrary(){};
      ShaderLibrary(int major_version);

      std::string GetSnippet(std::string name, SnippetType type);

    protected: 
      
      struct SharedObject {

        ShaderSnippet* snippets;
        int num_snippets;

      };

      std::shared_ptr<SharedObject> obj_ = nullptr;

    };
  }
}

#endif