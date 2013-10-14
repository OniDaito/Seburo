/**
* @brief Geometry Classes - Basic placeholders for geometry
* @file geometry.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/10/2013
*
*/


#ifndef S9_GEOMETRY_HPP
#define S9_GEOMETRY_HPP

#include "common.hpp"
#include "primitives.hpp"

namespace s9 {
	

	typedef uint32_t IndicesType; // Expand potentially on 64 bit systems?

	/**
	 * Typedef hint for Geometry
	 * Ultimately, we have per face data but quads/triangle/fans are implict structures from indices
	 */
	

	typedef enum {
		TRIANGLES,
		QUADS,
		TRISTRIP,
		TRIFAN,
		POINTS
	}GeometryPrimitive;


	/**
	 * AllocationPolicyNew - default allocation policy using the heap and new
	 * A Policy for the allocation of vertices & indices to vertices
	 * We can have other policies should we need to in order to streamline allocation later
	 * Modern C++ Design: Applied Generic and Design Patterns (C++ in Depth)
	 * For some reason, we can't return types - is that a template function thing?
	 */

	class AllocationPolicyNew {
	protected:
		template<typename VertexType>
   		void allocateVertices(VertexType* vp, IndicesType const size) const {
			vp = static_cast<VertexType*>(new VertexType[size]);    		
    	}

    	void allocateIndices(IndicesType* ip, IndicesType const size) const {
    		ip = static_cast<IndicesType*>(new IndicesType[size]);  
    	}

    	template<typename FaceType>
    	void allocateFaces(FaceType *fp, IndicesType const size_faces_, GeometryPrimitive const type ) const {
    		
    		switch(type){
    			case TRIANGLES:
    				assert(size_faces_ % 3 == 0);
    				fp = static_cast<FaceType*>(new FaceType[size_faces_ / 3]);
    				return;
    			case QUADS:
    				assert(size_faces_ % 4 == 0);
    				fp = static_cast<FaceType*>(new FaceType[size_faces_ / 4]);
    				return;
    			break;

    			case TRISTRIP:
    				assert(size_faces_ > 2);
    				fp = static_cast<FaceType*>(new FaceType[1]);
    				return;
    			break;

    			case TRIFAN:
    				assert(size_faces_ > 2);
    				fp = static_cast<FaceType*>(new FaceType[1]);
    				return;
    			break;

    			case POINTS:
    				fp = nullptr;
    				return;
    			break;

    			default:
    				assert(false && "SEBURO ASSERT - Must provide a type for geometry.");
    			break;
    		};

    	}


    	template<typename VertexType>
    	void deleteVertices(VertexType* const verts) const {
    		delete verts;  
    	}

    	void deleteIndices(IndicesType* const indices) const {
    		delete indices;
    	}

   		template<typename FaceType>
    	void deleteFaces(FaceType* const faces) const {
    		delete faces;
    	}

    	virtual ~AllocationPolicyNew() {};

	};


	/**
	 * GeometryT is the template for the basic class that forms geometry.
	 * Geometry is a static collection of vertices and indices in memory.
	 * It had a policy class to allocate its various 
	 */
	template< typename VertexType, typename FaceType, typename AllocationPolicy> 
	class SEBUROAPI GeometryT : public AllocationPolicy {
	public:
		GeometryT() { size_ = size_vertices_ = size_faces_ = 0; 
			vertices_ = nullptr; 
			indices_ = nullptr;
			faces_= nullptr;
			alloc_vertices_ = alloc_indices_ = alloc_faces_ = indexed_ = false; };

		GeometryT(IndicesType num_verts, IndicesType num_indices, GeometryPrimitive prim_type ) {
			alloc_vertices_ = alloc_indices_ = alloc_faces_ = indexed_ = false;
			allocate(num_verts, num_indices, prim_type );
			prim_type_ = prim_type;
		};

		using AllocationPolicy::deleteVertices;
		using AllocationPolicy::deleteIndices;
		using AllocationPolicy::deleteFaces;

		~GeometryT() {
			if (alloc_indices_)
				deleteIndices(indices_);
			if (alloc_vertices_)
				deleteVertices(vertices_);
			if (alloc_faces_)
				deleteFaces(faces_);
		}

	
	protected:

		using AllocationPolicy::allocateVertices;
		using AllocationPolicy::allocateIndices;
		using AllocationPolicy::allocateFaces;

	
		void allocate(IndicesType num_verts, IndicesType num_indices, GeometryPrimitive prim_type) {
			size_ = num_indices;
			size_vertices_ = num_verts;

			if (num_verts > 0){
				allocateVertices(vertices_, num_verts);
				alloc_vertices_ = true;
			}

			if (num_indices > 0) {
				allocateIndices(indices_, num_indices);
				alloc_indices_ = true;
			}
			
			if (num_indices > 0){
				size_faces_ = num_indices;
				indexed_ = true;
			}
			else {
				size_faces_ = num_verts;
			}

			allocateFaces(faces_, size_faces_, prim_type);
			alloc_faces_ = true;


		}

		


		// Plain-old-data - no need to be shared_obj
		// Allocated using the allocation policy

		IndicesType 		size_;
		IndicesType			size_vertices_;
		IndicesType			size_faces_;
		VertexType*			vertices_;
		IndicesType*		indices_;

		bool	alloc_vertices_;
		bool	alloc_faces_;
		bool	alloc_indices_;

		bool	indexed_;

		// Per face stuff	
		GeometryPrimitive	prim_type_;
		FaceType*			faces_;

	};





}


#endif
