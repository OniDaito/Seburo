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

		///\todo ShaderLibrary for our UberShader
		/*class SEBUROAPI ShaderLibrary {

		};*/

		class SEBUROAPI Shader { ///\todo shared object? Possibly? If we add caching then fo shure!
		
		public:
			Shader() { vs_ = fs_ = gs_ = program_ = 0; }

			void load(std::string glsl);
			void load(std::string vert, std::string frag);
			void load(std::string vert, std::string frag, std::string geom);

			GLuint getProgram() { return program_; };
			GLint location(const char * name) {return glGetUniformLocation(program_, name); }
			
			// Fluent interface for quick setting

			Shader& s(const char * name, glm::vec3 v);
			Shader& s(const char * name, glm::vec4 v);
			Shader& s(const char * name, glm::mat4 v);
			Shader& s(const char * name, float f);
			Shader& s(const char * name, int i);

			void bind() 	{ glUseProgram(program_); }
			void unbind() {	glUseProgram(0); }
			
			~Shader();
			
		protected:
		   
			bool parse(std::string &glsl, std::string &vs, std::string &fs, std::string &gs);
			bool createShader(GLenum type, GLuint &handle, std::string &data);
			bool createAndLink();

			GLuint vs_, fs_, gs_;
			GLuint program_;

		};
	}
}


#endif
