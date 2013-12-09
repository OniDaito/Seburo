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

			virtual operator int() const { return obj_.use_count() > 0; };

			void bind() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, obj_->id);  glViewport(0, 0, obj_->width, obj_->height); };
			void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); } ;
			bool checkStatus();
			void printFramebufferInfo();
			void resize(size_t w, size_t h);
			
			Texture colour() { return obj_->colour; }
			
			glm::vec2 size() { return glm::vec2(obj_->width, obj_->height); }

			GLuint width() {return obj_->width; };
			GLuint height() {return obj_->height; }
			
	
		};

		std::string getTextureParameters(GLuint id);
		std::string getRenderbufferParameters(GLuint id);
		std::string convertInternalFormatToString(GLenum format);
	}
}

#endif
