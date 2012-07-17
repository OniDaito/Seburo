/**
* @brief OpenGL Renderer - Renders basic objects given a Primitive and Geometry
* @file shapes.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 06/07/2012
*
*/


#ifndef S9GLOBJECT_HPP
#define S9GLOBJECT_HPP

#include "common.hpp"
#include "primitive.hpp"
#include "geom.hpp"

namespace s9 {

	/*
	 * S9 GL records the OpenGL State for a primitive and a set of Geometry
	 */
	
	template <class T>
	class S9GLObject{
	public:
		S9GLObject() {};
		
	protected:
		struct SharedObj {
			GLuint mVAO;
			std::vector<GLuint> vVBO;
			GLuint mNum;
			T mGeom;
			Primitive mPrimitive;
		};
		
		boost::shared_ptr<SharedObj> mObj;
		
		void _genOne() {
			mObj->vVBO.push_back(0);
			glGenBuffers(1,&(mObj->vVBO[0]));
			glGenVertexArrays(1,&(mObj->mVAO));
			glBindVertexArray(mObj->mVAO);
		}
		
		void _genIndices(T g){
			mObj->vVBO.push_back(0);
			glGenBuffers(1,&(mObj->vVBO.back()));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mObj->vVBO.back());
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, g.indexsize() * sizeof(size_t), g.indexaddr(), GL_STATIC_DRAW);
		}
		
	public:
	
		virtual operator int() const { return mObj.use_count() > 0; };
			
		S9GLObject(T g, Primitive p) {
			mObj.reset(new SharedObj());
			mObj->mPrimitive = p;
			mObj->mGeom = g;
		};
	
		virtual ~S9GLObject() {
			glDeleteBuffers(mObj->vVBO.size(), &(mObj->vVBO[0]));
			glDeleteVertexArrays(1, &(mObj->mVAO));
		}
	
		void bind() { glBindVertexArray(mObj->mVAO); }
		
		void unbind() { 
			glBindVertexArray(0);
			for (int id = 0; id < mObj->vVBO.size(); ++ id)
				glDisableVertexAttribArray(id);
		}
		
		void draw(GLuint type = GL_TRIANGLES) {
			if (mObj->mGeom) {
				bind();
	
				if(mObj->mGeom.isIndexed()){
					glDrawElements(type, mObj->mNum, GL_UNSIGNED_INT, 0);
				}
				else {
					glDrawArrays(type, 0, mObj->mNum);
				}

				CXGLERROR
				unbind();
			}
		}
		
	};
	
	
	template <>
	inline S9GLObject<GeometryFullFloat>::S9GLObject(GeometryFullFloat g, Primitive p){
		mObj.reset(new SharedObj());
		mObj->mGeom = g;
		mObj->mPrimitive = p; 
		
		// Generate VBO and VAO and link VAO
		_genOne();
		
		glBindBuffer(GL_ARRAY_BUFFER, mObj->vVBO.back() );
		glBufferData(GL_ARRAY_BUFFER, g.size() * sizeof(VertPNCTF), g.addr(), GL_STATIC_DRAW);
		
		// Generate the Indices if we have them
		if (g.isIndexed())
			_genIndices(g); // On the stack and by copy?
			
		
	
		// Now bind and set the pointers
		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, mObj->vVBO[0]);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertPNCTF),(GLubyte*)(0) );
	/*	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(VertPNCTF),(GLubyte*)(sizeof(float_t) * 3) );
		glVertexAttribPointer(2,4,GL_FLOAT,GL_FALSE,sizeof(VertPNCTF),(GLubyte*)(sizeof(float_t) * 6) );
		glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,sizeof(VertPNCTF),(GLubyte*)(sizeof(float_t) * 10));*/
		
		mObj->mNum = g.size() / 3;
				
		std::cout << mObj->mNum << std::endl;
		
		if (g.isIndexed()){
			glEnableVertexAttribArray(mObj->vVBO.size()-1);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mObj->vVBO.back());
			glVertexAttribPointer(0, 1,GL_UNSIGNED_INT,GL_FALSE,0, (GLubyte*) NULL);
			mObj->mNum = g.indexsize();
		}
	
		glBindVertexArray(0);

	///\todo - Not sure why the below is needed but it stops things breaking

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		for (int id = 0; id < mObj->vVBO.size(); ++id)
			glDisableVertexAttribArray(id);
			

	}
	
	
}

#endif
