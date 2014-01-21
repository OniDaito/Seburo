/**
* @brief Representation of a surface material
* @file material.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 21/01/2014
*
*/

#ifndef S9_MATERIAL_HPP
#define S9_MATERIAL_HPP

#include "common.hpp"
#include "math_utils.hpp"


namespace s9{

  struct Material {

    ///\todo at some point we need a colour class of some kind

    Material(glm::vec3 a = glm::vec3(1.0f,1.0f,1.0f), 
      glm::vec3 d = glm::vec3(1.0f,1.0f,1.0f), 
      glm::vec3 s = glm::vec3(1.0f,1.0f,1.0f), 
      glm::vec3 e = glm::vec3(1.0f,1.0f,1.0f), float_t h = 1.0f, std::string n = "material") {

      ambient = glm::vec4(a.x, a.y, a.z, 1.0f);
      diffuse = glm::vec4(d.x, d.y, d.z, 1.0f);
      specular= glm::vec4(s.x, s.y, s.z, 1.0f);
      emissive = glm::vec4(e.x, e.y, e.z, 0.0f);
      shine = h;
      name = n;
    }


    glm::vec4 ambient, diffuse, specular, emissive;
    float_t shine;
    std::string name;

  };

}

#endif