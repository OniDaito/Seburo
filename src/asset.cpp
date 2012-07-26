/**
* @brief Loading a model or Asset with Assimp
* @file asset.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 26/07/2012
*
*/

#include "asset.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign;
using namespace s9;



const struct aiScene* AssetImporter::pScene;

/*
 * Recursive load function. Dependent on the actual type of the geom
 */ 

AssetPtr AssetImporter::_load (const struct aiScene *sc, const struct aiNode* nd, AssetPtr p ) {

	vector<float_t> verts;
	vector<float_t> norms;
	vector<uint32_t> indices;


	// draw all meshes assigned to this node - assuming triangles
	for (size_t n = 0; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = pScene->mMeshes[nd->mMeshes[n]];

		// Allocate vertices
	

		for (size_t k = 0; k < mesh->mNumVertices; k++){

			verts.push_back(0.0f);
			verts.push_back(0.0f);
			verts.push_back(0.0f);
			
			norms.push_back(0.0f);
			norms.push_back(0.0f);
			norms.push_back(0.0f);
		}
	

		for (size_t t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			
			if ( face->mNumIndices == 3) {
				for(size_t i = 0; i < face->mNumIndices; ++i) {
					int index = face->mIndices[i];
					indices.push_back(index);
					
				//	if(mesh->mColors[0] != NULL)
				//		glColor4fv((GLfloat*)&mesh->mColors[0][index]);
					
					if(mesh->mNormals != NULL) {
						norms[index * 3 ] = mesh->mNormals[index].x;
						norms[index * 3 + 1] =  mesh->mNormals[index].y;
						norms[index * 3 + 2] =  mesh->mNormals[index].z;
					}
					
					verts[index * 3 ] =  mesh->mVertices[index].x;
					verts[index * 3 + 1] =  mesh->mVertices[index].y;
					verts[index * 3 + 2] =  mesh->mVertices[index].z;	

				}
			}
		}
	}

	GeometryPNF g (verts,norms);
	g.addIndices(indices);

	AssetPtr pp (new AssetBasic(g));

	if (p != AssetPtr())
		p->addChild(pp);

	for (size_t n = 0; n < nd->mNumChildren; ++n) {
		_load(sc, nd->mChildren[n], pp);
	}
	return pp;
}


/*
 * Load an Asset uisng the Assimp methodology for just PNF verts
 */

AssetBasic AssetImporter::load(std::string filename){

	AssetBasic p;

	pScene = aiImportFile(filename.c_str(),aiProcessPreset_TargetRealtime_MaxQuality);
	if (pScene) {
	/*	get_bounding_box(&scene_min,&scene_max);
		scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		scene_center.z = (scene_min.z + scene_max.z) / 2.0f;*/

		p = *(_load(pScene, pScene->mRootNode, AssetPtr()));
	
#ifdef DEBUG
		cout << "S9Gear - " << filename << " loaded with " <<  p.getGeometry().size()  << " vertices." << endl;
#endif
	
	} else
		cout << "S9Gear - Failed to load asset: " << filename << endl;

	return p;
}

AssetImporter::~AssetImporter() {
	aiReleaseImport(pScene);
}