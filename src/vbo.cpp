/**
* @brief VBO Basic Class
* @file vbo.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/

#include "vbo.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign; 

/*
 * VBO Buffer
 */

namespace s9 {


/*
 * VBO and VAO wrapper
 */

void VBOData::compile(){
	
/*	mObj->vVBO = new GLuint[mObj->vBuffers.size()];
	glGenBuffers(mObj->vBuffers.size(),mObj->vVBO);
	
	for (size_t i = 0; i < mObj->vBuffers.size(); ++i)
		mObj->vBuffers[i]->setID(mObj->vVBO[i]);
	
	glGenVertexArrays(1,&(mObj->mVAO));
	glBindVertexArray(mObj->mVAO);
		
	for (size_t i = 0; i < mObj->vBuffers.size(); ++i)
		mObj->vBuffers[i]->allocate();
	
	for (size_t i = 0; i < mObj->vBuffers.size(); ++i)
		mObj->vBuffers[i]->attach(i);
	
	mObj->mNumBufs =  mObj->vBuffers.size();
	mObj->mNumIndices = 0;
	
	// Default behaviour - set the first buffer to num elements, second to num indicies
	///\todo find a better way
	
	mObj->mNumElements =  mObj->vBuffers[0]->size() / 3;
	if (mObj->mNumBufs > 1)
		mObj->mNumIndices =  mObj->vBuffers[1]->size();
		
	glBindVertexArray(0);
	
	///\todo - Not sure why the below is needed but it stops things breaking
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	for (int id = 0; id < mObj->mNumBufs; ++id){
		glDisableVertexAttribArray(id);
	}*/
}


		
size_t VBOData::getNumIndices() {
	return mObj->mNumIndices;
}

size_t VBOData::getNumElements(){
	return mObj->mNumElements;
}

size_t VBOData::getNumBuffers(){
	return mObj->mNumBufs;
}
		

		
VBOData::~VBOData() {
	glDeleteBuffers(mObj->mNumBufs, &(mObj->vVBO[0]));
    glDeleteVertexArrays(1, &(mObj->mVAO));
}
		
		
/*
 * Bind this VBO/VAO and set the shader attribs
 * Using the layout directive in shaders to map attribs to numbers
 */

void VBOData::bind() {
	glBindVertexArray(mObj->mVAO);
}

void VBOData::unbind() {
	glBindVertexArray(0);
	
	for (int id = 0; id < mObj->mNumBufs; ++ id){
		glDisableVertexAttribArray(id);
	}
}

}


