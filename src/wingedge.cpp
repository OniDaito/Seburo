/**
* @brief Wing Edged Classes
* @file wingedge.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/07/2012
*
*/

#include "s9/wingedge.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign; 
using namespace s9;

using boost::shared_ptr;

/*
 * Given a primtive, create a winged edge structure
 * This assumes that primitives have buffer 0 as vertices and buffer 1 as indices
 */

void WingedEdge::make(DrawableGeometry geom) {
	
	mObj.reset(new SharedObj());
	mObj->mGeom = geom;
	
	// Create a temporary array of vertices for now. 
	std::vector< boost::shared_ptr<WE_Vertex> > vs;

	for (size_t i=0; i < mObj->mGeom.size(); i+=3){
		boost::shared_ptr<WE_Vertex> sv (new WE_Vertex);
		vs.push_back(sv);
		sv->idc = i; // Copy the indices
	}
	
	
	cerr << "Seburo Winged Edge Vertices Count: " << vs.size() << endl;

	// Now create proper winged edge and indices using a concatenated 32 bit index to a 64 bit key
	// In addition we create a straight list of unique triangles
	
	map< uint64_t, boost::shared_ptr<WE_Edge> > es;
	
	size_t bw = 0;

	uint32_t *indices = mObj->mGeom.indexaddr();

	for (size_t i = 0; i < mObj->mGeom.indexsize(); i+=3) {

		boost::shared_ptr<WE_Face>  f (new WE_Face());
		bool bf = false;
		uint32_t idcs[3];



		idcs[0] = indices[i];
		idcs[1] = indices[i+1];
		idcs[2] = indices[i+2]; 

		// Find Edges to add to face
		map< uint64_t, boost::shared_ptr<WE_Edge> >::iterator pi = es.end();
		
		boost::shared_ptr<WE_Edge> prev;
		boost::shared_ptr<WE_Edge> current;
		boost::shared_ptr<WE_Edge> first;
		
		vector<uint64_t> keys;
		
		for (int j = 0; j < 3; j++){
			
			map< uint64_t, boost::shared_ptr<WE_Edge> >::iterator fi;
			
			// create two keys and perform lookup
			int k = j == 2 ? 0 : j + 1;
			uint64_t jk = idcs[j]; jk = jk << 32;
			jk |= idcs[k];
			
			uint64_t kj = idcs[k]; kj = kj << 32;
			kj |= idcs[j];
						
			// new edge	
			boost::shared_ptr<WE_Edge> sp (new WE_Edge);
			sp->face = f;
			f->edge = sp;
							
			// Do we have a symetric edge?
			fi = es.find(kj);
			if (fi != es.end() ){
				sp->sym = fi->second;
				fi->second->sym = sp;
			}
			
			current = sp;
			sp->v0 = vs[idcs[j]];
			sp->v1 = vs[idcs[k]];
			
			if (j == 0){
				prev = current;
				first = current;
			}else {
				prev->next = current;
				current->prev = prev;
				prev = current;
			}
			
			
			// this should never happen as its basically a badly wound polygon - 
			// in this case remove the face and all associated edges
			fi = es.find(jk);
			if (fi != es.end()){
				bf = true;
				bw++;
				break;
			}
			keys.push_back(jk);
			es.insert( pair<uint64_t, boost::shared_ptr<WE_Edge> > (jk, sp) );
			
		}
		
		if (!bf) {
			// Close the loop off
			first->prev = current;
			current->next = first;
	
			
			mObj->mWE.push_back(f);
		}else {
			// Delete any added edges for this badly wound face
			for (size_t i = 0; i < keys.size(); i++){
				map< uint64_t, boost::shared_ptr<WE_Edge> >::iterator pi = es.find(keys[i]);
				es.erase (pi);      
			}
		}
	}
		
	cerr << "Seburo Winged Edge edges count: " << es.size() << endl;
	cerr << "Seburo Winged Edge Face Count: " << mObj->mWE.size() << endl;
	cerr << "Seburo Winged Edge Discarded Face Count: " << bw << endl;
}

/*
 * Generate a primitive from this. Essentially flatten it out
 */
 /*

Primitive WingedEdge::flatten() { 
	
	VBOData v;
	
	VBOBuffer<GLfloat> verts(3);
	VBOBuffer<GLfloat> texs(2);
	VBOBuffer<GLfloat> norms(3);
	VBOBuffer<GLuint> tids(1);
	
	for (size_t i = 0; i < mObj->mWE.size(); ++i) {
	
		shared_ptr<WE_Face> sf = mObj->mWE[i];
		
		size_t i0 = sf->edge->v0->idc;
		size_t i1 = sf->edge->next->v0->idc;
		size_t i2 = sf->edge->next->next->v0->idc;
		
		VBOBuffer<GLfloat> vv = mObj->mPrimitive.getVBO().getBuffer<GLfloat>(0);
		
		glm::vec3 v0 (vv[i0], vv[i0+1], vv[i0+2]);
		glm::vec3 v1 (vv[i1], vv[i1+1], vv[i1+2]);
		glm::vec3 v2 (vv[i2], vv[i2+1], vv[i2+2]);
			 
		// Vertices
		verts += v0.x,v0.y,v0.z, v1.x,v1.y,v1.z,v2.x,v2.y,v2.z;
	
		glm::vec3 t0 = v1 - v0;
		glm::vec3 t1 = v2 - v0;
		glm::vec3 n = glm::cross(t0,t1);
		
		// TexCoords, normals and ids
		
		for (size_t j =0; j < 3; j++){
			texs += 0,0;
			norms += n.x, n.y, n.z;			
		}
	}
			
	// Finally, compile the VBO
	v.push_back(verts);
	v.push_back(norms);
	v.push_back(texs);
	v.setNumElements(verts.size());
	
	Primitive p(v);
	
	return p;
}*/
