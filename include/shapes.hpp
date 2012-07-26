/**
* @brief Shapes Classes - basics
* @file shapes.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 06/07/2012
*
*/


#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "common.hpp"
#include "geometry.hpp"
#include "primitive.hpp"

namespace s9 {
	
	class Quad : public Primitive {
	public:
		Quad(){};
		Quad(float_t w, float_t h);
		GeometryFullFloat mGeom;
	};

	class Triangle : public Primitive {
	public:
		Triangle(){};
		Triangle(float_t w, float_t h);
		GeometryFullFloat mGeom;
	};

}


#endif
