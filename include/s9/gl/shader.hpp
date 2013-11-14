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
#include "../string_utils.hpp"
#include "utils.hpp"
#include "../file.hpp"

/*
 * Basic Shader class - loads and binds
 */

namespace s9 {

	namespace gl {

		///\todo ShaderLibrary for our UberShader
		/*class SEBUROAPI ShaderLibrary {

		};*/

		/**
		 *
		 */

		template <typename T>
		struct SEBUROAPI ShaderClause {
			ShaderClause(const std::string &s, T& d, size_t l = 1 ) : name(s), data(d), size(l) {}
			
			const std::string name;
			T& data;
			size_t size;

		};

		/**
		 * Called by nodes to present data to the currently bound shader
		 * Follows the visitor pattern. 
		 */

		class SEBUROAPI ShaderVisitor {
		public:

			GLuint location(const char * name ){ 
      	GLint p;
        glGetIntegerv(GL_CURRENT_PROGRAM, &p); 
        return glGetUniformLocation(p, name);
      }

			template<typename T>
			void sign( ShaderClause<T> &c) { assert(false); }

		};

		
		class SEBUROAPI Shader { ///\todo shared object? Possibly? If we add caching then fo shure!
		
		public:
			Shader() {};

			///\todo not sure if we need all these constructors but hey
			Shader(s9::File glsl);
			Shader(s9::File vert, s9::File frag);
			Shader(s9::File vert, s9::File frag, s9::File geom);
			Shader(std::string glsl_string);
			Shader(std::string vert_string, std::string frag_string);
			Shader(std::string vert_string, std::string frag_string, std::string geom_string);

			GLuint getProgram() { return obj_->program; };
			GLint location(const char * name) {return glGetUniformLocation(obj_->program, name); }
			
			// Fluent interface for quick setting

			Shader& s(const char * name, glm::vec3 v);
			Shader& s(const char * name, glm::vec4 v);
			Shader& s(const char * name, glm::mat4 v);
			Shader& s(const char * name, float f);
			Shader& s(const char * name, int i);

			/// Bind this shader to the current context, saving the current shader
			void bind() 	{ 
				glGetIntegerv(GL_CURRENT_PROGRAM, &(obj_->prev));  
				glUseProgram(obj_->program); 
			}
			
			/// Restore the previous shader
			void unbind() {	glUseProgram(obj_->prev); }
						
		protected:
		   
			bool parse(std::string &glsl, std::string &vs, std::string &fs, std::string &gs);
			bool createShader(GLenum type, GLuint &handle, std::string &data);
			bool createAndLink();

			struct SharedObject {
				~SharedObject();
				GLuint vs, fs, gs;
				GLint program, prev;
			};

			std::shared_ptr<SharedObject> obj_ = nullptr;
		};
	}
}


#endif
