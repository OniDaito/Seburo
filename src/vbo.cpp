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
using namespace s9;


/*
 * VBO Constructor
 */

VBOData::VBOData() {
   
}

/*
 * VBO Destructor
 */

VBOData::~VBOData() {
	glDeleteBuffers(mNumBufs, vbo);
    glDeleteVertexArrays(1, &vao);
}

/*
 * Bind this VBO/VAO and set the shader attribs
 * Using the layout directive in shaders to map attribs to numbers
 */

void VBOData::bind() {
	glBindVertexArray(vao);
}

void VBOData::unbind() {
	glBindVertexArray(0);
	
	for (int id = 0; id < mNumBufs; ++ id){
		glDisableVertexAttribArray(id);
	}
}


void VBOData::compile(size_t buffers) {
	// Find out how many buffers we need
	int s = 0;
	mUsed = buffers;
	
	if (mUsed & VBO_VERT) s++;
	if (mUsed & VBO_IDCE) s++;
	if (mUsed & VBO_COLR) s++;
	if (mUsed & VBO_NORM) s++;
	if (mUsed & VBO_TEXC) s++;
	if (mUsed & VBO_TEXI) s++;
	
	vbo = new GLuint[s];
	glGenBuffers(s,vbo);
	s = 0;
	if (mUsed & VBO_VERT) { mVID = vbo[s]; s++;}
	if (mUsed & VBO_IDCE) { mIID = vbo[s]; s++;}
	if (mUsed & VBO_COLR) { mCID = vbo[s]; s++;}
	if (mUsed & VBO_NORM) { mNID = vbo[s]; s++;}
	if (mUsed & VBO_TEXC) { mTID = vbo[s]; s++;}
	if (mUsed & VBO_TEXI) { mTTD = vbo[s]; s++;}
	
	// Create and bind the VAO
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	
	// Populate buffers
	
	allocateVertices();
	allocateIndices();
	allocateColours();
	allocateNormals();
	allocateTexCoords();
	allocateTexIDs();
	s = 0;
	
	if (mUsed & VBO_VERT){
		glEnableVertexAttribArray(s);
		glBindBuffer(GL_ARRAY_BUFFER, mVID);
		glVertexAttribPointer(s,3,GL_FLOAT,GL_FALSE,0, (GLubyte*) NULL);
		mNumElements = mVertices.size() / 3;
		s++;
	} 
	if (mUsed & VBO_IDCE){
		glEnableVertexAttribArray(s);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIID);
		glVertexAttribPointer(s,1,GL_UNSIGNED_INT,GL_FALSE,0, (GLubyte*) NULL);
		mNumIndices = mIndices.size();
		s++;
	} 
	if (mUsed & VBO_COLR){
		glEnableVertexAttribArray(s);
		glBindBuffer(GL_ARRAY_BUFFER, mCID);
		glVertexAttribPointer(s,4,GL_FLOAT,GL_FALSE,0, (GLubyte*) NULL);
		s++;
	} 
	if (mUsed & VBO_NORM){
		glEnableVertexAttribArray(s);
		glBindBuffer(GL_ARRAY_BUFFER, mNID);
		glVertexAttribPointer(s,3,GL_FLOAT,GL_FALSE,0, (GLubyte*) NULL);
		s++;
	}
	if (mUsed & VBO_TEXC){
		glEnableVertexAttribArray(s);
		glBindBuffer(GL_ARRAY_BUFFER, mTID);
		glVertexAttribPointer(s,2,GL_FLOAT,GL_FALSE,0, (GLubyte*) NULL);
		s++;
	} 
	
	if (mUsed & VBO_TEXI){
		glEnableVertexAttribArray(s);
		glBindBuffer(GL_ARRAY_BUFFER, mTTD);
		///\todo NOTE THE I in the function below!
		glVertexAttribIPointer(s,1,GL_UNSIGNED_INT,0, (GLubyte*) NULL);
		s++;
	} 
	
	mNumBufs = s;

	glBindVertexArray(0);
	
	///\todo - Not sure why the below is needed but it stops things breaking
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	for (int id = 0; id < mNumBufs; ++id){
		glDisableVertexAttribArray(id);
	}
	
}

/*
 * Allocate based on the current size of the arrays
 */

void VBOData::allocateVertices() {
	if (mUsed & VBO_VERT){
		glBindBuffer(GL_ARRAY_BUFFER, mVID);
		glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(GLfloat), &mVertices[0], GL_DYNAMIC_DRAW);
	}
}

void VBOData::allocateIndices() {
	if (mUsed & VBO_IDCE){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices[0], GL_DYNAMIC_DRAW);
	}
}

void VBOData::allocateTexCoords() {
	if (mUsed & VBO_TEXC){
		glBindBuffer(GL_ARRAY_BUFFER, mTID);
		glBufferData(GL_ARRAY_BUFFER, mTexCoords.size()  * sizeof(GLfloat), &mTexCoords[0], GL_DYNAMIC_DRAW);
	}
}

void VBOData::allocateNormals() {
	if (mUsed & VBO_NORM){
		glBindBuffer(GL_ARRAY_BUFFER, mNID);
		glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(GLfloat), &mNormals[0], GL_DYNAMIC_DRAW);
	}
}

void VBOData::allocateColours() {
	if (mUsed & VBO_COLR){
		glBindBuffer(GL_ARRAY_BUFFER, mCID);
		glBufferData(GL_ARRAY_BUFFER, mColours.size() * sizeof(GLfloat), &mColours[0], GL_DYNAMIC_DRAW);
	}
}

void VBOData::allocateTexIDs() {
	if (mUsed & VBO_TEXI){
		glBindBuffer(GL_ARRAY_BUFFER, mTTD);
		glBufferData(GL_ARRAY_BUFFER, vTexIDs.size() * sizeof(GLuint), &vTexIDs[0], GL_DYNAMIC_DRAW);
	}
}
