/**
* @brief OpenGL Specfic Primitives and such
* @file shapes.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 18/07/2012
*
*/

#ifndef GL_SHAPES_HPP
#define GL_SHAPES_HPP

#include "../common.hpp"
#include "common.hpp"
#include "utils.hpp"
#include "../shapes.hpp"
#include "../primitive.hpp"

namespace s9 {

	namespace gl {


		/*
		 * Extends the basic Quad for OpenGL stuff
		 */

		class Quad : public s9::Quad, public ViaVAO {
		protected:
			void _gen();
			void _allocate();

		public:
			Quad(){};
			Quad(float_t w, float_t h) : s9::Quad(w,h) { mVAO = 0; }
			void draw();
			
	
		};

		/*
		 * Extend geometry for an OpenGL Triangle
		 */

		class Triangle : public s9::Triangle, public ViaVAO {
		protected:
			void _gen();
			void _allocate();
	
		public:
			Triangle() {};
			Triangle(float_t w, float_t h) : s9::Triangle(w,h) { mVAO = 0; }
			void draw();
		
		};

	}

}

#endif