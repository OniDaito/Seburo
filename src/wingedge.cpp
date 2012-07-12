/**
* @brief Wing Edged Classes
* @file wingedge.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/07/2012
*
*/

#include "wingedge.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign; 
using namespace s9;


void WingedEdge::make(Primitive p) {
	
	mObj.reset(new SharedObj());
	mObj->mPrimitive = p;
	
	// Create a temporary array of vertices for now. 
	std::vector< boost::shared_ptr<WE_Vertex> > vs;

	for (size_t i=0; i < p.getVBO().vVertices.size(); i+=3){
		shared_ptr<WE_Vertex> sv (new WE_Vertex);
		vs.push_back(sv);
		sv->idc = i; // Copy the indices
	}
	
	
	cerr << "S9Gear Winged Edge Vertices Count: " << vs.size() << endl;

	// Now create proper winged edge and indices using a concatenated 32 bit index to a 64 bit key
	// In addition we create a straight list of unique triangles
	
	map< uint64_t, shared_ptr<WE_Edge> > es;
	
	size_t bw = 0;

	for (size_t i = 0; i < p.getVBO().vIndices.size(); i+=3) {

		shared_ptr<WE_Face>  f (new WE_Face());
		bool bf = false;
		uint32_t idcs[3];
		idcs[0] = p.getVBO().vIndices[i];
		idcs[1] = p.getVBO().vIndices[i+1];
		idcs[2] = p.getVBO().vIndices[i+2]; 

		// Find Edges to add to face
		map< uint64_t, shared_ptr<WE_Edge> >::iterator pi = es.end();
		
		boost::shared_ptr<WE_Edge> prev;
		boost::shared_ptr<WE_Edge> current;
		boost::shared_ptr<WE_Edge> first;
		
		vector<uint64_t> keys;
		
		for (int j = 0; j < 3; j++){
			
			map< uint64_t, shared_ptr<WE_Edge> >::iterator fi;
			
			// create two keys and perform lookup
			int k = j == 2 ? 0 : j + 1;
			uint64_t jk = idcs[j]; jk = jk << 32;
			jk |= idcs[k];
			
			uint64_t kj = idcs[k]; kj = kj << 32;
			kj |= idcs[j];
						
			// new edge	
			shared_ptr<WE_Edge> sp (new WE_Edge);
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
			es.insert( pair<uint64_t, shared_ptr<WE_Edge> > (jk, sp) );
			
		}
		
		if (!bf) {
			// Close the loop off
			first->prev = current;
			current->next = first;
	
			
			mObj->mWE.push_back(f);
		}else {
			// Delete any added edges for this badly wound face
			for (size_t i = 0; i < keys.size(); i++){
				map< uint64_t, shared_ptr<WE_Edge> >::iterator pi = es.find(keys[i]);
				es.erase (pi);      
			}
		}
	}
		
	cerr << "S9Gear Winged Edge edges count: " << es.size() << endl;
	cerr << "S9Gear Winged Edge Face Count: " << mObj->mWE.size() << endl;
	cerr << "S9Gear Winged Edge Discarded Face Count: " << bw << endl;
}

/*
 * Generate a primitive from this. Essentially flatten it out
 */

Primitive WingedEdge::flatten() { 

	Primitive p;
	p.make();
	
	for (size_t i = 0; i < mObj->mWE.size(); ++i) {
	
		shared_ptr<WE_Face> sf = mObj->mWE[i];
		
		size_t i0 = sf->edge->v0->idc;
		size_t i1 = sf->edge->next->v0->idc;
		size_t i2 = sf->edge->next->next->v0->idc;
		
		glm::vec3 v0 (mObj->mPrimitive.getVBO().vVertices[i0], mObj->mPrimitive.getVBO().vVertices[i0+1], mObj->mPrimitive.getVBO().vVertices[i0+2]);
		glm::vec3 v1 (mObj->mPrimitive.getVBO().vVertices[i1], mObj->mPrimitive.getVBO().vVertices[i1+1], mObj->mPrimitive.getVBO().vVertices[i1+2]);
		glm::vec3 v2 (mObj->mPrimitive.getVBO().vVertices[i2], mObj->mPrimitive.getVBO().vVertices[i2+1], mObj->mPrimitive.getVBO().vVertices[i2+2]);
			
		// Vertices
		
		p.getVBO().vVertices.push_back(v0.x);
		p.getVBO().vVertices.push_back(v0.y);
		p.getVBO().vVertices.push_back(v0.z);
		
		p.getVBO().vVertices.push_back(v1.x);
		p.getVBO().vVertices.push_back(v1.y);
		p.getVBO().vVertices.push_back(v1.z);
		
		p.getVBO().vVertices.push_back(v2.x);
		p.getVBO().vVertices.push_back(v2.y);
		p.getVBO().vVertices.push_back(v2.z);
		
		glm::vec3 t0 = v1 - v0;
		glm::vec3 t1 = v2 - v0;
		glm::vec3 n = glm::cross(t0,t1);
		
		// TexCoords, normals and ids
		
		for (size_t j =0; j < 3; j++){
			p.getVBO().vTexCoords.push_back(0);
			p.getVBO().vTexCoords.push_back(0);
			
			p.getVBO().vNormals.push_back(n.x);
			p.getVBO().vNormals.push_back(n.y);
			p.getVBO().vNormals.push_back(n.z);
			
			
			p.getVBO().vTexIDs.push_back(0);
		}
	}
			
	// Finally, compile the VBO
	p.getVBO().compile(VBO_VERT | VBO_TEXC | VBO_NORM | VBO_TEXI);
	
	return p;
}
