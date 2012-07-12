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
	
	// Initial state of the world
	mPos = glm::vec3(0,0,0);
	mLook = glm::vec3(0,0,-1);
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
 
void Primitive::draw(GLuint type) {
	mObj->mVBO.bind();
	
	if (mObj->mVBO.mUsed & VBO_IDCE && mObj->mVBO.mNumIndices > 0)
		glDrawElements(type, mObj->mVBO.mNumIndices, GL_UNSIGNED_INT, 0);
	else if (mObj->mVBO.mNumElements > 0)
		glDrawArrays(type, 0, mObj->mVBO.mNumElements);
	
	CXGLERROR
	mObj->mVBO.unbind();
	
	BOOST_FOREACH ( PrimPtr p, mObj->vChildren) {
		p->draw();
	}
}

/*
 * Get Matrix - a recursive call all the way up the hierarchy
 */
 
glm::mat4 Primitive::getMatrix() {
	glm::mat4 m = getLocalMatrix();
	if (getParent())
		m = _getMatrix(getParent(),m);
	return m;
}

glm::mat4 Primitive::_getMatrix(PrimPtr p, glm::mat4 m) {
	m = p->getLocalMatrix() * m;
	if (p->getParent()){
		_getMatrix(p->getParent(),m);
	}
	return m;
}
 

/*
 * Assimp Related Functions - use of this-> as this class is a decorator 
 */
 
const struct aiScene* AssetGenerator::pScene;

	
void AssetGenerator::recursiveCreate (const struct aiScene *sc, const struct aiNode* nd, PrimPtr p) {

	// draw all meshes assigned to this node - assuming triangles
	for (size_t n = 0; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = pScene->mMeshes[nd->mMeshes[n]];

		// Allocate vertices
		for (size_t k = 0; k < mesh->mNumVertices; k++){
			p->getVBO().vVertices.push_back(0.0f);
			p->getVBO().vVertices.push_back(0.0f);
			p->getVBO().vVertices.push_back(0.0f);
			
			p->getVBO().vNormals.push_back(0.0f);
			p->getVBO().vNormals.push_back(0.0f);
			p->getVBO().vNormals.push_back(0.0f);
		}
	

		for (size_t t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			
			if ( face->mNumIndices == 3) {
				for(size_t i = 0; i < face->mNumIndices; ++i) {
					int index = face->mIndices[i];
					p->getVBO().vIndices.push_back(index);
					
				//	if(mesh->mColors[0] != NULL)
				//		glColor4fv((GLfloat*)&mesh->mColors[0][index]);
					
					if(mesh->mNormals != NULL) {
						p->getVBO().vNormals[index * 3 ] = mesh->mNormals[index].x;
						p->getVBO().vNormals[index * 3 + 1] =  mesh->mNormals[index].y;
						p->getVBO().vNormals[index * 3 + 2] =  mesh->mNormals[index].z;
					}
					
					p->getVBO().vVertices[index * 3 ] =  mesh->mVertices[index].x;
					p->getVBO().vVertices[index * 3 + 1] =  mesh->mVertices[index].y;
					p->getVBO().vVertices[index * 3 + 2] =  mesh->mVertices[index].z;	

				}
			}
		}
	}
	
	
	p->getVBO().mNumElements = p->getVBO().vVertices.size() / 3;
	p->getVBO().mNumIndices = p->getVBO().vIndices.size();
	p->getVBO().compile(VBO_IDCE | VBO_VERT | VBO_NORM);
	CXGLERROR

	for (size_t n = 0; n < nd->mNumChildren; ++n) {
		PrimPtr pp(new Primitive());
		pp->make();
		p->addChild(pp);
		recursiveCreate(sc, nd->mChildren[n], pp);
	}
}


void AssetGenerator::loadAsset(std::string filename, Primitive &p){

	p.make();
	
	pScene = aiImportFile(filename.c_str(),aiProcessPreset_TargetRealtime_MaxQuality);
	if (pScene) {
	/*	get_bounding_box(&scene_min,&scene_max);
		scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		scene_center.z = (scene_min.z + scene_max.z) / 2.0f;*/
		
		PrimPtr pp = boost::make_shared<Primitive>(p);
		recursiveCreate(pScene, pScene->mRootNode,pp);
	
#ifdef DEBUG
		cout << "S9Gear - " << filename << " loaded with " <<  pp->getVBO().mNumIndices / 3 << " faces." << endl;
#endif
	
	}else
		cout << "S9Gear - Failed to load asset: " << filename << endl;
}

AssetGenerator::~AssetGenerator() {
	aiReleaseImport(pScene);
}
