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
#include "asset.hpp"

namespace s9 {
	
	/*
	 * Essentially the same as Assets but they have special properties on their geometry
	 */ 

	class SEBUROAPI Quad : public Asset<GeometryFullFloat> {
	public:
		Quad(){};
		Quad(float w, float h);
		void resize(float w, float h);

	};

	class SEBUROAPI Triangle : public Asset<GeometryFullFloat> {
	public:
		Triangle(){};
		Triangle(float w, float h);
		void resize(float w, float h);
	};

}


#endif
