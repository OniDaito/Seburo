/**
* @brief Geometry Class
* @file geometry.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 08/10/2013
*
*/

#include "s9/geometry.hpp"

using namespace std;
using namespace s9;

/**
 * GeometryT constructor with no indices passed (assume not indexed therefore)
 */

template <class T>
GeometryT::GeometryT (std::vector<T> &vertices) : _obj( std::shared_ptr<SharedObj> (new SharedObj())) {
	_obj->vertices = vertices;
	_obj->_dirty = true;
	_obj->_resized = true;
}

/**
 * GeometryT constructor with indices passed in
 */

template <class T>
GeometryT::GeometryT (std::vector<T> vertices, std::vector<uint32_t> indices ) {
	_obj->vertices = vertices;
	_obj->indices = indices;
	_obj->_dirty = true;
	_obj->_resized = true;
}



	// Specialist contructors for speed - basically, zipping from different vectors
	
	template<>
	 inline Geometry<VertPNG>::Geometry(std::vector<glm::vec3> v, std::vector<glm::vec3> n) {
		_obj.reset(new SharedObj());
		
		if (v.size() != n.size()) { std::cerr << "SEBURO - Counts do not match" << std::endl; throw; return; }
		
		for (uint32_t i=0; i < v.size(); ++i){
			VertPNG png = {v[i],n[i]};
			_obj->_buffer.push_back( png );
		}
		setNumElements(v.size());
		setResized(true);
	}
	
	template<>
	inline Geometry<VertPNF>::Geometry (std::vector<float> v, std::vector<float> n) {
		_obj.reset(new SharedObj());
		
		if (v.size() != n.size()) { std::cerr << "SEBURO - Counts do not match" << std::endl; throw; return; }
		
		for (uint32_t i=0; i < v.size(); i+=3){
			Float3 a = {v[i],v[i+1],v[i+2]}; 
			Float3 b = {n[i],n[i+1],n[i+2]};
			VertPNF p = {a,b}; 
			_obj->_buffer.push_back(p);	
		}
		setNumElements(v.size());
		setResized(true);
	}
	

	template<>
	inline Geometry<VertPNCTF>::Geometry(std::vector<float> v, std::vector<float> n, std::vector<float> t, std::vector<float> c) {
		_obj.reset(new SharedObj());
		///\todo add size checking heres
		
		for (uint32_t i=0; i < v.size() / 3; i++){
			Float3 v0 = {v[i*3],v[i*3+1],v[i*3+2]};
			Float3 v1 = {n[i*3],n[i*3+1],n[i*3+2]};
			Float4 v2 = {c[i*4],c[i*4+1],c[i*4+2],c[i*4+3]};
			Float2 v3 = {t[i*2],t[i*2+1]};
			
			VertPNCTF p = {v0,v1,v2,v3};
			_obj->_buffer.push_back(p);
		}
		setNumElements(v.size());
		setResized(true);
	}

	// Conversion templates - probaby inefficient ><

	/*
	 * Conversion from basic to 8 texture parameters - Quite specific
	 */
	
	template <>
	template <>
	inline Geometry<VertPNT8F> Geometry<VertPNF>::convert() {

		std::vector<VertPNT8F> vtemp;
		std::vector<VertPNF> vstart = this->_obj->_buffer;
		for (std::vector<VertPNF>::iterator it = vstart.begin(); it != vstart.end(); it++) {
			VertPNF p = *it;
			VertPNT8F tp;
			tp.mP = p.mP;
			tp.mN = p.mN;

			for (int i = 0; i < 8; ++i){
				Float2 tt; tt.x = 0.0f; tt.y = 0.0f;
				tp.mT[i] = tt;
			}
			vtemp.push_back(tp);
		}

		Geometry<VertPNT8F> b (vtemp);
		b.addIndices(getIndices());
		b.setResized(true);

		return b;
	}

	/*
	 * Conversion from basic to full
	 */

	template <>
	template <>
	inline Geometry<VertPNCTF> Geometry<VertPNF>::convert() {
		std::vector<VertPNCTF> vtemp;
		std::vector<VertPNF> vstart = this->_obj->_buffer;
		for (std::vector<VertPNF>::iterator it = vstart.begin(); it != vstart.end(); it++) {
			VertPNF p = *it;
			VertPNCTF tp;
			tp.mP = p.mP;
			tp.mN = p.mN;
			tp.mC.x = tp.mC.y = tp.mC.z = 0.5f;
			tp.mC.w = 1.0f;
			tp.mT.x = tp.mT.y = 0.0f;
			vtemp.push_back(tp);
		}

		Geometry<VertPNCTF> b (vtemp);
		b.addIndices(getIndices());
		b.setResized(true);
		return b;
	}
