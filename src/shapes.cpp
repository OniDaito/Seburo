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

	obj_->geometry[0] = aa;
	obj_->geometry[1] = ab;
	obj_->geometry[2] = ac;
	obj_->geometry[3] = ac;

	obj_->geometry[4] = ba;
	obj_->geometry[5] = bb;
	obj_->geometry[6] = bc;
	obj_->geometry[7] = bc;

	IndicesType idx = 0;
	obj_->geometry.setIndex(idx++,0);
	obj_->geometry.setIndex(idx++,1);
	obj_->geometry.setIndex(idx++,2);
	obj_->geometry.setIndex(idx++,3);

	obj_->geometry.setIndex(idx++,7);
	obj_->geometry.setIndex(idx++,6);
	obj_->geometry.setIndex(idx++,5);
	obj_->geometry.setIndex(idx++,5);

	obj_->geometry.setIndex(idx++,0);
	obj_->geometry.setIndex(idx++,3);
	obj_->geometry.setIndex(idx++,7);
	obj_->geometry.setIndex(idx++,4);

	obj_->geometry.setIndex(idx++,1);
	obj_->geometry.setIndex(idx++,5);
	obj_->geometry.setIndex(idx++,6);
	obj_->geometry.setIndex(idx++,2);

	obj_->geometry.setIndex(idx++,0);
	obj_->geometry.setIndex(idx++,4);
	obj_->geometry.setIndex(idx++,5);
	obj_->geometry.setIndex(idx++,1);

	obj_->geometry.setIndex(idx++,2);
	obj_->geometry.setIndex(idx++,6);
	obj_->geometry.setIndex(idx++,7);
	obj_->geometry.setIndex(idx++,3);
	
}