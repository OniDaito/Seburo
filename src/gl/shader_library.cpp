/**
* @brief Shader Library File
* @file shader_library.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 24/03/2014
*
*/


#include "s9/gl/shader_library.hpp"

using namespace std;
using namespace s9;
using namespace s9::gl;

static const int NUM_SNIPPETS_150 = 17;
static const int NUM_SNIPPETS_4 = 19;


/**
 * Snippets for 1.50 Shader model
 */

///\todo having all this here does mean it ends up in memory that we dont need perhaps :S

ShaderSnippet snippets_150[NUM_SNIPPETS_150] = { 
  // Basic Header
  {VERTEX_SNIPPET,    "Header", "#version 150\n" },
  {FRAGMENT_SNIPPET,  "Header", "#version 150\n" },

  // Bare minimum
  {VERTEX_SNIPPET,      "Basic",            "attribute vec3 aVertexPosition;\nuniform mat4 uModelMatrix;\n"},
  
  // Vertex Colour
  {VERTEX_SNIPPET,      "VertexColour",     "attribute vec4 aVertexColour;\nvarying vec4 vVertexColour;\n"},
  {FRAGMENT_SNIPPET,    "VertexColour",     "varying vec4 vVertexColour;\n"},

  // Vertex Texture Co-ordinate
  {VERTEX_SNIPPET,      "VertexTexCoord",   "attribute vec2 aVertexTexCoord; varying vec2 vTexCoord;\n"},
  {FRAGMENT_SNIPPET,    "VertexTexCoord",   "varying vec2 vTexCoord;\n"},

  // Basic Camera matrices
  {VERTEX_SNIPPET,      "BasicCamera",      "uniform mat4 uCameraMatrix;\nuniform mat4 uProjectionMatrix;\nuniform mat4 uCameraInverseMatrix;\n"},

  // Extra camera things like near and far
  {VERTEX_SNIPPET,      "CameraExtra",      "uniform float uCameraNear;\nuniform float uCameraFar;\n"},
  {FRAGMENT_SNIPPET,    "CameraExtra",      "uniform float uCameraNear;\nuniform float uCameraFar;\n"},

  // Vertex Normal stuff
  {VERTEX_SNIPPET,      "VertexNormal",     "attribute vec3 aVertexNormal;\nvarying vec3 vVertexNormal;\nuniform mat3 uNormalMatrix;\n"},
  {FRAGMENT_SNIPPET,    "VertexNormal",     "varying vec3 vVertexNormal;\n"},
  {VERTEX_MAIN,         "VertexNormal",     "vVertexNormal = normalize(uNormalMatrix * aVertexNormal);\n"},

  // Basic Material
  {FRAGMENT_SNIPPET,    "BasicMaterial",    "uniform vec3 uMaterialAmbientColor;\nuniform vec3 uMaterialDiffuseColor;\nuniform vec3 uMaterialSpecularColor;\nuniform float uMaterialShininess;\nuniform vec3 uMaterialEmissiveColor;\n"},
  
  // Vertex Tangent
  {VERTEX_SNIPPET,      "VertexTangent",    "attribute vec3 aVertexTangent;\nvarying vec4 vTangent;\n"},
  {FRAGMENT_SNIPPET,    "VertexTangent",    "varying vec4 vTangent;\n"},
  {VERTEX_MAIN,         "VertexTangent",    "vTangent = vec4(aVertexTangent,1.0);\n"},
};


/**
 * Snippets for 4 Shader model
 * \TODO - positional arguments are fixed at present - we will need to change that at some point
 */

