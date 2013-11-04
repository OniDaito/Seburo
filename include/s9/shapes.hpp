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

		gl::Drawable gl_drawable;

	};


	/**
	 * Basic interface to shapes
	 * \todo brew and draw go through a lot of calls - worrying :S
	 */

	class SEBUROAPI Shape {

	public:
		Shape () {brewed_ = false; drawable_ = false; };
		Shape (bool drawable) : drawable_(drawable) { brewed_ = false; }

		virtual void draw(GeometryPrimitive g = TRIANGLES) { if (obj_ != nullptr) obj_->draw(g); };
		virtual void brew(gl::BrewFlags b=gl::BrewFlagsDefault) { if (obj_ != nullptr)  { obj_->brew(b); brewed_ = true; } };
		virtual bool brewed() { return brewed_; }
		virtual bool drawable() { return drawable_; }

		const Shape& operator= (const Shape &s ) { 
			obj_ = s.obj_; 
			brewed_ = s.brewed_; 
			drawable_ = s.drawable_; 
			return *this;
		}

	protected:
		bool brewed_;
		bool drawable_;

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



  class SEBUROAPI TriMesh : public Shape {

  public:

    TriMesh() {};
    TriMesh(size_t num_verts, size_t num_indices);

    const GeometryT<Vertex3, Face3, AllocationPolicyNew>* geometry();

  };

}


#endif

