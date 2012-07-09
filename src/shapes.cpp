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

	void makeQuad(Primitive &p, float_t w, float_t h) {
		
		p.make();
		
		// Quad for Camera drawing
		p.getVBO().mIndices += 0,3,1,3,2,1;
		p.getVBO().mVertices += 0.0f,0.0f,0.0f,
			w, 0.0f,0.0f, 
			w, h,0.0f,
			0.0f, h,0.0f;
		
		p.getVBO().mTexCoords += 0.0, h,
			w, h,
			w, 0.0,
			0.0,0.0;
		
		p.getVBO().mColours += 1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f;
			
		p.getVBO().compile(VBO_VERT | VBO_IDCE | VBO_TEXC | VBO_COLR );
	}

	void makeReferenceQuad(Primitive &p, float_t w, float_t h) {
		p.make();
		
		// Quad for Camera drawing
		p.getVBO().mIndices += 0,3,1,3,2,1;
		p.getVBO().mVertices += 0.0f,0.0f,0.0f,
			w, 0.0f,0.0f, 
			w, h,0.0f,
			0.0f, h,0.0f;
		
		p.getVBO().mTexCoords += 0.0, h,
			w, h,
			w, 0.0,
			0.0,0.0;
		
		p.getVBO().mColours += 0.0f,0.0f,0.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f;
			
		p.getVBO().compile(VBO_VERT | VBO_IDCE | VBO_TEXC | VBO_COLR );
		
		CXGLERROR
	}
	
	void makeReferenceTriangle(Primitive &p, float_t w, float_t h){
		
		p.make();
		
		// Quad for Camera drawing
		p.getVBO().mIndices += 0,1,2;
		p.getVBO().mVertices += 0.0f,0.0f,0.0f,
			w, 0.0f,0.0f, 
			w/2, h,0.0f;

		p.getVBO().mColours += 0.0f,0.0f,0.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f;
			
		p.getVBO().compile(VBO_VERT | VBO_IDCE | VBO_COLR );
		
		CXGLERROR
	}
	
}
