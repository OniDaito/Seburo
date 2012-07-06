/**
* @brief FBO Classes
* @file fbo.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/


#ifndef FBO_HPP
#define FBO_HPP


#include "common.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace s9 {

	/*
	 * Basic FBO Class
	 */
	 
	class FBO {
	public:
		void setup(size_t w, size_t h);
		void bind() { glBindFramebuffer(GL_FRAMEBUFFER, mID); glViewport(0,0,mW,mH); };
		void unbind() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); } ;
		bool checkStatus();
		void printFramebufferInfo();
		void resize(size_t w, size_t h);
		void bindColour() {glBindTexture(GL_TEXTURE_RECTANGLE, mColour); }
		void unbindColour() {glBindTexture(GL_TEXTURE_RECTANGLE, 0); }
		void bindDepth() {glBindTexture(GL_TEXTURE_RECTANGLE, mDepth); }
		void unbindDepth() {glBindTexture(GL_TEXTURE_RECTANGLE, 0);  }
		
		GLuint getWidth() {return mW; };
		GLuint getHeight() {return mH; }
		
	protected:
		GLuint mW,mH,mID,mDepth,mColour;

	};

	std::string getTextureParameters(GLuint id);
	std::string getRenderbufferParameters(GLuint id);
	std::string convertInternalFormatToString(GLenum format);
}

#endif
