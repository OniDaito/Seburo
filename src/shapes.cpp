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


Quad::Quad (float w, float h)  {

	geometry_ = std::make_shared< GeometryT<Vertex4, Face4, AllocationPolicyNew> >(6,0,TRIANGLES);

	float w2 = w/2;
	float h2 = h/2;

	Vertex4 aa ( glm::vec4(-w2,	-h2, 0.0f, 1.0f), glm::vec4(.0f, .0f, 1.0f, 1.0f), glm::vec4(1.0f), glm::vec2(0.0f,1.0f));
	Vertex4 ab ( glm::vec4(w2, 	-h2, 0.0f, 1.0f), glm::vec4(.0f, .0f, 1.0f, 1.0f), glm::vec4(1.0f), glm::vec2(1.0f,1.0f));
	Vertex4 ac ( glm::vec4(w2, 	 h2, 0.0f, 1.0f), glm::vec4(.0f, .0f, 1.0f, 1.0f), glm::vec4(1.0f), glm::vec2(1.0f,0.0f));
	Vertex4 ad ( glm::vec4(-w2,  h2, 0.0f, 1.0f), glm::vec4(.0f, .0f, 1.0f, 1.0f), glm::vec4(1.0f), glm::vec2(0.0f,0.0f));

	(*geometry_)[0] = aa;
	(*geometry_)[1] = ab;
	(*geometry_)[2] = ac;
	(*geometry_)[3] = aa;
	(*geometry_)[4] = ac;
	(*geometry_)[5] = ad;

	// No indices here - no real need

}



/*
shared_ptr< GeometryT<Vertex4, Face4, AllocationPolicyNew> > Quad::geometry() {
	std::shared_ptr<ShapeObjQuad> t = std::static_pointer_cast<ShapeObjQuad>(obj_) ;
	return std::make_shared<  GeometryT<Vertex4, Face4, AllocationPolicyNew> > (t);
}*/


/**
 * Build a Cuboid with w,h,d centred at the origin
 */

Cuboid::Cuboid (float w, float h, float d)   {

	geometry_ = std::make_shared< GeometryT<Vertex4, Face4, AllocationPolicyNew> >(8,36,TRIANGLES);


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

	(*geometry_)[0] = aa;
	(*geometry_)[1] = ab;
	(*geometry_)[2] = ac;
	(*geometry_)[3] = ad;

	(*geometry_)[4] = ba;
	(*geometry_)[5] = bb;
	(*geometry_)[6] = bc;
	(*geometry_)[7] = bd;

	IndicesType indices[36] = {0,3,1,0,2,3,0,1,5,0,5,4,1,3,7,1,7,5,6,7,5,6,4,5,4,2,6,4,0,2,2,7,3,2,6,7};

	geometry_->SetIndices(indices);
}



/**
 * Build a Sphere with triangles, give radius and segements
 */

Sphere::Sphere (float_t radius, size_t segments)  {

	size_t vidx = 0;
	size_t num_vertices = segments / 2 * ((segments+2) * 2);
	size_t num_indices = (num_vertices - 3) * 3;

	IndicesType indices[num_indices];


	geometry_ =  std::make_shared< GeometryT<Vertex4, Face4, AllocationPolicyNew> >(num_vertices, num_indices, TRIANGLES );

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

    	(*geometry_)[vidx] = aa;
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

    	(*geometry_)[vidx] = bb;
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
	
	geometry_->SetIndices(indices);

}


/**
 * Build a basic TriMesh with a set number of verts and indices
 */

TriMesh::TriMesh(IndicesType num_verts, size_t num_indices)  {
	geometry_ = std::make_shared<GeometryT<Vertex3, Face3, AllocationPolicyNew> >(num_verts, num_indices, TRIANGLES);
}


/**
 * Build a basic TriMeshSkinned with a set number of verts and indices
 */

