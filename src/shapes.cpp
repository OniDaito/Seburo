/**
* @brief Shapes Classes
* @file shapes.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 06/07/2012
*
*/

#include "shapes.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign; 

namespace s9{

	void makeQuad(PrimPtr p, float_t w, float_t h) {
		
		VBOData vbo = p->getVBO();
		
		// Quad for Camera drawing
		vbo.mIndices += 0,3,1,3,2,1;
		vbo.mVertices += 0.0f,0.0f,0.0f,
			w, 0.0f,0.0f, 
			w, h,0.0f,
			0.0f, h,0.0f;
		
		vbo.mTexCoords += 0.0, h,
			w, h,
			w, 0.0,
			0.0,0.0;
		
			
		vbo.mColours += 1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f;
			
		vbo.compile(VBO_VERT | VBO_IDCE | VBO_TEXC | VBO_COLR );
	}

	void makeReferenceQuad(PrimPtr p, float_t w, float_t h) {
		
		VBOData vbo = p->getVBO();
		
		// Quad for Camera drawing
		vbo.mIndices += 0,3,1,3,2,1;
		vbo.mVertices += 0.0f,0.0f,0.0f,
			w, 0.0f,0.0f, 
			w, h,0.0f,
			0.0f, h,0.0f;
		
		vbo.mTexCoords += 0.0, h,
			w, h,
			w, 0.0,
			0.0,0.0;
		
		vbo.mColours += 0.0f,0.0f,0.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f;
			
		vbo.compile(VBO_VERT | VBO_IDCE | VBO_TEXC | VBO_COLR );
	}
}
