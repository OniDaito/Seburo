/**
* @brief GL Utilities that might be handy
* @file glutils.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 06/07/2012
*
*/

#ifndef GL_UTILS_HPP
#define GL_UTILS_HPP

#include "../common.hpp"
#include "common.hpp"

namespace s9 {
	namespace gl {

///\todo set this to go off only once otherwise we end up spamming the logs ( maybe even quit?)

	static std::string LAST_ERROR;

#define CXGLERROR CheckError(__FUNCTION__,__LINE__,__FILE__);

		inline bool CheckError(const char * func, int line, const char* file) {
			int Error;
			if((Error = glGetError()) != GL_NO_ERROR){
				std::string ErrorString;
				switch(Error)
				{
				case GL_INVALID_ENUM:
					ErrorString = "GL_INVALID_ENUM";
					break;
				case GL_INVALID_VALUE:
					ErrorString = "GL_INVALID_VALUE";
					break;
				case GL_INVALID_OPERATION:
					ErrorString = "GL_INVALID_OPERATION";
					break;
				case GL_INVALID_FRAMEBUFFER_OPERATION:
					ErrorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
					break;
				case GL_OUT_OF_MEMORY:
					ErrorString = "GL_OUT_OF_MEMORY";
					break;
				default:
					ErrorString = "UNKNOWN";
					break;
				}

				// Only print the error once as OpenGL has a tendency to spam things a lot

				std::stringstream ss;
				ss << "SEBURO - OpenGL Error " << ErrorString << " at " << line << " in " << func << " in " << file;

				std::string msg = ss.str();

				if (LAST_ERROR.compare(msg) !=0 ){
					LAST_ERROR = msg;
					std::cerr << msg << std::endl;
				}
				
			}
			return Error == GL_NO_ERROR;
		}
	}
}

#endif
