/**
* @brief Shapes Classes - More complex geometry
* @file shapes.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/10/2013
*
*/

#ifndef S9_SHAPES_HPP
#define S9_SHAPES_HPP

#include "common.hpp"
#include "geometry.hpp"
#include "gl/drawable.hpp" // Default to OpenGL for now
#include "math_utils.hpp"

namespace s9 {
	
  ///\todo still not happy with this structure! ><


	/**
	 * Basic interface to shapes
	 * \todo brew and draw go through a lot of calls - worrying :S
	 */

  template<typename VertexType, typename FaceType, typename AllocationType>
	class SEBUROAPI Shape {

	public:
		Shape () {};
    ///\todo we may be able to move brew/draw somwhere else or inherit?
		void Brew(gl::BrewFlags b=gl::BrewFlagsDefault) { gl_drawable_.Brew(*geometry_, b); }
		void Draw(GeometryPrimitive g = TRIANGLES) { gl_drawable_.Draw(*geometry_, g); };		
    bool brewed() { if(geometry_ != nullptr) return gl_drawable_.brewed(); return false;} //\todo should be IsBrewed()

    const gl::Drawable & gl_drawable() const { return gl_drawable_; }
    
	protected:
	
		std::shared_ptr< GeometryT<VertexType, FaceType, AllocationType> > geometry_;
    gl::Drawable gl_drawable_;

  public:
    
    typedef std::shared_ptr<  GeometryT<VertexType, FaceType, AllocationType> > Shape::*unspecified_bool_type;
    operator unspecified_bool_type() const { return ( geometry_.get() == 0 ) ? 0 : &Shape::geometry_; }
    void reset() { geometry_.reset(); }

	};

  /**
   * Partial specialization of shape that deals with Shared Shape Geometries
   */

  template<typename VertexType, typename FaceType>
  class SEBUROAPI Shape<VertexType, FaceType, AllocationPolicyShared> {

  public:
    Shape (Shape<VertexType,FaceType,AllocationPolicyNew> & shared) : shared_(shared) {};
    void Brew(gl::BrewFlags b=gl::BrewFlagsDefault) { gl_drawable_.Brew(*geometry_, shared_.gl_drawable(), b); }
    void Draw(GeometryPrimitive g = TRIANGLES) { gl_drawable_.Draw(*geometry_, g); };   
    bool brewed() { if(geometry_ != nullptr) return gl_drawable_.brewed(); return false;} //\todo should be IsBrewed()

    const gl::Drawable & gl_drawable() const { return gl_drawable_; }
    
  protected:
  
    std::shared_ptr< GeometryT<VertexType, FaceType, AllocationPolicyShared> > geometry_;
    gl::Drawable gl_drawable_;
    Shape<VertexType,FaceType,AllocationPolicyNew>  & shared_;

  public:
    
    typedef std::shared_ptr<  GeometryT<VertexType, FaceType, AllocationPolicyShared> > Shape::*unspecified_bool_type;
    operator unspecified_bool_type() const { return ( geometry_.get() == 0 ) ? 0 : &Shape::geometry_; }
    void reset() { geometry_.reset(); }

  };



	/**
	 * A basic Quad with no indexing.
	 * Quad provides a basic contructor that does little more than build the ShapeObjectQuad
	 */ 

	class SEBUROAPI Quad : public Shape<Vertex4, Face4, AllocationPolicyNew> {
	public:

		Quad() {};
		Quad(float w, float h);

		std::shared_ptr< GeometryT<Vertex4, Face4, AllocationPolicyNew> > geometry() { return geometry_; };

	};

	/**
	 * A basic Cuboid, made up of Quads with the underlying Vec4 type
	 */ 


	class SEBUROAPI Cuboid : public Shape<Vertex4, Face4, AllocationPolicyNew> {
	public:

		Cuboid() {};
		Cuboid(float w, float h, float d);

		std::shared_ptr< GeometryT<Vertex4, Face4, AllocationPolicyNew> > geometry() { return geometry_; };
	
	};


  /**
   * A basic Sphere, made up of triangles with the underlying Vec4 type
   */ 

