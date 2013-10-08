/**
* @brief Geometry Classes
* @file geom.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 16/07/2012
*
*/


#ifndef S9_GEOMETRY_HPP
#define S9_GEOMETRY_HPP

#include "common.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "primitives.hpp"

namespace s9 {

	/*
	 * Basic Geometry Class - Passed up to the node. Created in various ways
	 * This is essentially a collection of types of vertices with addition and deletion
	 * taken care of. 
	 *
	 * Geometry is combined with a node to be drawn. Geometry on its own is converted to
	 * a series of OpenGL buffers as needed.
	 * 
	 * For speed we pass in references to our data, held via our object
	 *
	 * \todo potential here for a 64bit set of indices at some point
	 */
	
	template <class T>
	class SEBUROAPI GeometryT {
	public:
		GeometryT() : {};
		
		GeometryT (std::vector<T> &vertices);
		GeometryT (std::vector<T> &vertices, std::vector<uint32_t> &indices );

	protected:
	
		struct SharedObj {
			std::vector<T>	 		&_vertices;
			std::vector<uint32_t> 	&_indices;
			bool _dirty,_resized;
		};
		
		std::shared_ptr<SharedObj> _obj;
	
	public:
		

		std::vector<T> getBuffer() {return _obj->_vertices; };
	
		virtual operator int() const { return _obj.use_count() > 0; };
	
		/// Return the address of the data
		void* addr() { return &(_obj->_vertices[0]); };

		/// Return the address of the index
		uint32_t* addrIndices() { return &(_obj->_indices[0]); };
		
		/// Return the size of the data buffer
		uint32_t size() { return _obj->_vertices.size(); };
		uint32_t indexSize() { return _obj->_indices.size(); };
		int elementSize() { return sizeof(T); };
		
		/// Is this geometry indexed
		bool isIndexed() {return _obj->_indices.size() > 0; };
	
		/// Have any values been changed
		bool isDirty() {return _obj->_dirty;};

		/// Set the dirty/values changed flag
		void setDirty(bool b) {_obj->_dirty = b; };

		/// Has this geometry been resized
		bool isResized() {return _obj->_resized;};

		/// Set the geometry resize flag
		void setResized(bool b){ _obj->_resized = b; };

	
		std::vector<uint32_t>  getIndices() {return _obj->_indices; };

		void addVertex(T v) {_obj->_vertices.push_back(v); setResized(true); };
		void setVertex(T v, uint32_t p) { _obj->_vertices[p] = v; setDirty(true); };
		void delVertex(uint32_t p) { _obj->_vertices.erase( _obj->_vertices.begin() + p); setResized(true); };
		void addIndices(std::vector<uint32_t> idx) { _obj->_indices = idx; setNumElements(idx.size()); setResized(true); };
		T getVertex(uint32_t i) { return _obj->_vertices[i]; };
	};



	// Handy Typedefs
	
	typedef GeometryT<Vertex2> Geometry2;
	typedef GeometryT<Vertex3> Geometry3;
	typedef GeometryT<Vertex4> Geometry4;

}


#endif
