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
		p.getVBO().vIndices += 0,3,1,3,2,1;
		p.getVBO().vVertices += 0.0f,0.0f,0.0f,
			w, 0.0f,0.0f, 
			w, h,0.0f,
			0.0f, h,0.0f;
		
		p.getVBO().vTexCoords += 0.0, h,
			w, h,
			w, 0.0,
			0.0,0.0;
		
		p.getVBO().vColours += 1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f;
			
		p.getVBO().compile(VBO_VERT | VBO_IDCE | VBO_TEXC | VBO_COLR );
	}

	void makeReferenceQuad(Primitive &p, float_t w, float_t h) {
		p.make();
		
		// Quad for Camera drawing
		p.getVBO().vIndices += 0,3,1,3,2,1;
		p.getVBO().vVertices += 0.0f,0.0f,0.0f,
			w, 0.0f,0.0f, 
			w, h,0.0f,
			0.0f, h,0.0f;
		
		p.getVBO().vTexCoords += 0.0, h,
			w, h,
			w, 0.0,
			0.0,0.0;
		
		p.getVBO().vColours += 0.0f,0.0f,0.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f;
			
		p.getVBO().compile(VBO_VERT | VBO_IDCE | VBO_TEXC | VBO_COLR );
		
		CXGLERROR
	}
	
	void makeReferenceTriangle(Primitive &p, float_t w, float_t h){
		
		p.make();
		
		// Quad for Camera drawing
		p.getVBO().vIndices += 0,1,2;
		p.getVBO().vVertices += 0.0f,0.0f,0.0f,
			w, 0.0f,0.0f, 
			w/2, h,0.0f;

		p.getVBO().vColours += 0.0f,0.0f,0.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f;
			
		p.getVBO().compile(VBO_VERT | VBO_IDCE | VBO_COLR );
		
		CXGLERROR
	}
	
	/*
	 * Create a bounding box, centrered around 0,0,0
	 */
	
	void makeBoundingBox(Primitive &p, float_t w, float_t h, float_t d, float_t s){
		p.make();
		
		glm::vec3 corners[8];
		corners[0] = glm::vec3(-w/2.0, h/2.0, -d/2.0);
		corners[1] = glm::vec3(w/2.0, h/2.0, -d/2.0);
		corners[2] = glm::vec3(-w/2.0, h/2.0, d/2.0);
		corners[3] = glm::vec3(w/2.0, h/2.0, d/2.0);
		corners[4] = glm::vec3(-w/2.0, -h/2.0, -d/2.0);
		corners[5] = glm::vec3(w/2.0, -h/2.0, -d/2.0);
		corners[6] = glm::vec3(-w/2.0, -h/2.0, d/2.0);
		corners[7] = glm::vec3(w/2.0, -h/2.0, d/2.0);
		
		for (int i=0; i < 8; i++){
			p.getVBO().vVertices += corners[i].x, corners[i].y, corners[i].z; 
			
			glm::vec3 m = glm::vec3(corners[i].x + s, corners[i].y + s, corners[i].z + s); 
			glm::vec3 n = glm::vec3(corners[i].x + s, corners[i].y + s, corners[i].z + s);
			glm::vec3 o = glm::vec3(corners[i].x + s, corners[i].y + s, corners[i].z + s);
			
			if ( corners[i].x > 0){
				m.x = corners[i].x - s;
				n.x = corners[i].x - s;
				o.x = corners[i].x - s;
			}
			
			if ( corners[i].y > 0){
				m.y = corners[i].y - s;
				n.y = corners[i].y - s;
				o.y = corners[i].y - s;
			}
			
			if ( corners[i].z > 0){
				m.z = corners[i].z - s;
				n.z = corners[i].z - s;
				o.z = corners[i].z - s;
			}
			
		}
				
		p.getVBO().vVertices += 0.0f,0.0f,0.0f,
			w, 0.0f,0.0f, 
			w/2, h,0.0f;
	
	}	
}