TriMeshSkinned::TriMeshSkinned(IndicesType num_verts, size_t num_indices) {
	geometry_ = std::make_shared<GeometryT<Vertex3Skin, Face3, AllocationPolicyNew> >(num_verts, num_indices, TRIANGLES);
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

	geometry_ = std::make_shared<GeometryT<Vertex3, Face3, AllocationPolicyNew> >(num_verts, num_indices, TRIANGLES);


	float top = height/2.0f;
	float bottom = -top;

	IndicesType indices[num_indices];

	// Top cap
	size_t idx = 0;
	Vertex3 top_center ( glm::vec3( 0, top, 0));
	(*geometry_)[idx] = top_center;
	idx++;

	for (size_t i = 0; i < segments; ++i){
		float step = static_cast<float>( DegToRad( (360.0f / segments) * i));
		Vertex3 aa ( glm::vec3( sin(step) * radius, top, cos(step) * radius ));
		(*geometry_)[idx + i] = aa;
	}

	idx += segments;

	for (size_t j = 1; j < stacks; ++j){
		for (size_t i = 0; i < segments; ++i){
			float step = static_cast<float>( DegToRad( (360.0f / segments) * i));
			Vertex3 aa ( glm::vec3( sin(step) * radius, height / stacks * j,  cos(step) * radius));
			(*geometry_)[idx + i] = aa;
		}
		idx += segments;
	}

	// Bottom Cap
	
	for (size_t i = 0; i < segments; ++i){
		float step = static_cast<float>( DegToRad( (360.0f / segments) * i));
		Vertex3 aa ( glm::vec3( sin(step) * radius, bottom,  cos(step) * radius));
		(*geometry_)[idx + i] = aa;
	}

	idx += segments;

	Vertex3 bottom_center ( glm::vec3( 0, bottom, 0));
	(*geometry_)[idx] = bottom_center;
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


	geometry_->SetIndices(indices);

}


/**
 * Builds a spike with a rounded bottom (or any pyramid basically)
 */


Spike::Spike (size_t segments, size_t stacks, float radius, float height){

	// Stacks must always be 1 or more
	if (stacks == 0 ) stacks = 1;

  IndicesType num_verts = (segments + 2) + ((stacks -1) * segments);
  size_t num_indices = ((segments * 2) + ((stacks-1) * segments * 2) ) * 3;


	geometry_ = std::make_shared<GeometryT<Vertex3, Face3, AllocationPolicyNew> >(num_verts, num_indices, TRIANGLES);

	float top = height/2.0f;
	float bottom = -top;

	IndicesType indices[num_indices];

	// Bottom cap
	size_t idx = 0;
	Vertex3 bottom_center ( glm::vec3( 0, top, 0));
	(*geometry_)[idx] = bottom_center;
	idx++;

	for (size_t i = 0; i < segments; ++i){
		float step = static_cast<float>( DegToRad( (360.0f / segments) * i));
		Vertex3 aa ( glm::vec3( sin(step) * radius, bottom, cos(step) * radius ));
		(*geometry_)[idx + i] = aa;
	}

	idx += segments;

	for (size_t j = 1; j < stacks; ++j){

		float th = height / stacks * j;
		float tr = radius / stacks * j;


		for (size_t i = 0; i < segments; ++i){
			float step = static_cast<float>( DegToRad( (360.0f / segments) * i));
			Vertex3 aa ( glm::vec3( sin(step) * tr, th,  cos(step) * tr));
			(*geometry_)[idx + i] = aa;
		}
		idx += segments;
	}

	// Top point

	Vertex3 top_centre ( glm::vec3( 0, top, 0));
	(*geometry_)[idx] = top_centre;
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

	geometry_->SetIndices(indices);

}




/**
 * Create a flat geometry with no indices or faces
 */

VertexSoup::VertexSoup(IndicesType num_verts) {
	geometry_ = std::make_shared< GeometryT<Vertex3, Face3, AllocationPolicyNew> >(num_verts,0,NONE);
}




/**
 * Shared Triangle Mesh Indices
 */

 /*

SharedTriMesh::SharedTriMesh(VertexSoup &shared, IndicesType num_indices) : shared_(shared){

	geometry_ = std::make_shared< GeometryT<Vertex3, Face3, AllocationPolicyShared> >(shared_.geometry()->GetSharedVertices(), num_indices, TRIANGLES);

}*/
