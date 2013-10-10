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
	 * ShapeT is the template for the basic class that forms geometry
	 * It looks after the memory and is the work horse for all geometry
	 * Shapes can contain several geometries of one kind (we dont mix quad and tri)
	 */

	template <class T, class U>
	class SEBUROAPI ShapeT : public GeometryT<T>  {
	protected:

		ShapeT() : obj_( std::shared_ptr<SharedObj> (new SharedObj())), GeometryT<T>(obj_->indices_by_ref) { std::cout << "called" << std::endl;}

		void generateIndicesFromGeometry();
		void generateGeometryFromIndices();

		struct SharedObj {
			std::vector<T> 					vertices;
			///\todo we have duplication here. Somewhat annoying
			std::vector<T*>					indices_by_ref;
			std::vector<uint32_t> 	indices;	/// numbered indices into the array of vertices
			std::vector<U>					geometry;

		};

		std::shared_ptr<SharedObj> obj_;
	};


	/**
	 * A basic Cuboid, made up of Quads with the underlying Vec3 type
	 * It contains a reference to its own vertices however. This is how more complex
	 * structures differ from their primitive counterparts
	 * indices match the quads
	 */ 

	class SEBUROAPI Cuboid : public ShapeT<Vertex3, Quad3>  {
	public:

		Cuboid() {};

		Cuboid(float w, float h, float d);

	};



}


#endif
