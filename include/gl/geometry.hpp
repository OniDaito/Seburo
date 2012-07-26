/**
* @brief OpenGL Specfic Primitives and such
* @file shapes.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 18/07/2012
*
*/

#ifndef GL_GEOMETRY_HPP
#define GL_GEOMETRY_HPP

#include "../common.hpp"
#include "common.hpp"
#include "utils.hpp"
#include "../primitive.hpp"
#include "../asset.hpp"


namespace s9 {

	namespace gl {

		/*
		 * This is a mixture of basic geometry and a primtive for OpenGL
		 * We inherit the asset and ViaVAO classes
		 * \todo provide convinience methods to link directly through to loading models
		 * \todo do we even need to mix asset with primitive? - Possibly as Primitive provides hierarchy!
		 * \todo what about if we pass an asset? What do we do about potential sub assets? - COPY CONSTRUCTOR! :D
		 */

		template <class T>
		class Geometry : public Asset<T>, public ViaVAO {

		protected:
			virtual void _gen() {}
		
		public:
			Geometry() {};
			Geometry(T a) : Asset<T>(a) {  mVAO = 0; };
			Geometry(Asset<T> b) : Asset<T>(b) { mVAO = 0; }

			// Override this 
			virtual void draw() {
				if(mVAO == 0) _gen();
				
				bind();
				if ( this->mGeom.indexsize() > 0)
					glDrawElements(GL_TRIANGLES, this->mGeom.indexsize(), GL_UNSIGNED_INT, 0);
				else
					glDrawArrays(GL_TRIANGLES,0, this->mGeom.size());

				unbind();
				CXGLERROR
			 }

		

		};

		/*
		 * Creating a VAO around a basic Asset
		 */

		template<>
		inline void Geometry<GeometryPNF>::_gen() {
			glGenVertexArrays(1,&mVAO);
	
			unsigned int handle[2];
			glGenBuffers(2,handle);

			glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
			glBufferData(GL_ARRAY_BUFFER, this->mGeom.size() * sizeof(VertPNF), this->mGeom.addr(), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mGeom.indexsize() * sizeof(uint32_t), this->mGeom.indexaddr(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

			bind();

			glBindBuffer(GL_ARRAY_BUFFER, handle[0]);

			glEnableVertexAttribArray(0); // Pos
			glEnableVertexAttribArray(1); // Normal
			glEnableVertexAttribArray(2); // Indices


			glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(VertPNF), (GLvoid*)offsetof(VertPNF,mP) );
			glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(VertPNF), (GLvoid*)offsetof(VertPNF,mN) );

			// Indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[1]);
			glVertexAttribPointer(2, 1,GL_UNSIGNED_INT,GL_FALSE,0, (GLubyte*) NULL);

			unbind();

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

			CXGLERROR
		}

	}
}

#endif