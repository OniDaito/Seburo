/**
* @brief Vertex Types
* @file vertex_types.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 16/07/2012
*
*/


#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include "common.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

///\todo is GLM dependent on OpenGL or not?

namespace s9 {

// Make sure there is no padding here. Can add padding later

#pragma pack(push)
#pragma pack(1)    

	/**
	 * Template for the vertex type we are defining
	 * Vertices must have at least a position but can have other things attached
	 * as per the OpenGL rules for per vertex data.
	 *
	 * For speed, we dont allow adding or deleting to vertices, we have to copy
	 * There are flags for what is in use however, which are checked for buffering
	 */

	
	template <class T, class U>
	struct VertexT {
		T p_; // position
		T n_; // normal
		T c_; // colour
		U u_; // texture uv
		T t_; // tangent

		uint8_t f_ = 0x1F; // Default - all on! :)

		// TODO - Flatten function -> spit out for OpenGL
	};


	// By Default just use the GLM Float values as they mesh well with OpenGL buffers and shaders

	typedef VertexT<glm::vec2, glm::vec2> Vertex2;
	typedef VertexT<glm::vec3, glm::vec2> Vertex3;
	typedef VertexT<glm::vec4, glm::vec2> Vertex4;
 
  /**
   * Geometry template baseclass. Any grouping of vertices is classed as geometry
   * We can build more complex geometries from these
   */

   template <class T>
   struct GeometryT {

   		std::vector<T> &vertices_;	/// \todo Even mega large meshes will depend on this! 
   		/// :S Potential bottle neck if EVERY triangle has one
   
   		T operator [] (int x){
			if (x < vertices_.size() && x > -1){
				return vertices_.at(x);	
			}
			assert(false);
		}

   };




	/**
	 * Template for a basic Quad. Used in the Geometry class. References a set of vertices
	 * This is essentially a *view* on a few vertices
	 */

	template <class T>
	struct QuadT : GeometryT<T> {

		QuadT(std::vector<T> &v) : GeometryT<T>(v) {
			assert (v.size() == 4);	
		}

	
	};

	typedef QuadT<Vertex2> Quad2;
	typedef QuadT<Vertex3> Quad3;
	typedef QuadT<Vertex4> Quad4;


	/**
	 * Template for a basic Triangle. Used in the Geometry class. References a set of vertices
	 * This is essentially a *view* on a few vertices
	 */

	template <class T>
	struct TriangleT : GeometryT<T> {

		TriangleT(std::vector<T> &v) : GeometryT<T>(v) {
			assert (v.size() == 3);	
		}

	};

	typedef TriangleT<Vertex2> Tri2;
	typedef TriangleT<Vertex3> Tri3;
	typedef TriangleT<Vertex4> Tr4;


#pragma pack(pop)  

}

#endif
