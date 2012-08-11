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

		/*
		 * Basic FBO Class with depth and colour attachments
		 */
		 
		class FBO {

		protected:
			struct SharedObj {
				GLuint mW,mH,mID,mDepth;
				Texture _colour;
				bool mOk;
			};
			boost::shared_ptr<SharedObj> _obj;
			
		public:
			FBO() {};
			FBO(size_t w, size_t h);

			virtual operator int() const { return _obj.use_count() > 0; };

			void bind() { glBindFramebuffer(GL_FRAMEBUFFER, _obj->mID); glViewport(0,0,_obj->mW,_obj->mH); };
			void unbind() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); } ;
			bool checkStatus();
			void printFramebufferInfo();
			void resize(size_t w, size_t h);
			void bindColour() { _obj->_colour.bind(); }
			void unbindColour() { _obj->_colour.unbind(); }
			void bindDepth() { glBindTexture(GL_TEXTURE_RECTANGLE, _obj->mDepth); }
			void unbindDepth() { glBindTexture(GL_TEXTURE_RECTANGLE, 0);  }
			glm::vec2 size() { return glm::vec2(_obj->mW, _obj->mH); }

			GLuint getWidth() {return _obj->mW; };
			GLuint getHeight() {return _obj->mH; }
			
		

		};

		std::string getTextureParameters(GLuint id);
		std::string getRenderbufferParameters(GLuint id);
		std::string convertInternalFormatToString(GLenum format);
	}
}

#endif
