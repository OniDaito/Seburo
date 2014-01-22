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
	 * Shared Object container for basic shape geometries - OpenGL at present
   * \todo can we template this class and ergo, have a base pointer we can always get to?
	 */

	struct ShapeObj {

		virtual void Brew(gl::BrewFlags b) = 0;
		virtual void Draw(GeometryPrimitive g) = 0;

		bool brewed() { return gl_drawable.brewed(); }

		gl::Drawable gl_drawable;

	};


	/**
	 * Basic interface to shapes
	 * \todo brew and draw go through a lot of calls - worrying :S
	 */

	class SEBUROAPI Shape {

	public:
		Shape () {};
		
		virtual void Draw(GeometryPrimitive g = TRIANGLES) { CXSHARED obj_->Draw(g); };
		virtual void Brew(gl::BrewFlags b=gl::BrewFlagsDefault) { CXSHARED  obj_->Brew(b);  };
		virtual bool brewed() { if(obj_ != nullptr) return obj_->brewed(); return false;} //\todo should be IsBrewed()
		virtual void Init() {};

		const Shape& operator= (const Shape &s ) { 
			obj_ = s.obj_; 
			return *this;
		}

		inline bool operator==(const Shape& lhs){ return lhs.obj_ == obj_;  }
		inline bool operator!=(const Shape& lhs){return lhs.obj_ != obj_;}

    const ShapeObj& obj() const {return *obj_; }


	protected:
	
		std::shared_ptr<ShapeObj> obj_ = nullptr;

  public:
    
    typedef std::shared_ptr<ShapeObj> Shape::*unspecified_bool_type;
    operator unspecified_bool_type() const { return ( obj_.get() == 0 ) ? 0 : &Shape::obj_; }
    void reset() { obj_.reset(); }

	};

	/**
	 * A basic Quad with no indexing.
	 * Quad provides a basic contructor that does little more than build the ShapeObjectQuad
	 */ 

	struct ShapeObjQuad : public ShapeObj {

		ShapeObjQuad() {
			geometry = GeometryT<Vertex4, Face4, AllocationPolicyNew> (6,0,TRIANGLES);	
		}

		void Brew (gl::BrewFlags b) { gl_drawable.Brew(geometry, b);}
		void Draw (GeometryPrimitive g = TRIANGLES) { gl_drawable.Draw(geometry, g); }

		GeometryT<Vertex4, Face4, AllocationPolicyNew> geometry;
			
	};

	class SEBUROAPI Quad : public Shape {
	public:

		Quad() {};
		Quad(float w, float h);

		GeometryT<Vertex4, Face4, AllocationPolicyNew>* geometry();

	};

	/**
	 * A basic Cuboid, made up of Quads with the underlying Vec4 type
	 */ 

	struct ShapeObjCuboid : public ShapeObj {
		ShapeObjCuboid() {
			geometry = GeometryT<Vertex4, Face4, AllocationPolicyNew>(8,36,TRIANGLES);
		}

		void Brew (gl::BrewFlags b) { gl_drawable.Brew(geometry, b); }
		void Draw (GeometryPrimitive g = TRIANGLES) { gl_drawable.Draw(geometry, g); }

		GeometryT<Vertex4, Face4, AllocationPolicyNew> geometry;
			
	};

	class SEBUROAPI Cuboid : public Shape {
	public:

		Cuboid() {};
		Cuboid(float w, float h, float d);

		GeometryT<Vertex4, Face4, AllocationPolicyNew>* geometry();
	
	};


  /**
   * A basic Sphere, made up of triangles with the underlying Vec4 type
   */ 

  struct ShapeObjSphere: public ShapeObj {
    ShapeObjSphere(IndicesType num_verts, size_t num_indices) {
      geometry = GeometryT<Vertex4, Face4, AllocationPolicyNew>(num_verts,num_indices,TRIANGLES);
    }

    void Brew (gl::BrewFlags b) { gl_drawable.Brew(geometry, b); }
    void Draw (GeometryPrimitive g = TRIANGLES) { gl_drawable.Draw(geometry, g); }

    GeometryT<Vertex4, Face4, AllocationPolicyNew> geometry;
      
  };


  class SEBUROAPI Sphere : public Shape {
  public:

    Sphere() {};
    Sphere(float_t radius, size_t segments);

    GeometryT<Vertex4, Face4, AllocationPolicyNew>* geometry();
  
  };

	/**
   * A Triangle Mesh made up of Vec3 types
   */ 


  struct ShapeObjTriMesh : public ShapeObj {

    ShapeObjTriMesh(IndicesType num_verts, size_t num_indices) {
      geometry = GeometryT<Vertex3, Face3, AllocationPolicyNew>(num_verts, num_indices, TRIANGLES);
    }

  	void Brew (gl::BrewFlags b) { gl_drawable.Brew(geometry, b); }
  	void Draw (GeometryPrimitive g = TRIANGLES) { gl_drawable.Draw(geometry, g); }

    GeometryT<Vertex3, Face3, AllocationPolicyNew> geometry;
      
  };

  /**
   * A mesh made up of triangles
   */

  class SEBUROAPI TriMesh : public Shape {

  public:

    TriMesh() {};
    TriMesh(IndicesType num_verts, size_t num_indices);

    GeometryT<Vertex3, Face3, AllocationPolicyNew>* geometry();

  };

  /**
   * A Triangle Mesh for skinning - using per vertex data for its weights
   */ 

  struct ShapeObjTriMeshSkinned : public ShapeObj {

    ShapeObjTriMeshSkinned(IndicesType num_verts, size_t num_indices) {
      geometry = GeometryT<Vertex3Skin, Face3, AllocationPolicyNew>(num_verts, num_indices, TRIANGLES);
    }

  	void Brew (gl::BrewFlags b) { gl_drawable.Brew(geometry, b); }
  	void Draw (GeometryPrimitive g = TRIANGLES) { gl_drawable.Draw(geometry, g); }

    GeometryT<Vertex3Skin, Face3, AllocationPolicyNew> geometry;
      
  };

  /**
   * A mesh made up of triangles and skinned
   */

  class SEBUROAPI TriMeshSkinned : public Shape {

  public:

    TriMeshSkinned() {};
    TriMeshSkinned(IndicesType num_verts, size_t num_indices);

    GeometryT<Vertex3Skin, Face3, AllocationPolicyNew>* geometry();

  };


  /**
   * A Soup of non-indexed vertices. Basically just a place-holder for flat geometry. No Faces, just a
   * vertex list
   */


   struct ShapeObjVertexSoup : public ShapeObj {

    ShapeObjVertexSoup(IndicesType num_verts ) {
      geometry = GeometryT<Vertex3, Face3, AllocationPolicyNew>(num_verts, 0, NONE);
    }

    void Brew (gl::BrewFlags b) { gl_drawable.Brew(geometry, b); }
    void Draw (GeometryPrimitive g = TRIANGLES) { gl_drawable.Draw(geometry, g); }

    GeometryT<Vertex3, Face3, AllocationPolicyNew> geometry;
      
  };

  class SEBUROAPI VertexSoup : public Shape {

  public:

    VertexSoup() {};
    VertexSoup(IndicesType num_verts);

    GeometryT<Vertex3, Face3, AllocationPolicyNew>* geometry();

  };


  /**
   * Shared Triangle Mesh. This is basically a set of indices, grouped as triangles, that can "look" 
   * at another array of vertices on the graphics card to get their data. At present, just VertexSoups are 
   * allowed.
   */

    struct ShapeObjSharedTriMesh : public ShapeObj {

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
      
  };

  /// So far, the only shape that shares data is a set of Triangles, SharedTriMesh

  class SEBUROAPI SharedTriMesh : public Shape {

  public:
    SharedTriMesh() {};
    SharedTriMesh(VertexSoup &shared, IndicesType num_indices);

    GeometryT<Vertex3, Face3, AllocationPolicyShared>* geometry();

  };




  /**
   * A basic Cylinder
   */ 

  struct ShapeObjCylinder : public ShapeObj {
    ShapeObjCylinder(size_t s, size_t f) {
      geometry = GeometryT<Vertex3, Face3, AllocationPolicyNew>(s,f,TRIANGLES);
    }

    void Brew (gl::BrewFlags b) { gl_drawable.Brew(geometry, b); }
    void Draw (GeometryPrimitive g = TRIANGLES) { gl_drawable.Draw(geometry, g); }

    GeometryT<Vertex3, Face3, AllocationPolicyNew> geometry;
      
  };

  class SEBUROAPI Cylinder : public Shape {
  public:

    Cylinder() {};
    Cylinder(size_t segments, size_t stacks, float diameter, float height);

    GeometryT<Vertex3, Face3, AllocationPolicyNew>* geometry();
  
  };

  /**
   * A pointed Cyclinder - spike thing :S
   */ 

  struct ShapeObjSpike : public ShapeObj {
    ShapeObjSpike(size_t s, size_t f) {
      geometry = GeometryT<Vertex3, Face3, AllocationPolicyNew>(s,f,TRIANGLES);
    }

    void Brew (gl::BrewFlags b) { gl_drawable.Brew(geometry, b); }
    void Draw (GeometryPrimitive g = TRIANGLES) { gl_drawable.Draw(geometry, g); }

    GeometryT<Vertex3, Face3, AllocationPolicyNew> geometry;
      
  };

  class SEBUROAPI Spike : public Shape {
  public:

    Spike() {};
    Spike(size_t segments, size_t stacks, float radius, float height);

    GeometryT<Vertex3, Face3, AllocationPolicyNew>* geometry();
  
  };

}


#endif

