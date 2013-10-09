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


	/// \todo this is tricky because this default is for the Vec3 type and we will need to override for Vertex2/4
	template <class T = glm::vec3, class U = glm::vec2>
	struct VertexT {

		VertexT(const T p = glm::vec3(1.0f), const T n = glm::vec3(1.0f), 
			const T c = glm::vec3(1.0f), const U u = glm::vec2(1.0f), 
			const T t = glm::vec3(1.0f)){
			p_ = p;
			n_ = n;
			c_ = c;
			u_ = u;
			t_ = t;
		}

		T p_; // position
		T n_; // normal
		T c_; // colour
		U u_; // texture uv
		T t_; // tangent

		uint8_t f_ = 0x1F; // Default - LSB is position - MSB is tangent

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

   		GeometryT (std::vector<T*> &i) : indices(i) {}

   		std::vector<T*> &indices;	/// \todo Even mega large meshes will depend on this! 
   		/// :S Potential bottle neck if EVERY triangle has one
   		/// \todo is oldschool T* a good idea?
   		bool indexed_ = true;

   		T* operator [] (int x){
			if (x < indices.size() && x > -1){
				return indices.at(x);	
			}
			assert(false);
		}

		/// \todo face operations?

   };




	/**
	 * Template for a basic Quad. Used in the Geometry class. References a set of vertices
	 * This is essentially a *view* on a few vertices
	 */

	template <class T>
	struct QuadT : GeometryT<T> {

		QuadT(T *a, T *b, T *c, T *d) : GeometryT<T>(indices) {
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
			indices.push_back(d);

			assert (indices.size() == 4);	
		}

		std::vector<T*> indices;

		/// \todo face operations?
			
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

		TriangleT(T *a, T *b, T *c) : GeometryT<T>(indices) {
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
			assert (indices.size() == 3);	
		}

		std::vector<T*> indices;

		/// \todo face operations?

	};

	typedef TriangleT<Vertex2> Triangle2;
	typedef TriangleT<Vertex3> Triangle3;
	typedef TriangleT<Vertex4> Triangle4;


#pragma pack(pop)  

}

#endif
