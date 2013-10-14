/**
* @brief Shapes Classes - More complex geometry
* @file shapes.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/10/2013
*
*/



#ifndef S9_SHAPES_HPP
#define S9_SHAPES_HPP

#include "common.hpp"
#include "geometry.hpp"
#include "gl/drawable.hpp" // Default to OpenGL for now

namespace s9 {
	

	class Shape {
	protected:

	};

	/**
	 * A basic Cuboid, made up of Quads with the underlying Vec3 type
	 * It contains a reference to its own vertices however. This is how more complex
	 * structures differ from their primitive counterparts
	 * indices match the quads
	 */ 

	class SEBUROAPI Cuboid : public Shape {
	public:

		Cuboid() {};
		Cuboid(float w, float h, float d);


		

	protected:

		struct SharedObj {
			SharedObj() {
				geometry = std::shared_ptr< GeometryT<Vertex4, Face4, AllocationPolicyNew> > ( new GeometryT<Vertex4, Face4, AllocationPolicyNew> (8,24,QUADS));
			}


			std::shared_ptr < GeometryT<Vertex4, Face4, AllocationPolicyNew> > geometry;
			gl::Drawable drawable;

		};

		std::shared_ptr<SharedObj> obj_;

	public:
		//@{
		//! Emulates shared_ptr-like behavior
		typedef std::shared_ptr<SharedObj> Cuboid::*unspecified_bool_type;
		operator unspecified_bool_type() const { return ( obj_.get() == 0 ) ? 0 : &Cuboid::obj_; }
		void reset() { obj_.reset(); }
		//@} 
	};



}


#endif

