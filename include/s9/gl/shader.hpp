/**
* @brief Shader Classes
* @file shader.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/


#ifndef GL_SHADER_HPP
#define GL_SHADER_HPP

#include "../common.hpp"
#include "common.hpp"
#include "../utils.hpp"

/*
 * Basic Shader class - loads and binds
 */

namespace s9 {

	namespace gl {

		class OPENGLCOURSEAPI Shader {
		public:
			void load(std::string vert, std::string frag);
			GLuint getProgram() { return mProgram; };
			
			GLint location(const char * name) {return glGetUniformLocation(mProgram, name); }
			
			// Fluent interface for quick setting

			Shader& s(const char * name, glm::vec3 v);
			Shader& s(const char * name, glm::vec4 v);
			Shader& s(const char * name, glm::mat4 v);
			Shader& s(const char * name, float f);
			Shader& s(const char * name, int i);

			void bind() { glUseProgram(mProgram);};
			void unbind() {glUseProgram(0);};
			
			~Shader() { glDetachShader(mProgram, mVS); glDetachShader(mProgram, mFS);  } 
			
		protected:
		   
			GLuint mVS, mFS;
			GLuint mProgram;

		};
	}
}


#endif
