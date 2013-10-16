/**
* @brief Drawable Class
* @file drawable.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/10/2013
*
*/

#include "s9/gl/drawable.hpp"

using namespace std;
using namespace s9;
using namespace s9::gl;






/**
 * DrawableT Constructor
 */
/*
template <class T, class U>
DrawableT<T,U>::DrawableT (GeometryT<T,U> geometry) : obj_( shared_ptr<SharedObj>(new SharedObj(geometry))) {}
*/


/**
 * Generate - generate a vao and some buffers depending on geometry
 */
/*
template <class T, class U>
void DrawableT<T,U>::generate(BrewFlags b) {
  
  // Generate the number and kind of buffers required

  glGenVertexArrays(1, &(obj_->vao));
  
  int s = 0;

  /// \todo non-interleaved buffers

  if (b.interleaved){
    if (obj_->shape.vertices.size() > 0) s++;
    if (obj_->shape.indices.size() > 0 && obj_->shape.indexed_) s++;
  }

  obj_->numbuffers = s;
  obj_->handle = new unsigned int[s];
  obj_->buffer_roles = new BufferRole[s];

  glGenBuffers(s, obj_->handle);

  // Organise the roles
  if (b.interleaved){
    if (obj_->shape.indexed_){
      /// index is the last buffer
      allocateIndexBuffer(b.access, obj_->numbuffers-1);
      obj_->buffer_roles[obj_->numbuffers-1] = INDEX_BUFFER;
    }
  }
  */

  // Allocation of memory on the card

  /*bind();

  glBindBuffer(GL_ARRAY_BUFFER, obj_->handle[0]);

  glEnableVertexAttribArray(0); // Pos
  glEnableVertexAttribArray(1); // Normal

  glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, getGeometry().elementsize(), (GLvoid*)offsetof(VertPNF,mP) );
  glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, getGeometry().elementsize(), (GLvoid*)offsetof(VertPNF,mN) );

  // Indices
  if (getGeometry().indexsize() > 0){
    glEnableVertexAttribArray(2); // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj_->handle[1]);
    glVertexAttribPointer(2, 1,GL_UNSIGNED_INT,GL_FALSE,0, (GLubyte*) NULL);
  }

  unbind();

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);*/
//}




/**
 * allocate - move geometry data into the VAO
 */
/*
template <class T, class U>
void DrawableT<T,U>::allocateIndexBuffer(GLint access, int handle) {

  /// the last buffer is always the index buffer

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj_->handle[handle]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj_->shape.indices.size() * sizeof(uint32_t), &(obj_->shape.indices[0]), access);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

}*/

/**
 * allocateDataBuffer - create a data buffer
 */


/*
template <class T, class U>
void DrawableT<T,U>::allocateDataBuffer (GLint access, int handle, BufferRole role) { }

template <class U>
void DrawableT< Vertex2, U>::allocateDataBuffer (GLint access, int handle, BufferRole role) { }


template <class U>
void DrawableT< Vertex3, U>::allocateDataBuffer (GLint access, int handle, BufferRole role) { }


template <class U>
void DrawableT< Vertex4, U>::allocateDataBuffer (GLint access, int handle, BufferRole role) { }

*/

/**
 * brew - actually create the buffers now we are setup
 */

/*template <class T, class U>
void DrawableT<T,U>::brew(BrewFlags b) {
  
  // Has this been brewed already?
  if (obj_->vao == 0 ){
    generate(b);
  }

  /// \todo check the flags on the geometry for dirty or resized

}*/

/*
  virtual void _update() {
        glBindBuffer(GL_ARRAY_BUFFER, _handle[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, getGeometry().size() * getGeometry().elementsize(), getGeometry().addr());
        getGeometry().setDirty(false);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
      }
      */



/**
 * draw - Traverse the hierarchy and perform drawing and composition
 */

/*template <class T, class U>
void DrawableT<T,U>::draw( ) {

}*/


/**
 * DNode drawToScreen - actually perform the draw call
 */
