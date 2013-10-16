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

      Drawable(){}
  

      /// Options for brewing that may need to be specified
     
      virtual void draw() {};
      
      // Template functions for the brewing of Geometry
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

        if (g.indexed()) {
          handles_.push_back(0);
        }

        glGenBuffers(handles_.size(), &(handles_[0]));

        allocate(g,b);

        bind();

      }

      void bind() { glBindVertexArray(vao_);}

      void unbind() { glBindVertexArray(0); }
      
    protected:


      template< typename VertexType, typename FaceType, typename AllocationPolicy> 
      void allocate (GeometryT<VertexType,FaceType,AllocationPolicy> &g, BrewFlags b){
        assert(false);
      }

      template< typename AllocationPolicy> 
      void allocate(GeometryT<Vertex2, Face3, AllocationPolicy> &g, BrewFlags b) {

      }
      
      template< typename AllocationPolicy> 
      void allocate(GeometryT<Vertex3, Face3, AllocationPolicy> &g, BrewFlags b) {

      }
      
      template< typename AllocationPolicy> 
      void allocate(GeometryT<Vertex4, Face3, AllocationPolicy> &g, BrewFlags b) {

      }
      
      template< typename AllocationPolicy> 
      void allocate(GeometryT<Vertex2, Face4, AllocationPolicy> &g, BrewFlags b) {

      }

      template< typename AllocationPolicy> 
      void allocate(GeometryT<Vertex3, Face4, AllocationPolicy> &g, BrewFlags b) {

      }

      template< typename AllocationPolicy> 
      void allocate(GeometryT<Vertex4, Face4, AllocationPolicy> &g, BrewFlags b) {
      }

  
      GLuint vao_;
      std::vector<unsigned int> handles_;

    };



  }

}


#endif