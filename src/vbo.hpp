/**
* @brief VBO Classes
* @file vbo.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/


#ifndef VBO_HPP
#define VBO_HPP

#include "common.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <boost/assign/list_inserter.hpp>

namespace s9 {
	
	
	/*
	 * Base Buffer - pure interface
	 */
	
	class VBOBaseBuffer  {
	public:		
		virtual void attach(GLuint pos) = 0;
		virtual void allocate()  = 0;
		virtual size_t size()  = 0;
		virtual void setID(GLuint id)  = 0;
		virtual GLuint getID()  = 0;
	};
	
	
	/*
	 * buffer types
	 */
	 
	template <class T>
	class VBOBuffer : public VBOBaseBuffer {
	public:
		inline VBOBuffer<T>& operator ,(const T &a){ push_back(a); return *this; }
		
		inline VBOBuffer<T>& operator+=( T a ){
			push_back(a);
			return *this;
		}
		
	
	protected: 
		
		struct SharedObj {
			size_t mW;
			GLuint mID;
			std::vector<T> vBuffer;
		};
		
		boost::shared_ptr<SharedObj> mObj;
		
	public:
		VBOBuffer (size_t width) {
			mObj.reset(new SharedObj());
			mObj->mW = width;
		}

		inline void push_back (T a) { mObj->vBuffer.push_back(a); };
			
		inline T operator[](size_t i) { return mObj->vBuffer[i]; };

		inline void set (size_t i, T a) { mObj->vBuffer[i] = a; };

		inline void setID(GLuint id) {
			mObj->mID = id;
		};

		// Annoyingly, I cant just pass the type here in as an argument to glVertexAttribPointer
		inline void attach(GLuint pos) {
			glBindBuffer(GL_ARRAY_BUFFER, mObj->mID);
			glVertexAttribPointer(pos, mObj->mW, GL_FLOAT, GL_FALSE,0, (GLubyte*) NULL);
		};

		inline GLuint getID() {
			return mObj->mID;
		};

		inline size_t size() {
			return mObj->vBuffer.size();
		};

		inline void allocate() {
			glBindBuffer(GL_ARRAY_BUFFER, mObj->mID );
			glBufferData(GL_ARRAY_BUFFER, mObj->vBuffer.size() * sizeof(T), &(mObj->vBuffer[0]), GL_DYNAMIC_DRAW);
		}
				
	};


	template<> inline void  VBOBuffer<GLfloat>::attach(GLuint pos) {
		glBindBuffer(GL_ARRAY_BUFFER, mObj->mID);
		glVertexAttribPointer(pos, mObj->mW, GL_FLOAT, GL_FALSE,0, (GLubyte*) NULL);
	}

	template<> inline void VBOBuffer<GLuint>::attach(GLuint pos) {
		glBindBuffer(GL_ARRAY_BUFFER, mObj->mID);
		glVertexAttribPointer(pos, mObj->mW, GL_UNSIGNED_INT, GL_FALSE,0, (GLubyte*) NULL);
	}

	template<> inline void VBOBuffer<GLubyte>::attach(GLuint pos) {
		glBindBuffer(GL_ARRAY_BUFFER, mObj->mID);
		glVertexAttribPointer(pos, mObj->mW, GL_UNSIGNED_BYTE, GL_FALSE,0, (GLubyte*) NULL);
	}

	template<> inline void VBOBuffer<GLint>::attach(GLuint pos) {
		glBindBuffer(GL_ARRAY_BUFFER, mObj->mID);
		glVertexAttribPointer(pos, mObj->mW, GL_INT, GL_FALSE,0, (GLubyte*) NULL);
	}

	template<> inline void VBOBuffer<GLdouble>::attach(GLuint pos) {
		glBindBuffer(GL_ARRAY_BUFFER, mObj->mID);
		glVertexAttribPointer(pos, mObj->mW, GL_DOUBLE, GL_FALSE,0, (GLubyte*) NULL);
	}
		



	/*
	 * VBO Data class to hold our primitives
	 */

	class VBOData {
	public:
		
		VBOData() {};
	
		~VBOData();
		
		void bind();
		void unbind();
		void compile();
		
		size_t getNumIndices();
		size_t getNumElements();
		size_t getNumBuffers();
		

		/*
		 * Push back function for adding buffers
		 */
		template <class U> 
		inline void push_back( VBOBuffer<U> a) {
			if(mObj == boost::shared_ptr<SharedObj>()){
				mObj.reset(new SharedObj());
				glGenVertexArrays(1,&(mObj->mVAO));
			}
			
			boost::shared_ptr< VBOBuffer<U> > pp = boost::make_shared< VBOBuffer<U> >(a);
			mObj->vBuffers.push_back(pp);
			
			mObj->vVBO.push_back(0);
			size_t p = mObj->vVBO.size()-1;
			glGenBuffers(1, &(mObj->vVBO[p]) );
			a.setID(mObj->vVBO[p]);
			
			glBindVertexArray(mObj->mVAO);
			a.allocate();
			a.attach(p);
			
			mObj->mNumBufs =  mObj->vBuffers.size();
			mObj->mNumIndices = 0;
			
			glBindVertexArray(0);
			
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			
			for (int id = 0; id < mObj->mNumBufs; ++id){
				glDisableVertexAttribArray(id);
			}
		}
		
		template <class U> 
		inline VBOBuffer<U> getBuffer(size_t pos) {
			return static_cast< VBOBuffer<U> >  ( *(mObj->vBuffers[pos]) ); 
		}
		
	
		struct SharedObj {
			size_t mNumBufs;		
			GLuint mNumElements;				// May be smaller than the size of the arrays due to buffering
			GLuint mNumIndices;					// Different to the above remmeber
		
			GLuint mVAO;
			std::vector<GLuint> vVBO;
			std::vector<boost::shared_ptr<VBOBaseBuffer> > vBuffers;
		};
		
		boost::shared_ptr<SharedObj> mObj;
		
	}; 
}

#endif

