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


Primitive::SharedObj::SharedObj() {
		
	mPos = glm::vec3(0,0,0);
	mLook = glm::vec3(0,0,1);
	mUp = glm::vec3(0,1,0);
	
	mRotMatrix = glm::mat4(1.0f);
	mTransMatrix = glm::mat4(1.0f);
	mScaleMatrix = glm::mat4(1.0f);
}

Primitive::Primitive() {

}


void Primitive::make() {
	mObj = boost::shared_ptr<SharedObj>(new SharedObj());
}

Primitive::~Primitive() {}


void Primitive::rotate(glm::vec3 r){
	glm::quat q_rotate;
	
	q_rotate = glm::rotate( q_rotate, r.x, glm::vec3( 1, 0, 0 ) );
	q_rotate = glm::rotate( q_rotate, r.y, glm::vec3( 0, 1, 0 ) );
	q_rotate = glm::rotate( q_rotate, r.z, glm::vec3( 0, 0, 1 ) );

	mObj->mLook = q_rotate * mObj->mLook;
	mObj->mUp = q_rotate * mObj->mUp;

	///\todo the maths here may not be correct
	mObj->mRotMatrix = glm::axisAngleMatrix(mObj->mLook, glm::dot(glm::vec3(0,1,0), mObj->mUp) );
}


/*
 * Recompute the matrices on the primitive
 */
 
void Primitive::compute() {

	mObj->mTransMatrix = glm::translate(glm::mat4(1.0f), mObj->mPos);
	mObj->mScaleMatrix = glm::scale(glm::mat4(1.0f), mObj->mScale);
}


/*
 * Draw - need to specify whether to draw the children recursively as well - Probably yes
 */
 
void Primitive::draw() {
	mObj->mVBO.bind();
	
	if (mObj->mVBO.mUsed & VBO_IDCE)
		glDrawElements(GL_TRIANGLES, mObj->mVBO.mNumIndices, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, mObj->mVBO.mNumElements);
	
	CXGLERROR
	mObj->mVBO.unbind();

}
 

/*
 * Assimp Related Functions - use of this-> as this class is a decorator 
 */

void AssetDecorator<Primitive>::recursiveCreate (const struct aiScene *sc, const struct aiNode* nd) {
	unsigned int i;
	unsigned int n = 0, t;

	// draw all meshes assigned to this node - assuming triangles
	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = pScene->mMeshes[nd->mMeshes[n]];

		// Allocate vertices
		for (size_t k = 0; k < mesh->mNumVertices; k++){
			mObj->mVBO.mVertices.push_back(0.0f);
			mObj->mVBO.mVertices.push_back(0.0f);
			mObj->mVBO.mVertices.push_back(0.0f);
			
			mObj->mVBO.mNormals.push_back(0.0f);
			mObj->mVBO.mNormals.push_back(0.0f);
			mObj->mVBO.mNormals.push_back(0.0f);
		}
	

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			
			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				mObj->mVBO.mIndices.push_back(index);
				
			//	if(mesh->mColors[0] != NULL)
			//		glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				
				if(mesh->mNormals != NULL) {
					mObj->mVBO.mNormals[index * 3 ] =  mesh->mNormals[index].x;
					mObj->mVBO.mNormals[index * 3 + 1] =  mesh->mNormals[index].y;
					mObj->mVBO.mNormals[index * 3 + 2] =  mesh->mNormals[index].z;
				}
				
				mObj->mVBO.mVertices[index * 3 ] =  mesh->mVertices[index].x;
				mObj->mVBO.mVertices[index * 3 + 1] =  mesh->mVertices[index].y;
				mObj->mVBO.mVertices[index * 3 + 2] =  mesh->mVertices[index].z;	

			}
		}
	}




	///\todo recursive create should create child primitives
/*
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursiveCreate(sc, nd->mChildren[n]);
	}*/
}


int AssetDecorator<Primitive>::loadAsset(std::string filename){
	make();
		
	pScene = aiImportFile(filename.c_str(),aiProcessPreset_TargetRealtime_MaxQuality);
	if (pScene) {
	/*	get_bounding_box(&scene_min,&scene_max);
		scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		scene_center.z = (scene_min.z + scene_max.z) / 2.0f;*/
		recursiveCreate(pScene, pScene->mRootNode);
		
		mObj->mVBO.mNumElements = mObj->mVBO.mVertices.size() / 3;
		mObj->mVBO.mNumIndices = mObj->mVBO.mIndices.size();
		mObj->mVBO.compile(VBO_IDCE | VBO_VERT | VBO_NORM);
		CXGLERROR
	
#ifdef DEBUG
		cout << "S9Gear - " << filename << " loaded with " << mObj->mVBO.mNumIndices / 3 << " faces." << endl;
#endif
		
		return 0;
	}
	cout << "S9Gear - Failed to load asset: " << filename << endl;
	return 1;
}

AssetPrimitive::~AssetPrimitive() {
	aiReleaseImport(pScene);
}
