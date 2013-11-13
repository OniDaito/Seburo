/**
* @brief DrawableClass. Takes geometry and creates OpenGL
* @file drawable.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/10/2013
*
*/

#ifndef S9_GL_DRAWABLE_HPP
#define S9_GL_DRAWABLE_HPP

#include "../common.hpp"
#include "common.hpp"
#include "utils.hpp"
#include "../geometry.hpp"


namespace s9{
  
  namespace gl {

    /// BufferRole - what each buffer actually represents. Per Vertex buffers.
    typedef enum {
      INTERLEAVED_BUFFER,
      INDEX_BUFFER,
      POSITION_BUFFER,
      COLOUR_BUFFER,
      NORMAL_BUFFER,
      UV_BUFFER,
      TANGENT_BUFFER
    } BufferRole;


    /// BrewFlags - user defined flags for drawing
    struct BrewFlags {
      BrewFlags(bool b, GLint a){
        interleaved = b;
        access = a;
      }

      bool interleaved = true;
      GLint access = GL_STATIC_DRAW;
    };

    static const BrewFlags BrewFlagsDefault = { true, GL_STATIC_DRAW };

    /**
     * Drawable Class - Provides functionality to the Shape class, allowing it 
     * to be drawn to the screen. Keeps track of buffers but only holds primitive types
     * This is loosely coupled with its geometry, only at brew time.
     */

    class Drawable {
    public:

      Drawable(){ vao_ = 0; brewed_ = false; }

      bool brewed() {return brewed_; }
      
      /// \todo unbrew method for cleaning data off the card
      /// Options for brewing that may need to be specified
      template< typename VertexType, typename FaceType, typename AllocationPolicy> 
      void draw(GeometryT<VertexType, FaceType, AllocationPolicy> &g, GeometryPrimitive gp) {
        if (vao_ == 0){
          std::cerr << "SEBURO DRAWABLE ERROR - attempting to draw a shape that is not brewed." << std::endl;
          return; 
        }

        bind();
        // Choose type based on the geometry with option of an override
        GLint type;
        if (gp == NONE)
          gp = g.prim_type();

        switch (gp){
          case TRIANGLES:
            type = GL_TRIANGLES;
            break;
          case POINTS:
            type = GL_POINTS;
            break;

          default:
            assert(false);
        }
                
        if ( g.indexed() ){
          /// \todo indices type matching GL_UNSIGNED_INT
          glDrawElements(type, g.size_indices(), GL_UNSIGNED_INT, 0);
        } else {
          glDrawArrays(type, 0, g.size_vertices());
        }

        unbind();
      };
      
      /// Template functions for the brewing of Geometry
      ///\todo allow rebrewing with the same size 

      template< typename VertexType, typename FaceType, typename AllocationPolicy> 
      void brew( GeometryT<VertexType, FaceType, AllocationPolicy> &g, BrewFlags b=BrewFlagsDefault) {
        glGenVertexArrays(1, &(vao_));
        
        // for now, always interleaved
        if (b.interleaved || true){
          handles_.push_back(0);
        } else {
          ///\todo non interleaved buffers
          assert(false);
        }

        /// Last handle is always the indices

        if (g.indexed()) {
          handles_.push_back(0);
        }

        glGenBuffers(handles_.size(), &(handles_[0]));
        allocate(g,b);
        setPointers(g,b);

        brewed_ = true;

      }
      
      /// Basic destruction of the buffers created
      ~Drawable() {
        glDeleteBuffers(handles_.size(), &(handles_[0]));
      } 


      void bind() { glBindVertexArray(vao_);}

      void unbind() { glBindVertexArray(0); }
      
    protected:

      /// allocation of the data into the buffer - copy to the card
      /// default template function should never be called.

      template< typename VertexType, typename FaceType, typename AllocationPolicy> 
      void allocate (GeometryT<VertexType,FaceType,AllocationPolicy> &g, BrewFlags b){
        STATIC_CHECK(false, Allocate_must_be_specialized_with_geometry);
      }

      template< typename AllocationPolicy> 
      void allocate(GeometryT<Vertex2, Face3, AllocationPolicy> &g, BrewFlags b) {

      }
      
      template< typename AllocationPolicy> 
      void allocate(GeometryT<Vertex3, Face3, AllocationPolicy> &g, BrewFlags b) {
        glBindBuffer(GL_ARRAY_BUFFER, handles_[0]);
        glBufferData(GL_ARRAY_BUFFER, g.size_vertices() * sizeof(Vertex3), &(g.vertices()[0]), b.access);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        if (g.indexed()){
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles_[1]);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, g.size_indices() * sizeof(IndicesType), &(g.indices()[0]), b.access);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
      }
      
      template< typename AllocationPolicy> 
      void allocate(GeometryT<Vertex4, Face3, AllocationPolicy> &g, BrewFlags b) {

      }
      
