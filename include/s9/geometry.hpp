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

#include "vertex_types.hpp"

namespace s9 {
	
	/*
	 * Base Geometry. Used by the primitive
	 * \todo rather than use a dirty flag, register a listener or similar. Make implicit!
	 */
	 
	class DrawableGeometry {
	public:
		virtual bool isDirty(){ return false;}
		virtual void setDirty(bool b) {}
		virtual void* addr() {return NULL; }
		virtual bool isIndexed(){return false; }
		virtual uint32_t* indexaddr(){return NULL; }
		virtual uint32_t size() {return 0;}
		virtual uint32_t indexsize() {return 0;}
	};
	
	

	/*
	 * Basic Geometry Class - Passed up to the primitive. Created in various ways
	 * This is essentially a collection of types of vertices with addition and deletion
	 * taken care of. Likely, this will be expanded to take into account PCL Meshes 
	 * and similar and will become subclassed or specific
	 * 
	 * This class uses a single, flat array of types, interleaved
	 */
	
	template <class T>
	class Geometry : public DrawableGeometry{
	public:
		Geometry() {};
		
		Geometry(std::vector<glm::vec3> v, std::vector<glm::vec3> n) {};
		Geometry (std::vector<float> v, std::vector<float> n) {};
		Geometry(std::vector<float> v, std::vector<float> n, std::vector<float> t, std::vector<float> c) {};

	protected:
	
		struct SharedObj {
			std::vector<T> _buffer;
			std::vector<uint32_t> _indices;
			bool _dirty,_resized;
			uint32_t _range;
		};
		
		boost::shared_ptr<SharedObj> _obj;
	
	public:
		
		Geometry(std::vector<T> v) {
			_obj.reset(new SharedObj());
			_obj->_buffer = v;
			_obj->_range = v.size();
			setResized(true);
		};

		void createEmpty() {_obj.reset(new SharedObj()); };

		template<class U>
		U convert() {};
 
		std::vector<T> getBuffer() {return _obj->_buffer; };
	
		virtual operator int() const { return _obj.use_count() > 0; };
	
		/// Return the address of the data
		void* addr() { return &(_obj->_buffer[0]); };

		/// Return the address of the index
		uint32_t* indexaddr() { return &(_obj->_indices[0]); };
		
		/// Return the size of the data buffer
		uint32_t size() { return _obj->_buffer.size(); };
		uint32_t indexsize() { return _obj->_indices.size(); };
		int elementsize() { return sizeof(T); };
		
		/// Get the number of elements to be drawn
		uint32_t numElements() {return _obj->_range; };

		/// Set the number of elements to be drawn. Allows buffering
		void setNumElements(uint32_t r) { _obj->_range = r; };

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
		void addVertex(T v) {_obj->_buffer.push_back(v); setResized(true); };
		
		void setVertex(T v, uint32_t p) { _obj->_buffer[p] = v; setDirty(true); };
		void delVertex(uint32_t p) { _obj->_buffer.erase( _obj->_buffer.begin() + p); setResized(true); };
		void addIndices(std::vector<uint32_t> idx) { _obj->_indices = idx; setNumElements(idx.size()); setResized(true); };
		T getVertex(uint32_t i) { return _obj->_buffer[i]; };
	};




	// Specialist contructors for speed - basically, zipping from different vectors
	
	template<>
	inline Geometry<VertPNG>::Geometry(std::vector<glm::vec3> v, std::vector<glm::vec3> n) {
		_obj.reset(new SharedObj());
		
		if (v.size() != n.size()) { std::cerr << "Seburo - Counts do not match" << std::endl; throw; return; }
		
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
		
		if (v.size() != n.size()) { std::cerr << "Seburo - Counts do not match" << std::endl; throw; return; }
		
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

	// Handy Typedefs
	
	typedef Geometry<VertPNCTF> GeometryFullFloat;
	typedef Geometry<VertPNCTG> GeometryFullGLM;
	typedef Geometry<VertPNF> GeometryPNF;
	typedef Geometry<VertPF> GeometryPF;
	typedef Geometry<VertPNT8F> GeometryPNT8F;

}


#endif
