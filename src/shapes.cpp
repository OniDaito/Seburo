/**
* @brief Shapes Classes
* @file shapes.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 06/07/2012
*
*/

#include "s9/shapes.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign; 

using namespace s9;

Quad::Quad(float w, float h) {
	
	_obj.reset(new SharedObj);

	vector<uint32_t> indices;
	vector<float> verts;
	vector<float> texcoords;
	vector<float> colours;
	vector<float> normals;
	
	// Quad for Camera drawing
	indices += 0,3,1,3,2,1;
	verts += 0.0f,0.0f,0.0f,
		w, 0.0f,0.0f, 
		w, h,0.0f,
		0.0f, h,0.0f;
		
	normals += 0.0f,0.0f,1.0f,
		0.0f, 0.0f,1.0f, 
		0.0f, 0.0f,1.0f,
		0.0f, 0.0f,1.0f;
	
	texcoords += 0.0, h,
		w, h,
		w, 0.0,
		0.0,0.0;
	
	colours += 1.0f,0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,1.0f,
		0.0f,1.0f,0.0f,1.0f,
		0.0f,0.0f,0.0f,1.0f;
	
	_obj->_geom = GeometryFullFloat(verts,normals,texcoords,colours);
	
	_obj->_geom.addIndices(indices);
}

///\todo
void Quad::resize(float w, float h){

}



Triangle::Triangle(float w, float h){
	
	_obj.reset(new SharedObj);
	
	vector<float> verts;
	vector<float> texcoords;
	vector<float> colours;
	vector<float> normals;
	
	// Quad for Camera drawing
	verts += 0.0f,0.0f,0.0f,
		w, 0.0f,0.0f, 
		w/2, h,0.0f;
		
	normals += 0.0f,0.0f,1.0f,
		0.0f, 0.0f,1.0f, 
		0.0f, 0.0f,1.0f;

	texcoords += 0.0, h,
		w, 0.0,
		w/2, h;

	colours += 1.0f,0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,1.0f,
		0.0f,1.0f,0.0f,1.0f;
		
	_obj->_geom = GeometryFullFloat(verts,normals,texcoords,colours);
	
}

///\todo
void Triangle::resize(float w, float h) {

}
