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
#include "shader_library.hpp"

/*
 * Basic Shader class - loads and binds
 * \todo shaders need tags so when we have loads & computed ones, we can tell where errors come from
 */

namespace s9 {

	namespace gl {

		/// Global Shader Constants

		static const size_t shader_bone_limit = 128; // this seems quite high :S

		///\todo ShaderLibrary for our UberShader
		/*class SEBUROAPI ShaderLibrary {

		};*/

		/**
		 * Shader Clause Template for data references with more than one entry
		 */

		template <typename T, size_t N>
		struct SEBUROAPI ShaderClause {
			ShaderClause(const std::string &s, const T (&d) [N] ) : name(s), data(d), size(N) {}
			
			const std::string name;
			const T (&data)[N];
			size_t size;

		};

		template <typename T>
		struct SEBUROAPI ShaderClause<T,1> {
			ShaderClause(const std::string &s, const T &d ) : name(s), data(d), size(1) {}
			
			const std::string name;
			const T& data;
			size_t size;

		};


		/**
		 * Called by nodes to present data to the currently bound shader
		 * Follows the visitor pattern. 
		 * \todo put a cache here perhaps?
		 */

		class SEBUROAPI ShaderVisitor {
		public:

			GLuint Location(const char * name ){ 
      	GLint p;
        glGetIntegerv(GL_CURRENT_PROGRAM, &p);
        CXGLERROR
        if (p > -1)
        	return glGetUniformLocation(p, name);
        return 0;
      }

      bool Bound() {
      	GLint p;
        glGetIntegerv(GL_CURRENT_PROGRAM, &p);
        return p > -1;
      }

			template<typename T, size_t N>
			void Sign( ShaderClause<T, N> &c);

			/// Vector4 single signing
			void Sign(ShaderClause<glm::vec4,1> &c);

			/// Vector2 single signing
			void Sign(ShaderClause<glm::vec2,1> &c);


			/// Matrix4 single signing

			void Sign( ShaderClause<glm::mat4, 1> &c);

			/// Matrix 4 multiple signing - flattens all the matrices into one array - potentially slow :S
			///\todo - speed up?
			template<size_t N>
			void Sign( ShaderClause<glm::mat4, N> &c) {
			
				GLuint l = Location(c.name.c_str());
				GLfloat tp [N * 16];
				for (int i = 0; i < N; ++i){
					tp[i*16] = c.data[i][0][0];
					tp[i*16 + 1] = c.data[i][1][0];
					tp[i*16 + 2] = c.data[i][2][0];
					tp[i*16 + 3] = c.data[i][3][0];
					tp[i*16 + 4] = c.data[i][0][1];
					tp[i*16 + 5] = c.data[i][1][1];
					tp[i*16 + 6] = c.data[i][2][1];
					tp[i*16 + 7] = c.data[i][3][1];
					tp[i*16 + 8] = c.data[i][0][2];
					tp[i*16 + 9] = c.data[i][1][2];
					tp[i*16 + 10] = c.data[i][2][2];
					tp[i*16 + 11] = c.data[i][3][2];
					tp[i*16 + 12] = c.data[i][0][3];
					tp[i*16 + 13] = c.data[i][1][3];
					tp[i*16 + 14] = c.data[i][2][3];
					tp[i*16 + 15] = c.data[i][3][3];
				}

  			glUniformMatrix4fv( l, N, GL_FALSE, &(tp[0]) );
  			CXGLERROR
			}

			template<size_t N>
			void Sign( ShaderClause<glm::mat4x2, N> &c) {
				GLuint l = Location(c.name.c_str());
				GLfloat tp [N * 8];
				for (int i = 0; i < N; ++i){
					tp[i*8] = c.data[i][0][0];
					tp[i*8 + 1] = c.data[i][1][0];
					tp[i*8 + 2] = c.data[i][2][0];
					tp[i*8 + 3] = c.data[i][3][0];
					tp[i*8 + 4] = c.data[i][0][1];
					tp[i*8 + 5] = c.data[i][1][1];
					tp[i*8 + 6] = c.data[i][2][1];
					tp[i*8 + 7] = c.data[i][3][1];
				}

  			glUniformMatrix4x2fv( l, N, GL_FALSE, &(tp[0]) ); ///\todo test this
  				CXGLERROR
			}

