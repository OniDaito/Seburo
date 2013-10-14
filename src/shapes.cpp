/**
* @brief Shapes Classes
* @file shapes.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 14/10/2013
*
*/


#include "s9/shapes.hpp"

using namespace std;
using namespace s9;
using namespace s9::gl;



/**
 * Build a Cuboid with w,h,d centred at the origin
 */

Cuboid::Cuboid (float w, float h, float d) : obj_(std::shared_ptr<SharedObj> (new SharedObj()))  {

	float w2 = w/2;
	float h2 = h/2;
	float d2 = d/2;

	Vertex4 aa ( glm::vec4(-w2,  h2, -d2, 1));
	Vertex4 ab ( glm::vec4(-w2,  h2,  d2, 1));
	Vertex4 ac ( glm::vec4(-w2, -h2, -d2, 1));
	Vertex4 ad ( glm::vec4(-w2, -h2,  d2, 1));

	Vertex4 ba ( glm::vec4( w2,  h2, -d2, 1));
	Vertex4 bb ( glm::vec4( w2,  h2,  d2, 1));
	Vertex4 bc ( glm::vec4( w2, -h2, -d2, 1));
	Vertex4 bd ( glm::vec4( w2, -h2,  d2, 1));



	
	// Add the quads
/*
	Quad3 front (&(obj_->vertices[0]), &(obj_->vertices[1]), &(obj_->vertices[2]), &(obj_->vertices[3]));
	Quad3 back	(&(obj_->vertices[7]), &(obj_->vertices[6]), &(obj_->vertices[5]), &(obj_->vertices[4]));
	Quad3 left	(&(obj_->vertices[0]), &(obj_->vertices[3]), &(obj_->vertices[7]), &(obj_->vertices[4]));
	Quad3 right	(&(obj_->vertices[1]), &(obj_->vertices[5]), &(obj_->vertices[6]), &(obj_->vertices[2]));
	Quad3 top	(&(obj_->vertices[0]), &(obj_->vertices[4]), &(obj_->vertices[5]), &(obj_->vertices[1]));
	Quad3 bottom(&(obj_->vertices[2]), &(obj_->vertices[6]), &(obj_->vertices[7]), &(obj_->vertices[3]));

	obj_->primitives.push_back(front);
	obj_->primitives.push_back(back);
	obj_->primitives.push_back(left);
	obj_->primitives.push_back(right);
	obj_->primitives.push_back(top);
	obj_->primitives.push_back(bottom);

	generateIndicesFromGeometry();
*/
}