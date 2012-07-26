/**
* @brief Common Headers that are external for OpenGL
* @file gl/common.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 27/07/2012
*
*/


///\todo precompile!


#ifndef GL_COMMON_HPP
#define GL_COMMON_HPP

#include <GL/glew.h>
#include <GL/glut.h>

namespace s9 {
	namespace gl {
		
		/*
		 * Small Interface class to wrap the VAO state for an object
		 */

		class ViaVAO {
		public:
			void bind() { glBindVertexArray(mVAO); };
			void unbind()  { glBindVertexArray(0); };
			GLuint mVAO;

		};
	}
}


#endif