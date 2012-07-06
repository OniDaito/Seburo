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
 */
 
namespace s9 {

	class Primitive;
	
	typedef boost::shared_ptr<Primitive> PrimPtr;
	 
	 
	class Primitive {
	public:
		Primitive();

		virtual ~Primitive(); 
		
		void move(glm::vec3 p) {mPos += p; mLook+=p; compute(); };
		void rotate(glm::vec3 r);
		glm::mat4 getMatrix() { return mTransMatrix * mRotMatrix * mScaleMatrix; };
		
		void setLook(glm::vec3 v) {mLook = v; compute(); };
		void setPos(glm::vec3 v) {mPos = v; compute(); };
		void setScale(glm::vec3 v) {mScale = v; compute(); };
		
		int addChild(PrimPtr p) { vChildren.push_back(p); return vChildren.size()-1; };
		
		void compute();
		void draw();
	
		glm::vec3 getPos() { return mPos;};
		glm::vec3 getLook() { return mLook;}
		glm::vec3 getScale() { return mScale;}
		glm::vec3 getUp() { return mUp;}
				
		void bind() { mVBO.bind(); };
		void unbind() {mVBO.unbind(); };
		GLuint getNumElements() { return mVBO.mNumElements; };
		
		VBOData& getVBO() {return mVBO;};
		
	protected:
		
		VBOData mVBO;
		
		std::vector<PrimPtr> vChildren;
		
		glm::vec3 mPos;
		glm::vec3 mUp;
		glm::vec3 mLook;
		glm::vec3 mScale;
		
		glm::mat4 mTransMatrix;
		glm::mat4 mRotMatrix;
		glm::mat4 mScaleMatrix;

	};
	
	/*
	 * A Primitive with a model loader - assimp library
	 */

	class AssetPrimitive : public Primitive{
	public:
		~AssetPrimitive();
		int loadAsset(std::string filename);
			
	protected:
		void recursiveCreate (const struct aiScene *sc, const struct aiNode* nd);
		const struct aiScene* pScene;
	};
}

#endif
