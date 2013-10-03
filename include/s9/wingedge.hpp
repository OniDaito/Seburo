/**
* @brief Winged Edge Classes
* @file wingedge.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/07/2012
*
*/


#ifndef WINGEDGE_HPP
#define WINGEDGE_HPP

#include "common.hpp"
#include "geometry.hpp"

/*
 * Given a primtive with indices, create a winged edge structure for it
 */
 
namespace s9 {
	class WE_Vertex;
	class WE_Face;

	class SEBUROAPI WE_Edge {
	public:
		std::shared_ptr<WE_Vertex> v0, v1;
		std::shared_ptr<WE_Face> face;
		std::shared_ptr<WE_Edge> prev, next, sym;
	};
	
	typedef std::shared_ptr<WE_Edge> WEP_Edge;

	class SEBUROAPI WE_Vertex {
	public:
		std::vector< std::shared_ptr<WE_Edge> > edges;
		size_t idc; // Index into original data
	};

	class SEBUROAPI WE_Face {
	public:
		std::shared_ptr<WE_Edge> edge;
	};
	
	typedef std::shared_ptr<WE_Face> WEP_Face;


	class SEBUROAPI WingedEdge {
	public:
		WingedEdge(){};
		WingedEdge(DrawableGeometry geom);
		std::vector<WEP_Face> getFaces() {return mObj->mWE; };
		virtual operator int() const { return mObj.use_count() > 0; };
	
	protected:
		class SharedObj {
		public:
			std::vector<WEP_Face> mWE;
			DrawableGeometry mGeom;
		};
		
		std::shared_ptr<SharedObj> mObj;
	};


	/*
	 * Type safe version of the WingedEdge class for returning geometry flattened
	 */

	template <class T>
	class WingedEdgeT : public WingedEdge {
			Geometry<T> flatten(); 
	};
	
}

#endif
