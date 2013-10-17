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
 * Build a Quad with w,h
 */


Quad::Quad (float w, float h) {
	float w2 = w/2;
	float h2 = h/2;

}


/**
 * Build a Cuboid with w,h,d centred at the origin
 */

Cuboid::Cuboid (float w, float h, float d) : obj_(std::shared_ptr<SharedObj> (new SharedObj()))  {

	float w2 = w/2.0;
	float h2 = h/2.0;
	float d2 = d/2.0;

	Vertex4 aa ( glm::vec4(-w2,  h2, -d2, 1.0f));
	Vertex4 ab ( glm::vec4(-w2,  h2,  d2, 1.0f));
	Vertex4 ac ( glm::vec4(-w2, -h2, -d2, 1.0f));
	Vertex4 ad ( glm::vec4(-w2, -h2,  d2, 1.0f));

	Vertex4 ba ( glm::vec4( w2,  h2, -d2, 1.0f));
	Vertex4 bb ( glm::vec4( w2,  h2,  d2, 1.0f));
	Vertex4 bc ( glm::vec4( w2, -h2, -d2, 1.0f));
	Vertex4 bd ( glm::vec4( w2, -h2,  d2, 1.0f));

	obj_->geometry[0] = aa;
	obj_->geometry[1] = ab;
	obj_->geometry[2] = ac;
	obj_->geometry[3] = ad;

	obj_->geometry[4] = ba;
	obj_->geometry[5] = bb;
	obj_->geometry[6] = bc;
	obj_->geometry[7] = bd;

	IndicesType indices[36] = {0,3,1,0,2,3,0,1,5,0,5,4,1,3,7,1,7,5,6,7,5,6,4,5,4,2,6,4,0,2,2,7,3,2,6,7};

	obj_->geometry.setIndices(indices);

	
}