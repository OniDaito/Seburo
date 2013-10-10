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
#include "../node.hpp"


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


    /**
     * DrawableBase - this provides an interface for other classes to play with DrawableT
     * It is composed with Node to make the Drawable Node (DNode)
     */

    class DrawableBase {
    public:
      virtual void draw() = 0;
      virtual void brew() = 0;
      virtual inline void bind() = 0;
      virtual inline void unbind() = 0;

    };

    /**
     * The DrawbleT Template class. Uses composition to create OpenGL Buffers
     */

    template<class T, class U>
    class SEBUROAPI DrawableT : DrawableBase {
    public:
      DrawableT( GeometryT<T,U> &shape );
      
      /// Options for brewing that may need to be specified
      struct BrewFlags {
        bool interleaved = true;
        GLint access = GL_STATIC_DRAW;
      };


      void brew(BrewFlags b);

      /// Bind the vao to the OpenGL Context
      inline void bind() { glBindVertexArray(obj_->vao); } ;

      /// Unbind the context
      inline void unbind() { glBindVertexArray(0); };

      virtual void draw();

    protected:

      void allocateIndexBuffer(GLint method, int handle);
      void allocateDataBuffer(GLint method, int handle, BufferRole role);
      void generate(BrewFlags b);

      struct SharedObj {

        SharedObj (  GeometryT<T,U> &shape ) : shape(shape) {};

        GeometryT<T,U> &shape;
        GLuint vao;
        unsigned int *handle;
        uint8_t numbuffers;
        BufferRole *buffer_roles;

      };

      std::shared_ptr<SharedObj> obj_;
    };

    /**
     * Specialiased template classes for the different vertex types
     */

    template <class U>
    class SEBUROAPI DrawableT<Vertex2, U> : DrawableBase {
    public:
      void allocateDataBuffer (GLint method, int handle, BufferRole role);
    };

    template <class U>
    class SEBUROAPI DrawableT<Vertex3, U> : DrawableBase {
    public:
      void allocateDataBuffer (GLint method, int handle, BufferRole role);
    };

    template <class U>
    class SEBUROAPI DrawableT<Vertex4, U> : DrawableBase {
    public:
      void allocateDataBuffer (GLint method, int handle, BufferRole role);
    };


    /**
     * DNode - A node composed with a drawable to create something that can be drawn to the screen
     * Like Node, DNode has no sharable object. Maybe not the best idea?
     */

    class DNode : public Node {
    public:
      DNode (DrawableBase &dbase) : drawable_(dbase), Node() {}


    protected:
      void drawToScreen();
      DrawableBase &drawable_;

    };


  }


}


#endif