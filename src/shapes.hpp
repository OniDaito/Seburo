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
#include "primitive.hpp"

namespace s9 {
	
	void makeQuad(Primitive &p, float_t w, float_t h);
	void makeReferenceQuad(Primitive &p, float_t w, float_t h);
	void makeReferenceTriangle(Primitive &p, float_t w, float_t h);
}


#endif
