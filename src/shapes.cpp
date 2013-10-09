/**
* @brief Shapes Classes
* @file shapes.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 06/07/2012
*
*/

#include "s9/shapes.hpp"

using namespace std;
using namespace s9;

/**
 * generateIndices - generate indices from a pointer cloud
 * This is a bit nasty but needed really.
 */

template <class T, class U>
void ShapeT<T,U>::generateIndicesFromGeometry() {

	obj_->indices.clear();
	obj_->indices_by_ref.clear();

	for (GeometryT<U> g : obj_->geometry){
		for (T *a : obj_->geometry.indices){
			for (uint32_t i =0; i < obj_->vertices.size(); ++i){
				T b = obj_->vertices[i];
				
				if (a == &b){
					obj_->indices.push_back(i);
					obj_->indices_by_ref.push_back(a);
				}
			}
		}
	}
}

/**
 * generateRefFromIndices - Given indices create our geometry
 */

template <class T, class U>
void ShapeT<T,U>::generateGeometryFromIndices() {}


/**
 * Build a Cuboid with w,h,d centred at the origin
 */

Cuboid::Cuboid (float w, float h, float d)  {

	float w2 = w/2;
	float h2 = h/2;
	float d2 = d/2;

	Vertex3 aa ( glm::vec3(-w2,  h2, -d2));
	Vertex3 ab ( glm::vec3(-w2,  h2,  d2));
	Vertex3 ac ( glm::vec3(-w2, -h2, -d2));
	Vertex3 ad ( glm::vec3(-w2, -h2,  d2));

	Vertex3 ba ( glm::vec3( w2,  h2, -d2));
	Vertex3 bb ( glm::vec3( w2,  h2,  d2));
	Vertex3 bc ( glm::vec3( w2, -h2, -d2));
	Vertex3 bd ( glm::vec3( w2, -h2,  d2));

	obj_->vertices = { aa, ab, ac, ad, bb, ba, bb, bc, bd };
	
	// clockwise winding
	obj_->indices_by_ref = { &(obj_->vertices[0]), &(obj_->vertices[1]), &(obj_->vertices[2]), &(obj_->vertices[3]),
	&(obj_->vertices[7]), &(obj_->vertices[6]), &(obj_->vertices[5]), &(obj_->vertices[4]),
	&(obj_->vertices[0]),	&(obj_->vertices[4]),	&(obj_->vertices[5]),	&(obj_->vertices[1]),
	&(obj_->vertices[1]), &(obj_->vertices[5]), &(obj_->vertices[6]), &(obj_->vertices[2]),
	&(obj_->vertices[2]), &(obj_->vertices[6]), &(obj_->vertices[7]), &(obj_->vertices[3]),
	&(obj_->vertices[0]), &(obj_->vertices[3]), &(obj_->vertices[7]), &(obj_->vertices[4]) };

	// Add the quads

	Quad3 front (&(obj_->vertices[0]), 	&(obj_->vertices[1]), &(obj_->vertices[2]), &(obj_->vertices[3]));
	Quad3 back	(&(obj_->vertices[7]), 	&(obj_->vertices[6]), &(obj_->vertices[5]), &(obj_->vertices[4]));
	Quad3 left	(&(obj_->vertices[0]), 	&(obj_->vertices[3]), &(obj_->vertices[7]), &(obj_->vertices[4]));
	Quad3 right	(&(obj_->vertices[1]), 	&(obj_->vertices[5]), &(obj_->vertices[6]), &(obj_->vertices[2]));
	Quad3 top		(&(obj_->vertices[0]),	&(obj_->vertices[4]),	&(obj_->vertices[5]),	&(obj_->vertices[1]));
	Quad3 bottom(&(obj_->vertices[2]), 	&(obj_->vertices[6]), &(obj_->vertices[7]), &(obj_->vertices[3]));

	obj_->geometry.push_back(front);
	obj_->geometry.push_back(back);
	obj_->geometry.push_back(left);
	obj_->geometry.push_back(right);
	obj_->geometry.push_back(top);
	obj_->geometry.push_back(bottom);

	obj_->indices = { 0,1,2,3,7,6,5,4,0,4,5,1,1,5,6,2,6,7,3,0,3,7,4 };
}