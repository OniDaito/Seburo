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

/**
 * Build a Quad with w,h
 */


Quad::Quad (float w, float h) : Shape()  {

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

GeometryT<Vertex4, Face4, AllocationPolicyNew>* Quad::geometry() {
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

Cuboid::Cuboid (float w, float h, float d)  : Shape() {

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

	cuboid->geometry.SetIndices(indices);

	obj_ = std::static_pointer_cast<ShapeObj>(cuboid);

}

GeometryT<Vertex4, Face4, AllocationPolicyNew>* Cuboid::geometry() {
	std::shared_ptr<ShapeObjCuboid> t = std::static_pointer_cast<ShapeObjCuboid>(obj_) ;
	return &(t->geometry);
}



/**
 * Build a Sphere with triangles, give radius and segements
 */

Sphere::Sphere (float_t radius, size_t segments)  : Shape() {

	size_t vidx = 0;
	size_t num_vertices = segments / 2 * ((segments+2) * 2);
	size_t num_indices = (num_vertices - 3) * 3;

	IndicesType indices[num_indices];

	std::shared_ptr<ShapeObjSphere> sphere;
	sphere = std::make_shared<ShapeObjSphere>(num_vertices, num_indices );

	glm::vec4 colour(1.0f);

	for (size_t j = 0; j < segments / 2; ++j){
		float_t theta1 = j * 2  * PI / segments - (PI / 2);
    float_t theta2 = (j + 1) * 2 * PI / segments - (PI / 2);

    // Vertices

    for (size_t i=0; i < segments + 2; ++i){
    	float_t theta3 = i * 2 * PI / segments;
    	
    	glm::vec4 normal (static_cast<float_t>(cos(theta1) * cos(theta3)),
    		static_cast<float_t>(sin(theta1)),
    		static_cast<float_t>(cos(theta1) * sin(theta3)),
    		0.0f);

    	normal = glm::normalize(normal);

    	glm::vec4 position = normal * radius;
    	glm::vec2 texture = glm::vec2( 0.999f - i / segments, 0.999f - 2 * j / segments);
    	position.w = 1.0f;
    	normal.w = 1.0f;

    	Vertex4 aa (position, normal, colour, texture);

    	sphere->geometry[vidx] = aa;
    	vidx++;

    	glm::vec4 normal1 (static_cast<float_t>(cos(theta2) * cos(theta3)),
    		static_cast<float_t>(sin(theta2)),
    		static_cast<float_t>(cos(theta2) * sin(theta3)),
    		0.0f);

    	normal1 = glm::normalize(normal1);

    	glm::vec4 position1 = normal1 * radius;
    	position1.w = 1.0f;
    	normal1.w = 1.0f;

    	glm::vec2 texture1 = glm::vec2( 0.999f - i / segments, 0.999f - 2 * j / segments);

    	Vertex4 bb (position1, normal1, colour, texture1);

    	sphere->geometry[vidx] = bb;
    	vidx++;

   
    }
  }

  // Indices

  for (size_t i = 2; i < num_vertices; ++i ){
  	if ( i % 2 ==0){
  		indices[i*3] = i;
  		indices[i*3+1] = i-1;
  		indices[i*3+2] = i-2;
  	} else {
  		indices[i*3] = i;
  		indices[i*3+1] = i-2;
  		indices[i*3+2] = i-1;
  	}
 
 	}
	
	sphere->geometry.SetIndices(indices);
	obj_ = std::static_pointer_cast<ShapeObj>(sphere);

}

GeometryT<Vertex4, Face4, AllocationPolicyNew>* Sphere::geometry() {
	std::shared_ptr<ShapeObjSphere> t = std::static_pointer_cast<ShapeObjSphere>(obj_) ;
	return &(t->geometry);
}

/**
 * Build a basic TriMesh with a set number of verts and indices
 */

TriMesh::TriMesh(IndicesType num_verts, size_t num_indices) : Shape() {
	std::shared_ptr<ShapeObjTriMesh> trimesh;
	trimesh = std::make_shared<ShapeObjTriMesh>(num_verts, num_indices);
	obj_ = std::static_pointer_cast<ShapeObj>(trimesh);
}

GeometryT<Vertex3, Face3, AllocationPolicyNew>* TriMesh::geometry() {
	std::shared_ptr<ShapeObjTriMesh> t = std::static_pointer_cast<ShapeObjTriMesh>(obj_) ;
	return &(t->geometry);
}

/**
 * Build a basic TriMeshSkinned with a set number of verts and indices
 */

TriMeshSkinned::TriMeshSkinned(IndicesType num_verts, size_t num_indices) : Shape() {
	std::shared_ptr<ShapeObjTriMeshSkinned> trimesh;
	trimesh = std::make_shared<ShapeObjTriMeshSkinned>(num_verts, num_indices);
	obj_ = std::static_pointer_cast<ShapeObj>(trimesh);
}

GeometryT<Vertex3Skin, Face3, AllocationPolicyNew>* TriMeshSkinned::geometry() {
	std::shared_ptr<ShapeObjTriMeshSkinned> t = std::static_pointer_cast<ShapeObjTriMeshSkinned>(obj_) ;
	return &(t->geometry);
}



/**
 * Build a basic Cylinder with segments being the number of vertices around the edge
 * and stacks being the number of segments along the length
 */


Cylinder::Cylinder (size_t segments, size_t stacks, float radius, float height){

	// Stacks must always be 1 or more
	if (stacks == 0 ) stacks = 1;

  IndicesType num_verts = (segments * 2 + 2) + ((stacks -1) * segments);
  size_t num_indices = ((segments * 2) + (stacks * segments * 2) ) * 3;


	std::shared_ptr<ShapeObjCylinder> cylinder;
	cylinder = std::make_shared<ShapeObjCylinder>(num_verts, num_indices);

	float top = height/2.0f;
	float bottom = -top;

	IndicesType indices[num_indices];

	// Top cap
	size_t idx = 0;
	Vertex3 top_center ( glm::vec3( 0, top, 0));
	cylinder->geometry[idx] = top_center;
	idx++;

	for (size_t i = 0; i < segments; ++i){
		float step = static_cast<float>( DegToRad( (360.0f / segments) * i));
		Vertex3 aa ( glm::vec3( sin(step) * radius, top, cos(step) * radius ));
		cylinder->geometry[idx + i] = aa;
	}

	idx += segments;

	for (size_t j = 1; j < stacks; ++j){
		for (size_t i = 0; i < segments; ++i){
			float step = static_cast<float>( DegToRad( (360.0f / segments) * i));
			Vertex3 aa ( glm::vec3( sin(step) * radius, height / stacks * j,  cos(step) * radius));
			cylinder->geometry[idx + i] = aa;
		}
		idx += segments;
	}

	// Bottom Cap
	
	for (size_t i = 0; i < segments; ++i){
		float step = static_cast<float>( DegToRad( (360.0f / segments) * i));
		Vertex3 aa ( glm::vec3( sin(step) * radius, bottom,  cos(step) * radius));
		cylinder->geometry[idx + i] = aa;
	}

	idx += segments;

	Vertex3 bottom_center ( glm::vec3( 0, bottom, 0));
	cylinder->geometry[idx] = bottom_center;
	idx++;

	// Indices

	// Top Cap
	for (size_t i = 0; i < segments; ++i){
		indices[i * 3] = 0;
		indices[i * 3 + 1] = i+1;
		if (i + 1 < segments)
			indices[i * 3 + 2] = i+2;
		else
			indices[i * 3 + 2] = 1;

	}

	size_t indices_base = segments * 3;
	size_t vertex_base = 1;
	// Stacks  / sides
	for (size_t j = 0; j < stacks; ++j){
		for (size_t i = 0; i < segments; ++i){
			// First triangle 
			indices[indices_base ]	= vertex_base + i;
			indices[indices_base + 1]	= vertex_base + segments + i;
			
			if (i + 1 < segments)
				indices[indices_base + 2]	= vertex_base + i + 1;
			else
				indices[indices_base + 2]	= vertex_base;

			// second triangle
			if (i + 1 < segments)
				indices[indices_base + 3]	= vertex_base + i + 1;
			else
				indices[indices_base + 3]	= vertex_base;

			indices[indices_base + 4]	= vertex_base + segments + i;
			
			if (i + 1 < segments)
				indices[indices_base + 5]	= vertex_base + segments + i + 1;
			else
				indices[indices_base + 5]	= vertex_base + segments;

			indices_base += 6;
		}
		vertex_base += segments;
	}

	// Bottom cap - reverse direction :)
	vertex_base = num_verts - segments - 1;
	for (size_t i = 0; i < segments; ++i){
		
		indices[i * 3 + indices_base] = vertex_base + segments;

		if (i + 1 < segments)
			indices[i * 3 + 1 + indices_base] = vertex_base + i+1;
		else
			indices[i * 3 + 1 + indices_base] = vertex_base;

		indices[i * 3 + 2 + indices_base] = vertex_base + i;
		
	}


	cylinder->geometry.SetIndices(indices);
	obj_ = std::static_pointer_cast<ShapeObj>(cylinder);

}

GeometryT<Vertex3, Face3, AllocationPolicyNew>* Cylinder::geometry() {
	std::shared_ptr<ShapeObjCylinder> t = std::static_pointer_cast<ShapeObjCylinder>(obj_) ;
	return &(t->geometry);
}



/**
 * Builds a spike with a rounded bottom (or any pyramid basically)
 */


Spike::Spike (size_t segments, size_t stacks, float radius, float height){

	// Stacks must always be 1 or more
	if (stacks == 0 ) stacks = 1;

  IndicesType num_verts = (segments + 2) + ((stacks -1) * segments);
  size_t num_indices = ((segments * 2) + ((stacks-1) * segments * 2) ) * 3;


	std::shared_ptr<ShapeObjSpike> spike;
	spike = std::make_shared<ShapeObjSpike>(num_verts, num_indices);

	float top = height/2.0f;
	float bottom = -top;

	IndicesType indices[num_indices];

	// Bottom cap
	size_t idx = 0;
	Vertex3 bottom_center ( glm::vec3( 0, top, 0));
	spike->geometry[idx] = bottom_center;
	idx++;

	for (size_t i = 0; i < segments; ++i){
		float step = static_cast<float>( DegToRad( (360.0f / segments) * i));
		Vertex3 aa ( glm::vec3( sin(step) * radius, bottom, cos(step) * radius ));
		spike->geometry[idx + i] = aa;
	}

	idx += segments;

	for (size_t j = 1; j < stacks; ++j){

		float th = height / stacks * j;
		float tr = radius / stacks * j;


		for (size_t i = 0; i < segments; ++i){
			float step = static_cast<float>( DegToRad( (360.0f / segments) * i));
			Vertex3 aa ( glm::vec3( sin(step) * tr, th,  cos(step) * tr));
			spike->geometry[idx + i] = aa;
		}
		idx += segments;
	}

	// Top point

	Vertex3 top_centre ( glm::vec3( 0, top, 0));
	spike->geometry[idx] = top_centre;
	idx++;

	// Indices

	// Top Cap
	for (size_t i = 0; i < segments; ++i){
		indices[i * 3] = 0;
		indices[i * 3 + 1] = i+1;
		if (i + 1 < segments)
			indices[i * 3 + 2] = i+2;
		else
			indices[i * 3 + 2] = 1;

	}

	size_t indices_base = segments * 3;
	size_t vertex_base = 1;


	
	// Stacks  / sides
	for (size_t j = 1; j < stacks; ++j){
		for (size_t i = 0; i < segments; ++i){
			// First triangle 
			indices[indices_base ]	= vertex_base + i;
			indices[indices_base + 1]	= vertex_base + segments + i;
			
			if (i + 1 < segments)
				indices[indices_base + 2]	= vertex_base + i + 1;
			else
				indices[indices_base + 2]	= vertex_base;

			// second triangle
			if (i + 1 < segments)
				indices[indices_base + 3]	= vertex_base + i + 1;
			else
				indices[indices_base + 3]	= vertex_base;

			indices[indices_base + 4]	= vertex_base + segments + i;
			
			if (i + 1 < segments)
				indices[indices_base + 5]	= vertex_base + segments + i + 1;
			else
				indices[indices_base + 5]	= vertex_base + segments;

			indices_base += 6;
		}
		vertex_base += segments;
	}
	

	// Bottom cap - reverse direction :)
	vertex_base = num_verts - segments - 1;
	for (size_t i = 0; i < segments; ++i){
		
		indices[i * 3 + indices_base] = vertex_base + segments;

		if (i + 1 < segments)
			indices[i * 3 + 1 + indices_base] = vertex_base + i+1;
		else
			indices[i * 3 + 1 + indices_base] = vertex_base;

		indices[i * 3 + 2 + indices_base] = vertex_base + i;
		
	}


	spike->geometry.SetIndices(indices);
	obj_ = std::static_pointer_cast<ShapeObj>(spike);

}

GeometryT<Vertex3, Face3, AllocationPolicyNew>* Spike::geometry() {
	std::shared_ptr<ShapeObjSpike> t = std::static_pointer_cast<ShapeObjSpike>(obj_) ;
	return &(t->geometry);
}





/**
 * Create a flat geometry with no indices or faces
 */

VertexSoup::VertexSoup(IndicesType num_verts) {
	std::shared_ptr<ShapeObjVertexSoup> soup;
	soup = std::make_shared<ShapeObjVertexSoup>(num_verts);
	obj_ = std::static_pointer_cast<ShapeObj>(soup);
}


GeometryT<Vertex3, Face3, AllocationPolicyNew>* VertexSoup::geometry() {
	std::shared_ptr<ShapeObjVertexSoup> t = std::static_pointer_cast<ShapeObjVertexSoup>(obj_) ;
	return &(t->geometry);
}



/**
 * Shared Triangle Mesh Indices
 */

SharedTriMesh::SharedTriMesh(VertexSoup &shared, IndicesType num_indices){

	std::shared_ptr<ShapeObjSharedTriMesh> soup;
	soup = std::make_shared<ShapeObjSharedTriMesh>(shared.obj(), num_indices);
	obj_ = std::static_pointer_cast<ShapeObj>(soup);

}
