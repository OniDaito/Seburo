
/*
 * Assimp Related Functions - use of this-> as this class is a decorator 
 */
 
const struct aiScene* AssetGenerator::pScene;

	
void AssetGenerator::recursiveCreate (const struct aiScene *sc, const struct aiNode* nd, PrimPtr p) {

	VBOBuffer<GLfloat> verts(3);
	VBOBuffer<GLuint> indices(1);
	VBOBuffer<GLfloat> normals(3);

	// draw all meshes assigned to this node - assuming triangles
	for (size_t n = 0; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = pScene->mMeshes[nd->mMeshes[n]];

		// Allocate vertices
		for (size_t k = 0; k < mesh->mNumVertices; k++){
			verts.push_back(0.0f);
			verts.push_back(0.0f);
			verts.push_back(0.0f);
			
			normals.push_back(0.0f);
			normals.push_back(0.0f);
			normals.push_back(0.0f);
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
						normals.set(index * 3,  mesh->mNormals[index].x);
						normals.set(index * 3 + 1,  mesh->mNormals[index].y);
						normals.set(index * 3 + 2,  mesh->mNormals[index].z);
					}
					
					verts.set(index * 3,  mesh->mVertices[index].x);
					verts.set(index * 3 + 1,  mesh->mVertices[index].y);
					verts.set(index * 3 + 2,  mesh->mVertices[index].z);
				}
			}
		}
	}
	
	
	VBOData v;
	v.push_back(verts);
	v.push_back(indices,true);
	v.push_back(normals);
	v.setNumIndices(indices.size());
	p->setVBO(v);
	
	CXGLERROR

	for (size_t n = 0; n < nd->mNumChildren; ++n) {
		PrimPtr pp(new Primitive());
		pp->make();
		p->addChild(pp);
		recursiveCreate(sc, nd->mChildren[n], pp);
	}
}


Primitive AssetGenerator::loadAsset(std::string filename){
	Primitive p;
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
		cout << "S9Gear - " << filename << " loaded with " <<  pp->getVBO().getNumIndices() / 3 << " faces." << endl;
#endif
	
	}else
		cout << "S9Gear - Failed to load asset: " << filename << endl;
	
	return p;
}

AssetGenerator::~AssetGenerator() {
	aiReleaseImport(pScene);
}
