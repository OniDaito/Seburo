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
#include "geom.hpp"
#include "primitive.hpp"

namespace s9 {
	
	class Quad : public Primitive {
	public:
		Quad(float w, float h);
		GeometryFullFloat mGeom;
	};

	class Triangle : public Primitive {
	public:
		Triangle(float w, float h);
		GeometryFullFloat mGeom;
	};

}


#endif
