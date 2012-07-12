/**
* @brief VBO Classes
* @file vbo.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/


#ifndef VBO_HPP
#define VBO_HPP

#include "common.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace s9 {

	/*
	 * VBO Data class to hold our primitives
	 */

	#define VBO_VERT 0x01
	#define VBO_IDCE 0x02
	#define VBO_COLR 0x04
	#define VBO_NORM 0x08
	#define VBO_TEXC 0x10
	#define VBO_TEXI 0x20
	 
	class VBOData {
	public:
		
		VBOData();
		~VBOData();
		
		void compile(size_t buffers);
		
		void bind();
		void unbind();
		
		void allocateColours();
		void allocateVertices();
		void allocateTexCoords();
		void allocateNormals();
		void allocateIndices();
		void allocateTexIDs();
		
		size_t mUsed;
		size_t mNumBufs;
		size_t mVID;
		size_t mIID;
		size_t mCID;
		size_t mNID;
		size_t mTID;
		size_t mTTD;
		
		GLuint mNumElements;				// May be smaller than the size of the arrays due to buffering
		GLuint mNumIndices;					// Different to the above remmeber
		
		GLuint vao, *vbo;
		std::vector<GLfloat> vVertices;		// Vertices as 3 floats
		std::vector<GLuint> vIndices; 		// Indicies into the above array. Multiply by 3
		std::vector<GLfloat> vTexCoords;	// texture coordinates in u,v or pixel coords
		std::vector<GLfloat> vNormals;		// normals as 3 floats
		std::vector<GLfloat> vColours;		// colours as 3 or 4 floats
		std::vector<GLuint> vTexIDs;		// texids as indicies to textures per vertex - samplers basically
		
	}; 
}

#endif