      template< typename AllocationPolicy> 
      void allocate(GeometryT<Vertex2, Face4, AllocationPolicy> &g, BrewFlags b) {

      }

      template< typename AllocationPolicy> 
      void allocate(GeometryT<Vertex3, Face4, AllocationPolicy> &g, BrewFlags b) {

        glBindBuffer(GL_ARRAY_BUFFER, handles_[0]);
        glBufferData(GL_ARRAY_BUFFER, g.size_vertices() * sizeof(Vertex3), &(g.vertices()[0]), b.access);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        if (g.indexed()){
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles_[1]);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, g.size_indices() * sizeof(IndicesType), &(g.indices()[0]), b.access);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

      }

      template< typename AllocationPolicy> 
      void allocate(GeometryT<Vertex4, Face4, AllocationPolicy> &g, BrewFlags b) {
      
        glBindBuffer(GL_ARRAY_BUFFER, handles_[0]);
        glBufferData(GL_ARRAY_BUFFER, g.size_vertices() * sizeof(Vertex4), &(g.vertices()[0]), b.access);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        if (g.indexed()){
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles_[1]);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, g.size_indices() * sizeof(IndicesType), &(g.indices()[0]), b.access);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
      }


      template< typename VertexType, typename FaceType, typename AllocationPolicy> 
      void setPointers(GeometryT<VertexType,FaceType,AllocationPolicy> &g, BrewFlags b) {
        assert(false);
      }

      template<typename AllocationPolicy> 
      void setPointers(GeometryT<Vertex4, Face4, AllocationPolicy> &g, BrewFlags b) {
      
        bind();

        if (b.interleaved) {

          glBindBuffer(GL_ARRAY_BUFFER, handles_[0]);
          glEnableVertexAttribArray(0); // Pos
          glEnableVertexAttribArray(1); // Normal
          glEnableVertexAttribArray(2); // colour
          glEnableVertexAttribArray(3); // texture
          glEnableVertexAttribArray(4); // tangent

          uint32_t idx = 0;

          glVertexAttribPointer(idx++,4, GL_FLOAT, GL_FALSE, sizeof(Vertex4), (GLvoid*)offsetof( Vertex4, p) );
          glVertexAttribPointer(idx++,4, GL_FLOAT, GL_FALSE, sizeof(Vertex4), (GLvoid*)offsetof( Vertex4, n) );
          glVertexAttribPointer(idx++,4, GL_FLOAT, GL_FALSE, sizeof(Vertex4), (GLvoid*)offsetof( Vertex4, c) );
          glVertexAttribPointer(idx++,2, GL_FLOAT, GL_FALSE, sizeof(Vertex4), (GLvoid*)offsetof( Vertex4, u) );
          glVertexAttribPointer(idx++,4, GL_FLOAT, GL_FALSE, sizeof(Vertex4), (GLvoid*)offsetof( Vertex4, t) );

          // Indices
          if (g.indexed()){
            glEnableVertexAttribArray(5); // Indices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles_[1]);

            ///\todo GL_UNSIGNED_INT must match the IndiciesType

            glVertexAttribPointer(idx, 1, GL_UNSIGNED_INT, GL_FALSE,0, (GLubyte*) NULL);
          } 
        }

        unbind();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
      }

      template<typename AllocationPolicy>
      void setPointers(GeometryT<Vertex3, Face3, AllocationPolicy> &g, BrewFlags b) {
      
        bind();

        if (b.interleaved) {

          glBindBuffer(GL_ARRAY_BUFFER, handles_[0]);
          glEnableVertexAttribArray(0); // Pos
          glEnableVertexAttribArray(1); // Normal
          glEnableVertexAttribArray(2); // colour
          glEnableVertexAttribArray(3); // texture
          glEnableVertexAttribArray(4); // tangent

          uint32_t idx = 0;

          glVertexAttribPointer(idx++,3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (GLvoid*)offsetof( Vertex3, p) );
          glVertexAttribPointer(idx++,3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (GLvoid*)offsetof( Vertex3, n) );
          glVertexAttribPointer(idx++,3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (GLvoid*)offsetof( Vertex3, c) );
          glVertexAttribPointer(idx++,2, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (GLvoid*)offsetof( Vertex3, u) );
          glVertexAttribPointer(idx++,3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (GLvoid*)offsetof( Vertex3, t) );

          // Indices
          if (g.indexed()){
            glEnableVertexAttribArray(5); // Indices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles_[1]);

            ///\todo GL_UNSIGNED_INT must match the IndiciesType

            glVertexAttribPointer(idx, 1, GL_UNSIGNED_INT, GL_FALSE,0, (GLubyte*) NULL);
          } 
        }

        unbind();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
      }

  
      GLuint vao_;
      bool brewed_;
      std::vector<unsigned int> handles_;

    };



  }

}


#endif