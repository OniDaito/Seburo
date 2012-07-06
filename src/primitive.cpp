/**
* @brief Primitive Classes
* @file primitive.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/

#include "primitive.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign; 
using namespace s9;

Primitive::Primitive() {
	mPos = glm::vec3(0,0,0);
	mLook = glm::vec3(0,0,1);
	mUp = glm::vec3(0,1,0);
	
	mRotMatrix = glm::mat4(1.0f);
	mTransMatrix = glm::mat4(1.0f);
	mScaleMatrix = glm::mat4(1.0f);
}

Primitive::~Primitive() {}


void Primitive::rotate(glm::vec3 r){
	glm::quat q_rotate;
	
	q_rotate = glm::rotate( q_rotate, r.x, glm::vec3( 1, 0, 0 ) );
	q_rotate = glm::rotate( q_rotate, r.y, glm::vec3( 0, 1, 0 ) );
	q_rotate = glm::rotate( q_rotate, r.z, glm::vec3( 0, 0, 1 ) );

	mLook = q_rotate * mLook;
	mUp = q_rotate * mUp;

	///\todo the maths here may not be correct
	mRotMatrix = glm::axisAngleMatrix(mLook, glm::dot(glm::vec3(0,1,0), mUp) );
}


/*
 * Recompute the matrices on the primitive
 */
 
void Primitive::compute() {

	mTransMatrix = glm::translate(glm::mat4(1.0f), mPos);
	mScaleMatrix = glm::scale(glm::mat4(1.0f), mScale);
}


/*
 * Draw - need to specify whether to draw the children recursively as well - Probably yes
 */
 
void Primitive::draw() {
	mVBO.bind();
	
	if (mVBO.mUsed & VBO_IDCE)
		glDrawElements(GL_TRIANGLES, mVBO.mNumIndices, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, mVBO.mNumElements);
	
	mVBO.unbind();

}
 

/*
 * Assimp Related Functions
 */

void AssetPrimitive::recursiveCreate (const struct aiScene *sc, const struct aiNode* nd) {
	unsigned int i;
	unsigned int n = 0, t;

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = pScene->mMeshes[nd->mMeshes[n]];

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			
			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				
			//	if(mesh->mColors[0] != NULL)
			//		glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				
				if(mesh->mNormals != NULL) {
					mVBO.mNormals.push_back(mesh->mNormals[index].x);
					mVBO.mNormals.push_back(mesh->mNormals[index].y);
					mVBO.mNormals.push_back(mesh->mNormals[index].z);
				}
						
				mVBO.mVertices.push_back(mesh->mVertices[index].x);
				mVBO.mVertices.push_back(mesh->mVertices[index].y);
				mVBO.mVertices.push_back(mesh->mVertices[index].z);
			}
		
		}
		
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		recursiveCreate(sc, nd->mChildren[n]);
	}

}

int AssetPrimitive::loadAsset (std::string filename){

	pScene = aiImportFile(filename.c_str(),aiProcessPreset_TargetRealtime_MaxQuality);
	if (pScene) {
	/*	get_bounding_box(&scene_min,&scene_max);
		scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		scene_center.z = (scene_min.z + scene_max.z) / 2.0f;*/
		recursiveCreate(pScene, pScene->mRootNode);
		
		mVBO.mNumElements = mVBO.mVertices.size() / 3;
		mVBO.compile(VBO_VERT | VBO_NORM);
		CXGLERROR
		
		return 0;
	
	}
	return 1;
}

AssetPrimitive::~AssetPrimitive() {
	aiReleaseImport(pScene);
}
