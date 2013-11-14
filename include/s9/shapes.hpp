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

namespace s9 {
	
	/**
	 * Shared Object container for basic shape geometries - OpenGL at present
	 */

	struct ShapeObj {

		virtual void brew(gl::BrewFlags b) = 0;
		virtual void draw(GeometryPrimitive g) = 0;

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
		
		virtual void draw(GeometryPrimitive g = TRIANGLES) { if (obj_ != nullptr) obj_->draw(g); };
		virtual void brew(gl::BrewFlags b=gl::BrewFlagsDefault) { if (obj_ != nullptr)  { obj_->brew(b); } };
		virtual bool brewed() { if(obj_ != nullptr) return obj_->brewed(); return false;}

		const Shape& operator= (const Shape &s ) { 
			obj_ = s.obj_; 
			return *this;
		}

	protected:
	
		std::shared_ptr<ShapeObj> obj_ = nullptr;

	};

	/**
	 * A basic Quad with no indexing.
	 * Quad provides a basic contructor that does little more than build the ShapeObjectQuad
	 */ 

	struct ShapeObjQuad : public ShapeObj {

		ShapeObjQuad() {
			geometry = GeometryT<Vertex4, Face4, AllocationPolicyNew> (6,0,TRIANGLES);	
		}

		void brew (gl::BrewFlags b) { gl_drawable.brew(geometry, b);}
		void draw (GeometryPrimitive g = TRIANGLES) { gl_drawable.draw(geometry, g); }

		GeometryT<Vertex4, Face4, AllocationPolicyNew> geometry;
			
	};

	class SEBUROAPI Quad : public Shape {
	public:

		Quad() {};
		Quad(float w, float h);

		const GeometryT<Vertex4, Face4, AllocationPolicyNew>* geometry();

	};

	/**
	 * A basic Cuboid, made up of Quads with the underlying Vec4 type
	 */ 

	struct ShapeObjCuboid : public ShapeObj {
		ShapeObjCuboid() {
			geometry = GeometryT<Vertex4, Face4, AllocationPolicyNew>(8,36,TRIANGLES);
		}

		void brew (gl::BrewFlags b) { gl_drawable.brew(geometry, b); }
		void draw (GeometryPrimitive g = TRIANGLES) { gl_drawable.draw(geometry, g); }

		GeometryT<Vertex4, Face4, AllocationPolicyNew> geometry;
			
	};

	class SEBUROAPI Cuboid : public Shape {
	public:

		Cuboid() {};
		Cuboid(float w, float h, float d);

		const GeometryT<Vertex4, Face4, AllocationPolicyNew>* geometry();
	
	};

	/**
   * A Triangle Mesh made up of Vec3 types
   */ 


  struct ShapeObjTriMesh : public ShapeObj {

    ShapeObjTriMesh(size_t num_verts, size_t num_indices) {
      geometry = GeometryT<Vertex3, Face3, AllocationPolicyNew>(num_verts, num_indices, TRIANGLES);
    }

  	void brew (gl::BrewFlags b) { gl_drawable.brew(geometry, b); }
  	void draw (GeometryPrimitive g = TRIANGLES) { gl_drawable.draw(geometry, g); }

    GeometryT<Vertex3, Face3, AllocationPolicyNew> geometry;
      
  };

  /**
   * A mesh made up of triangles
   */

  class SEBUROAPI TriMesh : public Shape {

  public:

    TriMesh() {};
    TriMesh(size_t num_verts, size_t num_indices);

    const GeometryT<Vertex3, Face3, AllocationPolicyNew>* geometry();

  };

  /**
   * A Triangle Mesh for skinning - using per vertex data for its weights
   */ 

  struct ShapeObjTriMeshSkinned : public ShapeObj {

    ShapeObjTriMeshSkinned(size_t num_verts, size_t num_indices) {
      geometry = GeometryT<Vertex3Skin, Face3, AllocationPolicyNew>(num_verts, num_indices, TRIANGLES);
    }

  	void brew (gl::BrewFlags b) { gl_drawable.brew(geometry, b); }
  	void draw (GeometryPrimitive g = TRIANGLES) { gl_drawable.draw(geometry, g); }

    GeometryT<Vertex3Skin, Face3, AllocationPolicyNew> geometry;
      
  };

  /**
   * A mesh made up of triangles and skinned
   */

  class SEBUROAPI TriMeshSkinned : public Shape {

  public:

    TriMeshSkinned() {};
    TriMeshSkinned(size_t num_verts, size_t num_indices);

    const GeometryT<Vertex3Skin, Face3, AllocationPolicyNew>* geometry();

  };

}


#endif

