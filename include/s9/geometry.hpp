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
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		POINTS
	}GeometryPrimitive;


	/**
	 * AllocationPolicyNew - default allocation policy using the heap and new
	 * A Policy for the allocation of vertices & indices to vertices
	 * We can have other policies should we need to in order to streamline allocation later
	 * Modern C++ Design: Applied Generic and Design Patterns (C++ in Depth)
	 * For some reason, we can't return types - is that a template function thing?
   *
   * \todo override [] operators
	 */

	class AllocationPolicyNew {
	protected:
		template<typename VertexType>
   	bool allocateVertices(std::unique_ptr<VertexType[]> &vp, IndicesType size) const {
		  vp = std::unique_ptr<VertexType[]>(new VertexType[size]);
      return true;
  	}

  	bool allocateIndices(std::unique_ptr<IndicesType[]> &ip, IndicesType size) const {
  		ip = std::unique_ptr<IndicesType[]>(new IndicesType[size]); 
      for (IndicesType i =0; i < size; ++i)
        ip[i] = 0;
      return true;
  	}

  	template<typename FaceType>
  	bool allocateFaces(std::unique_ptr<FaceType[]> &fp, IndicesType size_faces, GeometryPrimitive type ) const {
  		
  		switch(type){
  			case TRIANGLES: {
  				assert(size_faces % 3 == 0);
  				fp = std::unique_ptr<FaceType[]>(new FaceType[size_faces / 3]);
  				return true;
        }
        break;
  			
        case QUADS: {
  				assert(size_faces % 4 == 0);
  				fp = std::unique_ptr<FaceType[]>(new FaceType[size_faces / 4]);
  				return true;
        }
  			break;

  			case TRIANGLE_STRIP: {
  				assert(size_faces > 2);
  				fp = std::unique_ptr<FaceType[]>(new FaceType[1]);
  				return true;
        }
  			break;

  			case TRIANGLE_FAN: {
  				assert(size_faces > 2);
  				fp = std::unique_ptr<FaceType[]>(new FaceType[1]);
  				return true;
        }
  			break;

  			case POINTS: {
  				fp = nullptr;
  				return false;
        }
  			break;

  			default: {
  				assert(false && "SEBURO ASSERT - Must provide a type for geometry.");
          return false;
        }
        break;
  		};

      return false;

  	}

	};


	/**
	 * GeometryT is the template for the basic class that forms geometry.
	 * Geometry is a static collection of vertices and indices in memory.
	 * It had a policy class to allocate its various 
	 */

	template< typename VertexType, typename FaceType, typename AllocationPolicy> 
	class SEBUROAPI GeometryT : private AllocationPolicy {
	public:
		GeometryT() { size_indices_= size_vertices_ = size_faces_ = 0; 
      vertices_ = nullptr; 
			indices_ = nullptr;
			faces_= nullptr;
			indexed_ = false; 

    };

		GeometryT(IndicesType num_verts, IndicesType num_indices, GeometryPrimitive prim_type ) {
      indexed_ = false;
			allocate(num_verts, num_indices, prim_type );
			prim_type_ = prim_type;
		};


    /**
     * operator[] will return the vertex at the position given, ignoring indices
     */

    VertexType& operator[](IndicesType idx) {
      assert(idx >= 0 && idx < size_vertices_);
      return vertices_[indices_[idx]];
  
    }
    
    const VertexType& operator[](IndicesType idx) const {
      assert(idx >= 0 && idx < size_vertices_);
      return vertices_[indices_[idx]];
    }

    void setVertex(IndicesType idx, VertexType v) {
      assert(idx >= 0 && idx < size_vertices_);
      vertices_[idx] = v;
    }

    void setIndex(IndicesType idx, IndicesType v) {
      assert(idx >= 0 && idx < size_indices_);
      indices_[idx] = v;
    }

    ///\todo one day, lets not use basic pointers? - How does using pointers affect allocation policy?

    const std::unique_ptr<IndicesType[]>&  indices() { return indices_; }
    const std::unique_ptr<VertexType[]>&  vertices() { return vertices_; }
    const std::unique_ptr<FaceType[]>&    faces() { return faces_; }


    bool indexed() {return indexed_; }

	
	protected:

		using AllocationPolicy::allocateVertices;
		using AllocationPolicy::allocateIndices;
		using AllocationPolicy::allocateFaces;

		void allocate(IndicesType num_verts, IndicesType num_indices, GeometryPrimitive prim_type) {
			size_indices_= num_indices;
			size_vertices_ = num_verts;

			if (num_verts > 0){
				allocateVertices(vertices_, num_verts);
			}

			if (num_indices > 0) {
				allocateIndices(indices_, num_indices);
				size_faces_ = num_indices;
			} else {
				size_faces_ = num_verts;
			}

			allocateFaces(faces_, size_faces_, prim_type);

		}

		// Plain-old-data - no need to be shared_obj
		// Allocated using the allocation policy
    // Defaults are set here too!
    // We use unique_ptr as we are using copy-constructor semantics in our shape classes
    // so we end up transferring ownership of these pointers from a temporary object to
    // another object, which means destruction occurs twice.

    // http://stackoverflow.com/questions/13061979/shared-ptr-to-an-array-should-it-be-used
    // Shared Pointers to arrays are odd


		IndicesType 		size_indices_   = 0;
		IndicesType			size_vertices_  = 0;
		IndicesType			size_faces_     = 0;
		std::unique_ptr<VertexType[]>	  vertices_       = nullptr;
		std::unique_ptr<IndicesType[]>	indices_        = nullptr;

		bool indexed_ = false;

		// Per face stuff	
		GeometryPrimitive	prim_type_  = TRIANGLES;
		std::unique_ptr<FaceType[]> faces_              = nullptr;

	};

}


#endif
