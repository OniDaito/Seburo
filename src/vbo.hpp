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
		virtual void attach(GLint pos, bool indices=false) = 0;
		virtual void allocate(bool indices=false)  = 0;
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
		
		
		
	public:
		boost::shared_ptr<SharedObj> mObj;
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
		inline void attach(GLint pos, bool indices=false) {
			glEnableVertexAttribArray(pos);
			if (indices) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mObj->mID);
				glVertexAttribPointer(pos,mObj->mW,GL_UNSIGNED_INT,GL_FALSE,0, (GLubyte*) NULL);
			} else {
				glBindBuffer(GL_ARRAY_BUFFER, mObj->mID);
				glVertexAttribPointer(pos, mObj->mW, GL_FLOAT, GL_FALSE,0, (GLubyte*) NULL);
			}
		
		};

		inline GLuint getID() {
			return mObj->mID;
		};

		inline size_t size() {
			return mObj->vBuffer.size();
		};

		inline void allocate(bool indices=false) {
			if (indices){
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mObj->mID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, mObj->vBuffer.size() * sizeof(T), &(mObj->vBuffer[0]), GL_DYNAMIC_DRAW);
			}
			else {
				glBindBuffer(GL_ARRAY_BUFFER, mObj->mID );
				glBufferData(GL_ARRAY_BUFFER, mObj->vBuffer.size() * sizeof(T), &(mObj->vBuffer[0]), GL_DYNAMIC_DRAW);
			}
		}		
	};


	template<> inline void  VBOBuffer<GLfloat>::attach(GLint pos, bool indices) {
		glEnableVertexAttribArray(pos);
		if (indices)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mObj->mID);
		else
			glBindBuffer(GL_ARRAY_BUFFER, mObj->mID);
		glVertexAttribPointer(pos, mObj->mW, GL_FLOAT, GL_FALSE,0, (GLubyte*) NULL);
	}

	template<> inline void VBOBuffer<GLuint>::attach(GLint pos, bool indices) {
		glEnableVertexAttribArray(pos);
		if (indices)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mObj->mID);
		else
			glBindBuffer(GL_ARRAY_BUFFER, mObj->mID);
		glVertexAttribIPointer(pos, mObj->mW, GL_UNSIGNED_INT, 0, (GLubyte*) NULL);
	}

	template<> inline void VBOBuffer<GLubyte>::attach(GLint pos, bool indices) {
		glEnableVertexAttribArray(pos);
		if (indices)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mObj->mID);
		else
			glBindBuffer(GL_ARRAY_BUFFER, mObj->mID);
		glVertexAttribIPointer(pos, mObj->mW, GL_UNSIGNED_BYTE, 0, (GLubyte*) NULL);
	}

	template<> inline void VBOBuffer<GLint>::attach(GLint pos, bool indices) {
		glEnableVertexAttribArray(pos);
		if (indices)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mObj->mID);
		else
			glBindBuffer(GL_ARRAY_BUFFER, mObj->mID);
		glVertexAttribIPointer(pos, mObj->mW, GL_INT, 0, (GLubyte*) NULL);
	}

	template<> inline void VBOBuffer<GLdouble>::attach(GLint pos, bool indices) {
		glEnableVertexAttribArray(pos);
		if (indices)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mObj->mID);
		else
			glBindBuffer(GL_ARRAY_BUFFER, mObj->mID);
		glVertexAttribPointer(pos, mObj->mW, GL_DOUBLE, GL_FALSE,0, (GLubyte*) NULL);
	}
		



	/*
	 * VBO Data class to hold our primitives
	 */

	class VBOData {
	public:
		
		VBOData() {};
	
		~VBOData() {};
		
		
		inline void setNumIndices(size_t x) {mObj->mNumIndices = x; };
		inline void setNumElements(size_t x) {mObj->mNumElements = x; };
		
		inline size_t getNumIndices() { return mObj->mNumIndices; };

		inline size_t getNumElements(){ return mObj->mNumElements; };

		inline size_t getNumBuffers(){ return mObj->mNumBufs; };
	
		inline void bind() { glBindVertexArray(mObj->mVAO); };

		inline void unbind() {
			glBindVertexArray(0);
			for (int id = 0; id < mObj->mNumBufs; ++ id){
				glDisableVertexAttribArray(id);
			}
		}

		/*
		 * Push back function for adding buffers
		 * Apparently we should have a bind/unbind here with respect to the VAO but that sucks
		 */
		template <class U> 
		inline void push_back( VBOBuffer<U> a, bool indices=false) {
			if(mObj == boost::shared_ptr<SharedObj>()){
				mObj.reset(new SharedObj());
				glGenVertexArrays(1,&(mObj->mVAO));
			}
			
			boost::shared_ptr< VBOBuffer<U> > pp = boost::make_shared< VBOBuffer<U> >(a);
			mObj->vBuffers.push_back(pp);
			
			mObj->vVBO.push_back(0);
			GLint p = mObj->vVBO.size()-1;
			glGenBuffers(1, &(mObj->vVBO[p]) );
			a.setID(mObj->vVBO[p]);
					
			glBindVertexArray(mObj->mVAO);
			a.allocate(indices);
			a.attach(p,indices);
			
			mObj->mNumBufs =  mObj->vBuffers.size();
			if (indices)
				mObj->mNumIndices = 0;
			else
				mObj->mNumIndices = a.size();
			
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
			boost::shared_ptr<VBOBuffer<U> > p = boost::static_pointer_cast< VBOBuffer<U>  >(mObj->vBuffers[pos]); 
			return *p;
		}
		
	
		class SharedObj {
		public:
			~SharedObj() {	glDeleteBuffers(mNumBufs, &(vVBO[0])); glDeleteVertexArrays(1, &(mVAO)); }
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

