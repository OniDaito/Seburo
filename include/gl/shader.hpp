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

/*
 * Basic Shader class - loads and binds
 * \todo fluent and shorthand interface
 * \todo geometry shader
 */

namespace s9 {

	namespace gl {

		class Shader {
		public:
			void load(std::string vert, std::string frag);
			GLuint getProgram() { return mProgram; };
			
			GLint location(const char * name) {return glGetUniformLocation(mProgram, name); }
			
			
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