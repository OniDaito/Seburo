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


Quad::Quad (float w, float h) : Shape(true)  {

  std::shared_ptr<ShapeObjQuad> quad;
	quad = std::make_shared<ShapeObjQuad>();

	float w2 = w/2;
	float h2 = h/2;

	Vertex4 aa ( glm::vec4(-w2,	-h2, 0.0f, 1.0f), glm::vec4(.0f, .0f, 1.0f, 1.0f), glm::vec4(1.0f), glm::vec2(0.0f,1.0f));
	Vertex4 ab ( glm::vec4(w2, 	-h2, 0.0f, 1.0f), glm::vec4(.0f, .0f, 1.0f, 1.0f), glm::vec4(1.0f), glm::vec2(1.0f,1.0f));
	Vertex4 ac ( glm::vec4(w2, 	 h2, 0.0f, 1.0f), glm::vec4(.0f, .0f, 1.0f, 1.0f), glm::vec4(1.0f), glm::vec2(1.0f,0.0f));
	Vertex4 ad ( glm::vec4(-w2,  h2, 0.0f, 1.0f), glm::vec4(.0f, .0f, 1.0f, 1.0f), glm::vec4(1.0f), glm::vec2(0.0f,0.0f));

	quad->geometry[0] = aa;
	quad->geometry[1] = ab;
	quad->geometry[2] = ac;
	quad->geometry[3] = aa;
	quad->geometry[4] = ac;
	quad->geometry[5] = ad;

	// Now set the shared pointer member
	obj_ = std::static_pointer_cast<ShapeObj>(quad);


	// No indices here - no real need

}

const GeometryT<Vertex4, Face4, AllocationPolicyNew>* Quad::geometry() {
	std::shared_ptr<ShapeObjQuad> t = std::static_pointer_cast<ShapeObjQuad>(obj_) ;
	return &(t->geometry);
}

/*
shared_ptr< GeometryT<Vertex4, Face4, AllocationPolicyNew> > Quad::geometry() {
	std::shared_ptr<ShapeObjQuad> t = std::static_pointer_cast<ShapeObjQuad>(obj_) ;
	return std::make_shared<  GeometryT<Vertex4, Face4, AllocationPolicyNew> > (t);
}*/


/**
 * Build a Cuboid with w,h,d centred at the origin
 */

Cuboid::Cuboid (float w, float h, float d)  : Shape(true) {

	std::shared_ptr<ShapeObjCuboid> cuboid;
	cuboid = std::make_shared<ShapeObjCuboid>();

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

	cuboid->geometry[0] = aa;
	cuboid->geometry[1] = ab;
	cuboid->geometry[2] = ac;
	cuboid->geometry[3] = ad;

	cuboid->geometry[4] = ba;
	cuboid->geometry[5] = bb;
	cuboid->geometry[6] = bc;
	cuboid->geometry[7] = bd;

	IndicesType indices[36] = {0,3,1,0,2,3,0,1,5,0,5,4,1,3,7,1,7,5,6,7,5,6,4,5,4,2,6,4,0,2,2,7,3,2,6,7};

	cuboid->geometry.setIndices(indices);

	obj_ = std::static_pointer_cast<ShapeObj>(cuboid);

}

const GeometryT<Vertex4, Face4, AllocationPolicyNew>* Cuboid::geometry() {
	std::shared_ptr<ShapeObjCuboid> t = std::static_pointer_cast<ShapeObjCuboid>(obj_) ;
	return &(t->geometry);
}


/**
 * Build a basic TriMesh with a set number of verts and indices
 */

TriMesh::TriMesh(size_t num_verts, size_t num_indices) {
	std::shared_ptr<ShapeObjTriMesh> trimesh;
	trimesh = std::make_shared<ShapeObjTriMesh>(num_verts, num_indices);


	obj_ = std::static_pointer_cast<ShapeObj>(trimesh);
}

const GeometryT<Vertex3, Face3, AllocationPolicyNew>* TriMesh::geometry() {
	std::shared_ptr<ShapeObjTriMesh> t = std::static_pointer_cast<ShapeObjTriMesh>(obj_) ;
	return &(t->geometry);
}

