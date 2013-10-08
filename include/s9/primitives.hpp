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
		T mP; // position
		T mN; // normal
		T mC; // colour
		U mU; // texture uv
		T mT; // tangent

		uint8_t mF &= 0x1F; // Default - all on! :)

		// TODO - Flatten function -> spit out for OpenGL
	};


	// By Default just use the GLM Float values as they mesh well with OpenGL buffers and shaders

	typedef VertexT<glm::vec2, glm::vec2> Vertex2;
	typedef VertexT<glm::vec3, glm::vec2> Vertex3;
	typedef VertexT<glm::vec4, glm::vec2> Vertex4;

	/**
	 * Template for a basic Quad. Used in the Geometry class
	 */

	template <class T>
	struct QuadT {

		QuadT(T &a, T &b, T &c, T &d) : mA(a), mB(b), mC(c), mD(d) {
		}

		T &mA;
		T &mB;
		T &mC;
		T &mD;

		T operator [] (int x){
			if (x < 4 && x > -1){
				T a[4] = {mA,mB,mC,mD};
				return a[x];	
			}

			assert(false);
		}

	};

	typedef QuadT<Vertex2> Quad2;
	typedef QuadT<Vertex3> Quad3;
	typedef QuadT<Vertex4> Quad4;


#pragma pack(pop)  

}

#endif
