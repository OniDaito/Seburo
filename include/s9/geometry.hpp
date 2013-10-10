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
#include "primitives.hpp"

namespace s9 {

	/**
	 * Geometry - the base class for our typed geometry. Keeps basic flags on dirty levels etc
	 */

	class Geometry {

		protected:
			Geometry () { dirty_ = true; resized_ = true; };

			bool dirty_;
			bool resized_;

		public:
			bool dirty() { return dirty_; };
			bool resized() { return resized_; };

			void set_dirty(bool b) { dirty_ = b; };
			void set_resized(bool b) { resized_ = b; };
	};

	/**
	 * GeometryT is the template for the basic class that forms geometry
	 * It looks after the memory and is the work horse for all geometry
	 * GeometryT can contain either quads or tris but not both, or neither.
	 */

	template <class T, class U>
	class SEBUROAPI GeometryT : Geometry {
	protected:

		struct SharedObj {
			std::vector<T> 			vertices;
			///\todo we have duplication here. Somewhat annoying
			std::vector<T*>			indices_by_ref;
			std::vector<uint32_t> 	indices;	/// numbered indices into the array of vertices
			std::vector<U>			primitives;

		};

		GeometryT() {};
		GeometryT(std::shared_ptr<SharedObj> a) : obj_(a) {};

		void generateIndicesFromGeometry();
		void generateGeometryFromIndices();


		std::shared_ptr<SharedObj> obj_;
	};


	/**
	 * A basic Cuboid, made up of Quads with the underlying Vec3 type
	 * It contains a reference to its own vertices however. This is how more complex
	 * structures differ from their primitive counterparts
	 * indices match the quads
	 */ 

	class SEBUROAPI Cuboid : public GeometryT<Vertex3, Quad3>  {
	public:

		Cuboid() {};

		Cuboid(float w, float h, float d);

	};



}


#endif
