/**
* @brief Vertex Types
* @file vertex_types.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 16/07/2012
*
*/


#ifndef VERTEX_TYPES_HPP
#define VERTEX_TYPES_HPP

#include "common.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace s9 {

// Make sure there is no padding here. Can add padding later

#pragma pack(push)
#pragma pack(1)    

	// Composite widths here

	struct Double3 { double_t x,y,z; };
	
	struct Double2 { double_t x,y; };
	
	struct Double4 { double_t x,y,z,w; };
	
	struct Float2 { float_t x,y; };

	struct Float3 { float_t x,y,z; };
	
	struct Float4 { float_t x,y,z,w;};
	
	///\todo uint
	
	template <class T>
	struct VertexP {
		T mP;
	};


	template <class T, class U>
	struct VertexPN {
		T mP;
		U mN;
	};
	
	template <class T, class U, class V>
	struct VertexPNT {
		T mP;
		U mN;
		V mT;
	};
	
	
	template <class T, class U, class V>
	struct VertexPNC {
		T mP;
		U mN;
		V mC;
	};
	
	template <class T, class U, class V>
	struct VertexPCT {
		T mP;
		U mC;
		V mT;
	};
	
	
	template <class T, class U, class V, class W>
	struct VertexPNCT {
		T mP;
		U mN;
		V mC;
		W mT;
	};

	
	// Specially for our Leeds stuff
	template <class T, class U, class V>
	struct VertexPNT8 {
		T mP;
		U mN;
		V mT0, mT1, mT2, mT3, mT4, mT5, mT6, mT7;
	};
	
	// Specific types - Generally in order of Position, Normal, Colour, Textures
	// Not sure where Tangents come in here
	
	typedef VertexP<glm::vec3> VertPG;
	typedef VertexP<Float3> VertPF;
	typedef VertexP<Double3> VertPD;
	typedef VertexPN<glm::vec3, glm::vec3> VertPNG;
	typedef VertexPN<Float3, Float3> VertPNF;
	typedef VertexPN<Double3, Double3> VertPND;

	typedef VertexPNT<glm::vec3, glm::vec3, glm::vec2> VertPNTG;
	typedef VertexPNT<Float3, Float3, Float2> VertPNTF;
	typedef VertexPNT<Double3, Double3, Double2> VertPNTD;

	typedef VertexPNCT<glm::vec3, glm::vec3,glm::vec4, glm::vec2 > VertPNCTG;
	typedef VertexPNCT<Float3, Float3, Float4, Float2> VertPNCTF;
	typedef VertexPNCT<Double3, Double3, Double4, Double2> VertPNCTD;
	
	typedef VertexPNC<glm::vec3, glm::vec3, glm::vec4> VertPNCG;
	typedef VertexPNC<Float3, Float3, Float4> VertPNCF;
	typedef VertexPNC<Double3, Double3, Double4> VertPNCD;
	
	typedef VertexPCT<glm::vec3, glm::vec4, glm::vec2> VertPCTG;
	typedef VertexPCT<Float3, Float4, Float2> VertPCTF;
	typedef VertexPCT<Double3, Double4, Double2> VertPCTD;


#pragma pack(pop)  

}

#endif