  class SEBUROAPI Sphere : public Shape<Vertex4, Face4, AllocationPolicyNew> {
  public:

    Sphere() {};
    Sphere(float_t radius, size_t segments);

    std::shared_ptr< GeometryT<Vertex4, Face4, AllocationPolicyNew> > geometry(){ return geometry_; };
  
  };

	
  /**
   * A mesh made up of triangles
   */

  class SEBUROAPI TriMesh : public Shape<Vertex3, Face3, AllocationPolicyNew> {

  public:

    TriMesh() {};
    TriMesh(IndicesType num_verts, size_t num_indices);

    std::shared_ptr< GeometryT<Vertex3, Face3, AllocationPolicyNew> > geometry(){ return geometry_; };

  };

  /**
   * A Triangle Mesh for skinning - using per vertex data for its weights
   */ 


  /**
   * A mesh made up of triangles and skinned
   */

  class SEBUROAPI TriMeshSkinned : public Shape<Vertex3Skin, Face3, AllocationPolicyNew> {

  public:

    TriMeshSkinned() {};
    TriMeshSkinned(IndicesType num_verts, size_t num_indices);

    std::shared_ptr< GeometryT<Vertex3Skin, Face3, AllocationPolicyNew> > geometry(){ return geometry_; };

  };


  /**
   * A Soup of non-indexed vertices. Basically just a place-holder for flat geometry. No Faces, just a
   * vertex list
   */



  class SEBUROAPI VertexSoup : public Shape<Vertex3, Face3, AllocationPolicyNew> {

  public:

    VertexSoup() {};
    VertexSoup(IndicesType num_verts);

    std::shared_ptr< GeometryT<Vertex3, Face3, AllocationPolicyNew> > geometry(){ return geometry_; };

  };


  /**
   * Shared Triangle Mesh. This is basically a set of indices, grouped as triangles, that can "look" 
   * at another array of vertices on the graphics card to get their data. At present, just VertexSoups are 
   * allowed.
   */

 /*   struct ShapeObjSharedTriMesh : public ShapeObj {

    ShapeObjSharedTriMesh( const ShapeObj &sp, size_t num_indices ) : 
      geometry(sp.geometry.GetSharableVertices(), num_indices, TRIANGLES ), shared(sp) {

    }

    void Brew (gl::BrewFlags b) {
      // Shared Drawable must be brewed before we brew the objects that depend on it. 
      if (!shared.brewed())
        shared.Brew(b);

      gl_drawable.Brew(geometry, shared.gl_drawable, b); 
    }

    void Draw (GeometryPrimitive g = TRIANGLES) { gl_drawable.Draw(geometry, g); }

    GeometryT<Vertex3, Face3, AllocationPolicyShared> geometry;
    ShapeObj &shared;
      
  };*/

  /// So far, the only shape that shares data is a set of Triangles, SharedTriMesh

  class SEBUROAPI SharedTriMesh : public Shape<Vertex3, Face3, AllocationPolicyShared> {

  public:
    SharedTriMesh(VertexSoup &shared, IndicesType num_indices) : Shape(shared) {}

    std::shared_ptr< GeometryT<Vertex3, Face3, AllocationPolicyShared> > geometry(){ return geometry_; };

  };




  /**
   * A basic Cylinder
   */ 



  class SEBUROAPI Cylinder : public Shape<Vertex3, Face3, AllocationPolicyNew> {
  public:

    Cylinder() {};
    Cylinder(size_t segments, size_t stacks, float diameter, float height);

    std::shared_ptr< GeometryT<Vertex3, Face3, AllocationPolicyNew> > geometry(){ return geometry_; };
  
  };

  /**
   * A pointed Cyclinder - spike thing :S
   */ 

 
  class SEBUROAPI Spike : public Shape<Vertex3, Face3, AllocationPolicyNew> {
  public:

    Spike() {};
    Spike(size_t segments, size_t stacks, float radius, float height);

    std::shared_ptr< GeometryT<Vertex3, Face3, AllocationPolicyNew> > geometry(){ return geometry_; };
  
  };

}


#endif

