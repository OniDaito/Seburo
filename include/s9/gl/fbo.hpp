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
				GLuint mW,mH,mID,mDepth,mColour;
				bool mOk;
			};
			boost::shared_ptr<SharedObj> mObj;
			
		public:
			FBO() {};
			FBO(size_t w, size_t h);
			void bind() { glBindFramebuffer(GL_FRAMEBUFFER, mObj->mID); glViewport(0,0,mObj->mW,mObj->mH); };
			void unbind() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); } ;
			bool checkStatus();
			void printFramebufferInfo();
			void resize(size_t w, size_t h);
			void bindColour() { glBindTexture(GL_TEXTURE_RECTANGLE, mObj->mColour); }
			void unbindColour() { glBindTexture(GL_TEXTURE_RECTANGLE, 0); }
			void bindDepth() { glBindTexture(GL_TEXTURE_RECTANGLE, mObj->mDepth); }
			void unbindDepth() { glBindTexture(GL_TEXTURE_RECTANGLE, 0);  }
			
			GLuint getWidth() {return mObj->mW; };
			GLuint getHeight() {return mObj->mH; }
			
		

		};

		std::string getTextureParameters(GLuint id);
		std::string getRenderbufferParameters(GLuint id);
		std::string convertInternalFormatToString(GLenum format);
	}
}

#endif