ShaderSnippet snippets_4[NUM_SNIPPETS_4] = { 

  // Header
  {VERTEX_SNIPPET,      "Header", "#version 420\n" },
  {FRAGMENT_SNIPPET,    "Header", "#version 420\n\nout vec4 fragColor;"},

  // Bare minimum
  {VERTEX_SNIPPET,      "Basic",            "layout (location = 0) in vec3 aVertexPosition;\nuniform mat4 uModelMatrix;\n out vec4 vVertexPosition;\n"},
  
  // Vertex Colour
  {VERTEX_SNIPPET,      "VertexColour",     "layout (location = 2) in vec4 aVertexColour;\nout vec4 vVertexColour;\n"},
  {FRAGMENT_SNIPPET,    "VertexColour",     "varying vec4 vVertexColour;\n"},
  {VERTEX_MAIN,         "VertexColour",     "vVertexColour = aVertexColour;\n"},

  // Vertex Texture Co-ordinate
  {VERTEX_SNIPPET,      "VertexTexCoord",   "layout (location = 3) in vec2 aVertexTexCoord;\nout vec4 vTexCoord;\n"},
  {FRAGMENT_SNIPPET,    "VertexTexCoord",   "varying vec2 vTexCoord;\n"},
  {VERTEX_MAIN,         "VertexTexCoord",   "vTexCoord = aVertexTexCoord;\n"},

   // Basic Camera matrices
  {VERTEX_SNIPPET,      "BasicCamera",      "uniform mat4 uCameraMatrix;\nuniform mat4 uProjectionMatrix;\nuniform mat4 uCameraInverseMatrix;\n"},

  // Extra camera things like near and far
  {VERTEX_SNIPPET,      "CameraExtra",      "uniform float uCameraNear;\nuniform float uCameraFar;\n"},
  {FRAGMENT_SNIPPET,    "CameraExtra",      "uniform float uCameraNear;\nuniform float uCameraFar;\n"},

  // Vertex Normal stuff
  {VERTEX_SNIPPET,      "VertexNormal",     "layout (location = 1) in vec3 aVertexNormal;\nout vec4 vNormal;\nuniform mat4 uNormalMatrix;\n"},
  {FRAGMENT_SNIPPET,    "VertexNormal",     "in vec4 vVertexNormal;\n"},
  {VERTEX_MAIN,         "VertexNormal",     "vVertexNormal = normalize(uNormalMatrix * vec4(aVertexNormal,1.0));\n"},

  // Basic Material
  {FRAGMENT_SNIPPET,    "BasicMaterial",    "uniform vec3 uMaterialAmbientColor;\nuniform vec3 uMaterialDiffuseColor;\nuniform vec3 uMaterialSpecularColor;\nuniform float uMaterialShininess;\nuniform vec3 uMaterialEmissiveColor;\n"},
  
  // Vertex Tangent
  {VERTEX_SNIPPET,      "VertexTangent",    "attribute vec3 aVertexTangent;\nvarying vec3 vTangent;\n"},
  {FRAGMENT_SNIPPET,    "VertexTangent",    "varying vec3 vTangent;\n"},
  {VERTEX_MAIN,         "VertexTangent",    "vTangent = aVertexTangent;\n"},
  
};




/**
 * Shader Library Class that loads all the snippets - Check the context currently loaded
 * Pass in a copy of the context so we can decide which shaders to build
 */

ShaderLibrary::ShaderLibrary(int major_version) : obj_(shared_ptr<SharedObject>(new SharedObject())) {
 
  // This is a bit scrappy but should do for now -  choose between two
  if (major_version < 4.0) {
    obj_->snippets = snippets_150;
    obj_->num_snippets = NUM_SNIPPETS_150;
  } else {
    obj_->snippets = snippets_4;
    obj_->num_snippets = NUM_SNIPPETS_4;
  }

}


std::string  ShaderLibrary::GetSnippet(std::string name, SnippetType type) {
  assert(obj_);

  for (int i=0; i < obj_->num_snippets; ++i){
    if (name.compare(obj_->snippets[i].label) != 0){
      if (type == obj_->snippets[i].type ){
        return obj_->snippets[i].text;
      }
    }
  }

  return "";

}




