/**
* @brief FBO Classes
* @file fbo.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/


#ifndef FBO_HPP
#define FBO_HPP


#include "../common.hpp"
#include "common.hpp"
#include "texture.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace s9 {

	namespace gl {

		/**
		 * Basic FBO Class with depth buffer and colour texture attachments
		 * \TODO - needs more options! Many more options
		 * \todo - depth?
		 * \todo we may end up with GL_TEXTURE_2D on creation. If we then resize, it wont work :S
		 */
		 
		class SEBUROAPI FBO {

		protected:
			struct SharedObject {

				~SharedObject();

				GLuint width, height, id, depth;
				Texture colour;
				bool ok;
			};
			std::shared_ptr<SharedObject> obj_;
			
		public:
			FBO() {};
			FBO(size_t w, size_t h);

			void Bind() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, obj_->id);};
			void Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); } ;
			bool CheckStatus();
			void PrintFramebufferInfo();
			void Resize(size_t w, size_t h);
			
			Texture colour() { return obj_->colour; }

			glm::vec2 size() { return glm::vec2(obj_->width, obj_->height); }

			GLuint width() {return obj_->width; };
			GLuint height() {return obj_->height; }

			typedef std::shared_ptr<SharedObject> FBO::*unspecified_bool_type;
    	operator unspecified_bool_type() const { return ( obj_.get() == 0 ) ? 0 : &FBO::obj_; }
    	void reset() { obj_.reset(); }
			
	
		};

		std::string GetTextureParameters(GLuint id);
		std::string GetRenderbufferParameters(GLuint id);
		std::string ConvertInternalFormatToString(GLenum format);
	}
}

#endif
