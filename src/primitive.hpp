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
		virtual void make();
		
		virtual operator int() const { return mObj.use_count() > 0; };

		virtual ~Primitive(); 
		
		void move(glm::vec3 p) { mObj->mPos += p; mObj->mLook+=p; compute(); };
		void rotate(glm::vec3 r);
		glm::mat4 getMatrix() { return mObj->mTransMatrix * mObj->mRotMatrix * mObj->mScaleMatrix; };
		
		void setLook(glm::vec3 v) {mObj->mLook = v; compute(); };
		void setPos(glm::vec3 v) {mObj->mPos = v; compute(); };
		void setScale(glm::vec3 v) {mObj->mScale = v; compute(); };
		
		int addChild(PrimPtr p) { mObj->vChildren.push_back(p); return mObj->vChildren.size()-1; };
		
		void compute();
		void draw();
	
		glm::vec3 getPos() { return mObj->mPos;};
		glm::vec3 getLook() { return mObj->mLook;}
		glm::vec3 getScale() { return mObj->mScale;}
		glm::vec3 getUp() { return mObj->mUp;}
				
		void bind() { mObj->mVBO.bind(); };
		void unbind() {mObj->mVBO.unbind(); };
		GLuint getNumElements() { return mObj->mVBO.mNumElements; };
		
		VBOData& getVBO() {return mObj->mVBO;};
		
	protected:
		
		class SharedObj {
		public:
			SharedObj();
			
			std::vector<PrimPtr> vChildren;
			VBOData mVBO;
			glm::vec3 mPos;
			glm::vec3 mUp;
			glm::vec3 mLook;
			glm::vec3 mScale;
		
			glm::mat4 mTransMatrix;
			glm::mat4 mRotMatrix;
			glm::mat4 mScaleMatrix;
		};
		
	
		boost::shared_ptr<SharedObj> mObj; 

	};
	


	/*
	 * Creates a primtive from an assimp call
	 */
	 
	template <class T>
	class AssetDecorator{
	public:
		void recursiveCreate (const struct aiScene *sc, const struct aiNode* nd) {};
		int loadAsset(std::string filename) {};
	};
	 
	template<>
	class AssetDecorator<Primitive> : public Primitive{  
	public:
		~AssetDecorator();
		int loadAsset(std::string filename);

	protected:
		
		void recursiveCreate (const struct aiScene *sc, const struct aiNode* nd);
	
		const struct aiScene* pScene;
	
	};
	
	
	typedef AssetDecorator<Primitive> AssetPrimitive;
	
	
}

#endif
