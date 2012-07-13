/**
* @brief Primitive Classes
* @file primitive.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/


#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "common.hpp"
#include "vbo.hpp"

/*
 * Struct for primitive objects with their own co-ordinates
 * \todo add functions for recursive nature of primitives - primtives can contain groups of primitives
 * \todo add static colour function that ensures unique colours
 * \todo copy, copy constructor and == operators
 */
 
namespace s9 {

	class Primitive;
	
	typedef boost::shared_ptr<Primitive> PrimPtr;
		
	class Primitive {
	public:
	
		Primitive();
		Primitive(VBOData v);
		virtual void make();
		
		virtual operator int() const { return mObj.use_count() > 0; };

		virtual ~Primitive(); 
		
		void move(glm::vec3 p) { mObj->mPos += p; compute(); };
		void rotate(glm::vec3 r);
		glm::mat4 getMatrix();
		glm::mat4 getLocalMatrix() { return mObj->mTransMatrix * mObj->mRotMatrix * mObj->mScaleMatrix; };
		
		glm::mat4 getTransMatrix() { return mObj->mTransMatrix; };
		glm::mat4 getRotMatrix() { return mObj->mRotMatrix; };
		glm::mat4 getScaleMatrix() { return mObj->mScaleMatrix; };
		
		void setLook(glm::vec3 v) {mObj->mLook = v; glm::normalize(v); compute(); };
		void setPos(glm::vec3 v) {mObj->mPos = v;compute(); };
		void setScale(glm::vec3 v) {mObj->mScale = v; compute(); };
		void setColour(glm::vec4 v) {mObj->mColour = v; };
		
		int addChild(PrimPtr p) { mObj->vChildren.push_back(p); p->mObj->pParent = PrimPtr(this); return mObj->vChildren.size()-1; };
		void removeChild(PrimPtr p){};
		
		void compute();
		void draw(GLuint type = GL_TRIANGLES);
	
		glm::vec3 getPos() { return mObj->mPos;};
		glm::vec3 getLook() { return mObj->mLook;};
		glm::vec3 getScale() { return mObj->mScale;};
		glm::vec3 getUp() { return mObj->mUp;};
		glm::vec4 getColour() { return mObj->mColour;};
				
		void bind() { mObj->mVBO.bind(); };
		void unbind() {mObj->mVBO.unbind(); };
		GLuint getNumElements() { return mObj->mVBO.getNumElements(); };
		
		PrimPtr getParent(){return mObj->pParent; };
		
		VBOData getVBO() {return mObj->mVBO;};
		void setVBO(VBOData b) {mObj->mVBO = b;};
		
	protected:
	
		glm::mat4 _getMatrix(PrimPtr p, glm::mat4 m);
		
		class SharedObj {
		public:
			SharedObj();
			
			std::vector<PrimPtr> vChildren;
			PrimPtr pParent;
			
			VBOData mVBO;
			glm::vec3 mPos;
			glm::vec3 mUp;
			glm::vec3 mLook;
			glm::vec3 mScale;
			
			glm::vec4 mColour;
		
			glm::mat4 mTransMatrix;
			glm::mat4 mRotMatrix;
			glm::mat4 mScaleMatrix;
		};
		
	
		boost::shared_ptr<SharedObj> mObj; 

	};
	

	/*
	 * static class to generate a primtive using the ASSIMP Library
	 */
	

	class AssetGenerator{
	public:
		~AssetGenerator();
		static Primitive loadAsset(std::string filename);

	protected:
		
		static void recursiveCreate (const struct aiScene *sc, const struct aiNode* nd, PrimPtr p);
		static const struct aiScene* pScene;
	};
	
	
	
}

#endif
