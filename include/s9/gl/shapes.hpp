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
#include "s9/gl/asset.hpp"

namespace s9 {

	namespace gl {


		/*
		 * Extends the basic Quad for OpenGL stuff
		 */

		class Quad : public s9::gl::Asset<GeometryFullFloat> {

		public:
			Quad(){};
			Quad(float w, float h) {
				s9::Quad q (w,h);
				_obj.reset(new SharedObj()); 
				_obj->_geom = q.getGeometry();
			}
		};

		/*
		 * Extend geometry for an OpenGL Triangle
		 */

/*		class Triangle : public s9::Triangle, public ViaVAO {
		protected:
			void _gen();
			void _allocate();
	
		public:
			Triangle() {};
			Triangle(float w, float h) : s9::Triangle(w,h) { mVAO = 0; }
			void draw();
		
		};*/

	}

}

#endif