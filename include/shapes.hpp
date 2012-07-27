/**
* @brief Shapes Classes - basics
* @file shapes.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 06/07/2012
*
*/


#ifndef S9_SHAPES_HPP
#define S9_SHAPES_HPP

#include "common.hpp"
#include "geometry.hpp"
#include "primitive.hpp"

namespace s9 {
	
	/*
	 * Essentially the same as Assets but they have special properties on their geometry
	 */ 

	class Quad : public Primitive {
	public:
		Quad(){};
		Quad(float_t w, float_t h);
		void resize(float_t w, float_t h);
		GeometryFullFloat mGeom;
	};

	class Triangle : public Primitive {
	public:
		Triangle(){};
		Triangle(float_t w, float_t h);
		void resize(float_t w, float_t h);
		GeometryFullFloat mGeom;
	};

}


#endif
