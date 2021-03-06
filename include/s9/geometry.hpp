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
	

	/**
	 * Typedef hint for Geometry
	 * Ultimately, we have per face data but quads/triangle/fans are implict structures from indices
	 */

	typedef enum {
		TRIANGLES,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		POINTS,
    LINES,
    LINE_LOOP,
    NONE
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
   	bool AllocateVertices(std::shared_ptr< std::vector<VertexType> >  &vp, IndicesType size) const {
		  vp =std::shared_ptr<std::vector<VertexType> > (new std::vector<VertexType>(size));
      return true;
  	}

  	bool AllocateIndices(std::unique_ptr<IndicesType[]> &ip, IndicesType size) const {
  		ip = std::unique_ptr<IndicesType[]>(new IndicesType[size]); 
      for (IndicesType i =0; i < size; ++i)
        ip[i] = 0;
      return true;
  	}

  	template<typename FaceType>
  	bool AllocateFaces(std::unique_ptr<FaceType[]> &fp, IndicesType size_faces, GeometryPrimitive type ) const {
  		
  		switch(type){
        case NONE:
        case POINTS:
          assert(size_faces == 0);
          fp = nullptr;
          return true;
        break;

        case LINE_LOOP:
  			case TRIANGLES: 
  				assert(size_faces % 3 == 0);
  				fp = std::unique_ptr<FaceType[]>(new FaceType[size_faces / 3]);
  				return true;
        
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


  			default: {
  				assert(false && "SEBURO ASSERT - Must provide a type for geometry indexing.");
          return false;
        }
        break;
  		};

      return false;
  	}

	};

  /**
   * The Shared Allocation Policy allows the vertices to be shared between geometries with their
   * own unique indices. We simply pass in the shared pointer from another geometry.
   */

  class AllocationPolicyShared : public AllocationPolicyNew {
  protected:
    template<typename VertexType>
    bool AllocateVertices(std::shared_ptr< std::vector<VertexType> >  &vp, std::shared_ptr< std::vector<VertexType> >  &np ) const {
      vp = np;
      return true;
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
		GeometryT() { 
		size_indices_= size_vertices_ = size_faces_ = 0; 
      		vertices_ = nullptr; 
		indices_ = nullptr;
		faces_= nullptr;
		indexed_ = false; 

    	}

	GeometryT(IndicesType num_verts, IndicesType num_indices, GeometryPrimitive prim_type ) {
      		indexed_ = false;		
      		size_indices_= num_indices;
     		size_vertices_ = num_verts;

      		if (num_verts > 0){
        		AllocateVertices(vertices_, num_verts);
      		}

      		if (num_indices > 0) {
        		AllocateIndices(indices_, num_indices);
        		indexed_ = true;
      		}	 

      		size_faces_ = indexed_ ? size_indices_ : 0;

      		AllocateFaces(faces_, size_faces_, prim_type);

		prim_type_ = prim_type;
	}

    	// Constructor for shared geometries
	
	   // This used to be templated for AllocationPolicyShared but GCC Complained
    	GeometryT( std::shared_ptr< std::vector<VertexType> >  &sp, IndicesType num_indices, GeometryPrimitive prim_type ) {
    
      		indexed_ = false;
      		size_indices_= num_indices;
  
      		AllocateVertices(vertices_, sp);
      
      		AllocateIndices(indices_, num_indices);
      		indexed_ = true;
     
      		AllocateFaces(faces_, num_indices, prim_type);

      		prim_type_ = prim_type;
    	}

    /**
     * operator[] will return the vertex at the position given, ignoring indices
     */

    VertexType& operator[](IndicesType idx) {
      assert(idx >= 0 && idx < size_vertices_);
      return (*vertices_)[idx];
  
    }
    
    const VertexType& operator[](IndicesType idx) const {
      assert(idx >= 0 && idx < size_vertices_);
      return vertices_[idx];
    }

    void SetVertex(IndicesType idx, VertexType v) {
      assert(idx >= 0 && idx < size_vertices_);
      vertices_[idx] = v;
    }

    void SetIndex(IndicesType idx, IndicesType v) {
      assert(idx >= 0 && idx < size_indices_);
      indices_[idx] = v;
    }

    const void SetIndices( IndicesType *a) const {
      for (int i =0; i < size_indices_; ++i){
        indices_[i] = a[i];
      }
    }

    ///\todo one day, lets not use basic pointers? - How does using pointers affect allocation policy?

    const std::unique_ptr<IndicesType[]>&  indices() const { return indices_; }
    std::vector<VertexType> &  vertices() const { return *vertices_; } ///\todo potentially cheeky?
    const std::unique_ptr<FaceType[]>&    faces() const { return faces_; }

    /// Return the shared pointer to the vertices so we can share them with other geometry
    std::shared_ptr<std::vector<VertexType> >&  GetSharedVertices() { return vertices_; }

    bool indexed() {return indexed_; }

    const IndicesType size_vertices() const { return size_vertices_; }
    const IndicesType size_indices() const { return size_indices_; }
    const IndicesType size_faces() const { return size_faces_; }
    const GeometryPrimitive prim_type() const { return prim_type_; }
	
	protected:

		using AllocationPolicy::AllocateVertices;
		using AllocationPolicy::AllocateIndices;
		using AllocationPolicy::AllocateFaces;





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
		std::shared_ptr<std::vector<VertexType> >	  vertices_  = nullptr;
		std::unique_ptr<IndicesType[]>	indices_        = nullptr;

		bool indexed_ = false;

		// Per face stuff	
		GeometryPrimitive	prim_type_  = TRIANGLES;
		std::unique_ptr<FaceType[]> faces_              = nullptr;

	};


 

}


#endif
