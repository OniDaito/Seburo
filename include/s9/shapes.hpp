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
	 */

  template<typename VertexType, typename FaceType, typename AllocationType>
	class SEBUROAPI Shape {

	public:
		Shape () {};
    ///\todo we may be able to move brew/draw somwhere else or inherit?
		void Brew(gl::BrewFlags b=gl::BrewFlagsDefault) { obj_->gl_drawable.Brew(obj_->geometry, b); }
		void Draw() { obj_->gl_drawable.Draw(obj_->geometry); };		
    bool brewed() { if(obj_ != nullptr) return obj_->gl_drawable.brewed(); return false;} //\todo should be IsBrewed()

    gl::Drawable & gl_drawable() { return obj_->gl_drawable; }

    GeometryT<VertexType, FaceType, AllocationType>& geometry() { return obj_->geometry; };
    
	protected:
	
    struct SharedObject {
      GeometryT<VertexType, FaceType, AllocationType> geometry;
      gl::Drawable gl_drawable;
    };

    std::shared_ptr<SharedObject> obj_ = nullptr;

	
  public:
    
    typedef std::shared_ptr<  GeometryT<VertexType, FaceType, AllocationType> > Shape::*unspecified_bool_type;
    operator unspecified_bool_type() const { return ( obj_.get() == 0 ) ? 0 : &Shape::obj_; }
    void reset() { obj_.reset(); }

	};

  /**
   * Partial specialization of shape that deals with Shared Shape Geometries
   */

  template<typename VertexType, typename FaceType>
  class SEBUROAPI Shape<VertexType, FaceType, AllocationPolicyShared> {

  public:
    Shape () {};
    Shape (Shape<VertexType,FaceType,AllocationPolicyNew> s) : obj_( std::shared_ptr<SharedObject>(new SharedObject(s))) {};
    void Brew(gl::BrewFlags b=gl::BrewFlagsDefault) { 

   //   if (!shared_.brewed())
   //     shared_.Brew(b);

      obj_->gl_drawable.Brew(obj_->geometry, obj_->shared.gl_drawable(), b); 

    }
    void Draw() { obj_->gl_drawable.Draw( obj_->geometry); };   
    bool brewed() { if(obj_ != nullptr) return obj_->gl_drawable.brewed(); return false;} //\todo should be IsBrewed()

    gl::Drawable & gl_drawable() { return obj_->gl_drawable; }
    GeometryT<VertexType, FaceType, AllocationPolicyShared>& geometry() { return obj_->geometry; };
    
  protected:

    struct SharedObject {

      SharedObject(Shape<VertexType,FaceType,AllocationPolicyNew>  s) : shared(s) {}

      GeometryT<VertexType, FaceType, AllocationPolicyShared> geometry;
      Shape<VertexType,FaceType,AllocationPolicyNew>  shared;
      gl::Drawable gl_drawable;
    };

    std::shared_ptr<SharedObject> obj_ = nullptr;

  public:
    
    typedef std::shared_ptr<  GeometryT<VertexType, FaceType, AllocationPolicyShared> > Shape::*unspecified_bool_type;
    operator unspecified_bool_type() const { return ( obj_.get() == 0 ) ? 0 : &Shape::obj_; }
    void reset() { obj_.reset(); }

  };



	/**
	 * A basic Quad with no indexing.
	 * Quad provides a basic contructor that does little more than build the ShapeObjectQuad
	 */ 

	class SEBUROAPI Quad : public Shape<Vertex4, Face4, AllocationPolicyNew> {
	public:

		Quad() {};
		Quad(float w, float h);

	
	};

	/**
	 * A basic Cuboid, made up of Quads with the underlying Vec4 type
	 */ 


	class SEBUROAPI Cuboid : public Shape<Vertex4, Face4, AllocationPolicyNew> {
	public:

		Cuboid() {};
		Cuboid(float w, float h, float d);
	
	};


  /**
   * A basic Sphere, made up of triangles with the underlying Vec4 type
   */ 

  class SEBUROAPI Sphere : public Shape<Vertex4, Face4, AllocationPolicyNew> {
  public:

    Sphere() {};
    Sphere(float_t radius, size_t segments);
  
  };

	
  /**
   * A mesh made up of triangles
   */

  class SEBUROAPI TriMesh : public Shape<Vertex3, Face3, AllocationPolicyNew> {

  public:

    TriMesh() {};
    TriMesh(IndicesType num_verts, size_t num_indices);

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

  };


  /**
   * A Soup of non-indexed vertices. Basically just a place-holder for flat geometry. No Faces, just a
   * vertex list
   */

  class SEBUROAPI VertexSoup : public Shape<Vertex3, Face3, AllocationPolicyNew> {

  public:

    VertexSoup() {};
    VertexSoup(IndicesType num_verts);

  };


  /// So far, the only shape that shares data is a set of Triangles, SharedTriMesh

  class SEBUROAPI SharedTriMesh : public Shape<Vertex3, Face3, AllocationPolicyShared> {

  public:
    SharedTriMesh() {};
    SharedTriMesh(Shape<Vertex3, Face3, AllocationPolicyNew> shared, IndicesType num_indices) : Shape<Vertex3, Face3, AllocationPolicyShared>(shared){
      obj_->geometry = GeometryT<Vertex3, Face3, AllocationPolicyShared> (obj_->shared.geometry().GetSharedVertices(), num_indices, TRIANGLES);
    }

  };

  /**
   * A basic Cylinder
   */ 

  class SEBUROAPI Cylinder : public Shape<Vertex3, Face3, AllocationPolicyNew> {
  public:

    Cylinder() {};
    Cylinder(size_t segments, size_t stacks, float diameter, float height);
  
  };

  /**
   * A pointed Cyclinder - spike thing :S
   */ 
 
  class SEBUROAPI Spike : public Shape<Vertex3, Face3, AllocationPolicyNew> {
  public:

    Spike() {};
    Spike(size_t segments, size_t stacks, float radius, float height);
  
  };

}


#endif

