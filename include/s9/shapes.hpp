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
	
	/**
	 * Basic interface to shapes
	 */

	class Shape {

	public:
		virtual void draw(GeometryPrimitive g = TRIANGLES) = 0;
		virtual void brew(gl::BrewFlags b=gl::BrewFlagsDefault) = 0;

	};

	/**
	 * A basic Quad with no indexing.
	 */ 

	class SEBUROAPI Quad : public Shape {
	public:

		Quad() {};
		Quad(float w, float h);

		void brew(gl::BrewFlags b=gl::BrewFlagsDefault) { obj_->brew(b);}
		void draw(GeometryPrimitive g = TRIANGLES)  { obj_->drawable.draw(obj_->geometry, g); }

	protected:

		struct SharedObj {
			SharedObj() {
				geometry = GeometryT<Vertex4, Face4, AllocationPolicyNew> (6,0,TRIANGLES);	
			}

			void brew (gl::BrewFlags b) {
				drawable.brew(geometry, b);
			}

			GeometryT<Vertex4, Face4, AllocationPolicyNew> geometry;
			gl::Drawable drawable;
		
		};

		std::shared_ptr<SharedObj> obj_;

	public:
		//@{
		//! Emulates shared_ptr-like behavior
		typedef std::shared_ptr<SharedObj> Quad::*unspecified_bool_type;
		operator unspecified_bool_type() const { return ( obj_.get() == 0 ) ? 0 : &Quad::obj_; }
		void reset() { obj_.reset(); }
		//@} 
	};

	/**
	 * A basic Cuboid, made up of Quads with the underlying Vec4 type
	 */ 

	class SEBUROAPI Cuboid : public Shape {
	public:

		Cuboid() {};
		Cuboid(float w, float h, float d);

		void brew(gl::BrewFlags b=gl::BrewFlagsDefault) { obj_->brew(b);}
		void draw(GeometryPrimitive g = TRIANGLES)  { obj_->drawable.draw(obj_->geometry, g); }

		GeometryT<Vertex4, Face4, AllocationPolicyNew>& geometry() {return obj_->geometry; }

	protected:

		struct SharedObj {
			SharedObj() {
				geometry = GeometryT<Vertex4, Face4, AllocationPolicyNew>(8,36,TRIANGLES);
			}
			
			void brew (gl::BrewFlags b) {
				drawable.brew(geometry, b);
			}

			GeometryT<Vertex4, Face4, AllocationPolicyNew> geometry;
			gl::Drawable drawable;

		};

		std::shared_ptr<SharedObj> obj_ = nullptr;

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

