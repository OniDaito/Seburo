/**
* @brief Vertex Types
* @file vertex_types.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 16/07/2012
*
* \todo triangle fan and other such bits
*
*/


#ifndef S9_PRIMITIVES_HPP
#define S9_PRIMITIVES_HPP

#include "common.hpp"
#include "string_utils.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

///\todo is GLM dependent on OpenGL or not?

namespace s9 {


	/// Global constants
	const static uint8_t geometry_max_bones = 16;

	/// Global typedefs
	typedef uint32_t IndicesType; // Expand potentially on 64 bit systems?


/// \todo using padding globally but that may interfere with GLM and not always be great
// eg vec3? we could pad to 4 and it might be better?

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
	template <class T, class U >
	struct VertexT {

		VertexT(const T pp = T(1.0f), const T pn = T(1.0f), 
			const T pc = T(1.0f), const U pu = U(1.0f), 
			const T pt = T(1.0f)){
			p = pp;
			n = pn;
			c = pc;
			u = pu;
			t = pt;
		}

		T p; // position
		T n; // normal
		T c; // colour
		U u; // texture uv
		T t; // tangent

		// TODO - Flatten function -> spit out for OpenGL
	};


	// By Default just use the GLM Float values as they mesh well with OpenGL buffers and shaders

	typedef VertexT<glm::vec2, glm::vec2> Vertex2;
	typedef VertexT<glm::vec3, glm::vec2> Vertex3;
	typedef VertexT<glm::vec4, glm::vec2> Vertex4;


	/**
	 * Template for just position vectors
	 */

	template <class T = glm::vec3>
	struct VertexPT {
		VertexPT(const T pp = T(1.0f)){
			p = pp;	
		} 

		T p;
	};

	typedef VertexPT<glm::vec2> Vertex2P;
	typedef VertexPT<glm::vec3> Vertex3P;
	typedef VertexPT<glm::vec4> Vertex4P;


	/**
	 * Template for basic position, normal and texture
	 */

	template <class T = glm::vec3, class U = glm::vec2>	
	struct VertexPNUT {
		VertexPNUT(const T pp = T(1.0f), const T pn = T(1.0f), 
			const U pu = U(1.0f)){
			p = pp;
			n = pn;
			u = pu;
		}

		T p; // position
		T n; // normal
		U u; // texture uv
	};


	typedef VertexPNUT<glm::vec2, glm::vec2> Vertex2PNU;
	typedef VertexPNUT<glm::vec3, glm::vec2> Vertex3PNU;
	typedef VertexPNUT<glm::vec4, glm::vec2> Vertex4PNU;

	/**
	 * Template for basic position, normal, tangent, texture and skinning
	 * We have an upper limit for the number of bones allowed - 16 normally
	 */

	template <typename T = glm::vec3, typename U = glm::vec2, typename V = IndicesType, typename W = float>	
	struct VertexPSkin {
		VertexPSkin(const T pp = T(1.0f), const T pn = T(1.0f), 
			const U pu = U(1.0f), const T pt = T(1.0f)){
			p = pp;
			n = pn;
			u = pu;
			t = pt;
		}

		T p; 	// position
		T n; 	// normal
		U u; 	// texture uv
		T t; 	// tangent
		V b[geometry_max_bones]; // bone index
		W w[geometry_max_bones]; // bone weight

	};

	typedef VertexPSkin<glm::vec3, glm::vec2, IndicesType, float> Vertex3Skin;
	typedef VertexPSkin<glm::vec4, glm::vec2, IndicesType, float> Vertex4Skin;

 	
 	/**
 	 * Template for basic face data
 	 */

 	template <class T = glm::vec3 >
 	struct FaceT {
 		FaceT (const T pn = T(1.0f), const T pc = T(1.0f)) {
 			normal = pn;
 			colour = pc;
 		} 

 		T normal;
 		T colour;
 	};
 
	
 	typedef FaceT<glm::vec3> Face3;
 	typedef FaceT<glm::vec4> Face4;


#pragma pack(pop)  

}

#endif
