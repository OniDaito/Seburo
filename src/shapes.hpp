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
	
	Primitive makeQuad(float_t w, float_t h);
	Primitive makeReferenceQuad(float_t w, float_t h);
	Primitive makeReferenceTriangle(float_t w, float_t h);
}


#endif
