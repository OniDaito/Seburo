/**
* @brief Shader Classes
* @file shader.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/


#ifndef SHADER_HPP
#define SHADER_HPP

#include "common.hpp"

/*
 * Basic Shader class - loads and binds
 */

namespace s9 {

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


#endif