			template<size_t N>
			void Sign( ShaderClause<glm::mat2x4, N> &c) {
				GLuint l = Location(c.name.c_str());
				GLfloat tp [N * 8];
				for (int i = 0; i < N; ++i){
					tp[i*8] = c.data[i][0][0];
					tp[i*8 + 1] = c.data[i][0][1];
					tp[i*8 + 2] = c.data[i][0][2];
					tp[i*8 + 3] = c.data[i][0][3];
					tp[i*8 + 4] = c.data[i][1][0];
					tp[i*8 + 5] = c.data[i][1][1];
					tp[i*8 + 6] = c.data[i][1][2];
					tp[i*8 + 7] = c.data[i][1][3];
				}

  			glUniformMatrix2x4fv( l, N, GL_FALSE, &(tp[0]) ); ///\todo test this
  				CXGLERROR
			}

			template<size_t N>
			void Sign( ShaderClause<float, N> &c);
			void Sign(ShaderClause<float, 1> &c);
			void Sign(ShaderClause<uint, 1> &c);
			void Sign(ShaderClause<int, 1> &c);

		};


		/**
		 * Our Shader Class workhorse. Added to nodes to create our graphics! :D
		 */
		
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

			GLuint program() { return obj_->program; };
			GLint Location(const char * name) {return glGetUniformLocation(obj_->program, name); }
			
			// Fluent interface for quick setting

			Shader& s(const char * name, glm::vec3 v);
			Shader& s(const char * name, glm::vec4 v);
			Shader& s(const char * name, glm::mat4 v);
			Shader& s(const char * name, float f);
			Shader& s(const char * name, int i);

			/// Bind this shader to the current context, saving the current shader
			void Bind() 	{ 
				glGetIntegerv(GL_CURRENT_PROGRAM, &(obj_->prev));  
				glUseProgram(obj_->program); 
			}
			
			/// Restore the previous shader
			void Unbind() {	glUseProgram(obj_->prev); }
			
		protected:
		  
			bool Parse(std::string &glsl, std::string &vs, std::string &fs, std::string &gs);
			bool CreateShader(GLenum type, GLuint &handle, std::string &data);
			bool CreateAndLink();

			struct SharedObject {
				~SharedObject();
				GLuint vs, fs, gs;
				GLint program, prev;
			};

			std::shared_ptr<SharedObject> obj_ = nullptr;

		public:

	    bool operator == (const Shader &ref) const { return this->obj_ == ref.obj_; }
	    typedef std::shared_ptr<SharedObject> Shader::*unspecified_bool_type;
	    operator unspecified_bool_type() const { return ( obj_.get() == 0 ) ? 0 : &Shader::obj_; }
	    void reset() { obj_.reset(); }


		};
		
		/* Create a shader in various ways, using the shader library class and other files we have
		 * Mostly static methods in this class really
		 */

		class ShaderBuilder {
		public:

			ShaderBuilder();
			ShaderBuilder(const Context context);

			ShaderBuilder& AddSnippet(std::string t);
			ShaderBuilder& AddUserText(SnippetType type, std::string text);
			Shader Build();
			ShaderBuilder& Clear();

		private:

			struct SharedObject {
				std::string vertex_buffer_;
				std::string vertex_main_buffer_;
				std::string fragment_buffer_;
				std::string fragment_main_buffer_;
				std::string geometry_buffer_;
				std::string geometry_main_buffer_;

				ShaderLibrary library_;
			};

		std::shared_ptr<SharedObject> obj_ = nullptr;

		public:

	    bool operator == (const ShaderBuilder &ref) const { return this->obj_ == ref.obj_; }
	    typedef std::shared_ptr<SharedObject> ShaderBuilder::*unspecified_bool_type;
	    operator unspecified_bool_type() const { return ( obj_.get() == 0 ) ? 0 : &ShaderBuilder::obj_; }
	    void reset() { obj_.reset(); }

		};
	
	}
}


#endif
