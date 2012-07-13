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

	Primitive makeQuad(float_t w, float_t h) {
		
		VBOBuffer<GLuint> indices(1);
		VBOBuffer<GLfloat> verts(3);
		VBOBuffer<GLfloat> texcoords(2);
		VBOBuffer<GLfloat> colours(4);
		
		// Quad for Camera drawing
		indices += 0,3,1,3,2,1;
		verts += 0.0f,0.0f,0.0f,
			w, 0.0f,0.0f, 
			w, h,0.0f,
			0.0f, h,0.0f;
		
		texcoords += 0.0, h,
			w, h,
			w, 0.0,
			0.0,0.0;
		
		colours += 1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f;
		
		
		VBOData v;
		v.push_back(verts);
		v.push_back(indices);
		v.push_back(texcoords);
		v.push_back(colours);
		v.compile();
		
		Primitive p(v);
	
		return p;
	}

	Primitive makeReferenceQuad(float_t w, float_t h) {
		
		VBOBuffer<GLuint> indices(1);
		VBOBuffer<GLfloat> verts(3);
		VBOBuffer<GLfloat> texcoords(2);
		VBOBuffer<GLfloat> colours(4);
		
		
		// Quad for Camera drawing
		indices += 0,3,1,3,2,1;
		verts += 0.0f,0.0f,0.0f,
			w, 0.0f,0.0f, 
			w, h,0.0f,
			0.0f, h,0.0f;
		
		texcoords += 0.0, h,
			w, h,
			w, 0.0,
			0.0,0.0;
		
		colours += 0.0f,0.0f,0.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f;
			
			
		VBOData v;
		v.push_back(verts);
		v.push_back(indices);
		v.push_back(texcoords);
		v.push_back(colours);
		v.compile();
		
		Primitive p(v);
		
		CXGLERROR
		
		return p;
	}
	
	Primitive makeReferenceTriangle(float_t w, float_t h){
		
		VBOBuffer<GLuint> indices(1);
		VBOBuffer<GLfloat> verts(3);
		VBOBuffer<GLfloat> texcoords(2);
		VBOBuffer<GLfloat> colours(4);
		
		// Quad for Camera drawing
		indices += 0,1,2;
		verts += 0.0f,0.0f,0.0f,
			w, 0.0f,0.0f, 
			w/2, h,0.0f;

		colours += 0.0f,0.0f,0.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f;
			
		VBOData v;
		v.push_back(verts);
		v.push_back(indices);
		v.push_back(colours);
		v.compile();
		
		Primitive p(v);
		
		CXGLERROR
		
		return p;
	}
	
	/*
	 * Create a bounding box, centrered around 0,0,0
	 */
	
	Primitive makeBoundingBox(float_t w, float_t h, float_t d, float_t s){
		
		VBOBuffer<GLfloat> verts(3);
		
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
			verts += corners[i].x, corners[i].y, corners[i].z; 
			
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
				
		verts += 0.0f,0.0f,0.0f,
			w, 0.0f,0.0f, 
			w/2, h,0.0f;
			
		VBOData v;
		v.push_back(verts);

		Primitive p(v);
		
		CXGLERROR
		
		return p;
	
	}	
}
