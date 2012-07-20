/**
* @brief OpenGL Specfic Primitives and such
* @file shapes.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 18/07/2012
*
*/

#ifndef GL_SHAPES_HPP
#define GL_SHAPES_HPP

#include "common.hpp"
#include "shapes.hpp"
#include "primitive.hpp"

namespace s9 {

	namespace gl {

		class ViaVAO {
		public:
			void bind() { glBindVertexArray(mVAO); };
			void unbind()  { glBindVertexArray(0); };
			GLuint mVAO;

		};

		/*
		 * Extends the basic Quad for OpenGL stuff
		 */

		class Quad : public s9::Quad, public ViaVAO {
		protected:
			void _gen();

		public:
			Quad(size_t w, size_t h) : s9::Quad(w,h) { mVAO = 0; }
			void draw();
			
	
		};

		/*
		 * Extend geometry for an OpenGL Triangle
		 */

		class Triangle : public s9::Triangle, public ViaVAO {
		protected:
			void _gen();
	
		public:
			Triangle(size_t w, size_t h) : s9::Triangle(w,h) { mVAO = 0; }
			void draw();
		
		};

	}

}

#endif