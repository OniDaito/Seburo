/**
* @brief Mesh Class that deals with PCL Libs and the final meshing
* @file mesh.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 18/05/2012
*
*/

#include "mesh.hpp"

using namespace std;
using namespace boost;
using namespace pcl;

size_t LeedsMesh::sBufferSize = 100; // Number of elements in the buffer

void LeedsMesh::setup(GlobalConfig &config) {
	mObj.reset(new SharedObj(config));
	
	// Create basic mesh
	mObj->pCloud.reset(new pcl::PointCloud<pcl::PointXYZ>);	
	mObj->pCloudFiltered.reset(new pcl::PointCloud<pcl::PointXYZ>);
	
	// Create bits to generate normals
	mObj->mNormals.reset (new pcl::PointCloud<pcl::Normal>);
	mObj->mTree.reset (new pcl::search::KdTree<pcl::PointXYZ>);
	mObj->mCloud_with_normals.reset (new pcl::PointCloud<pcl::PointNormal>);

	
	// Create a simple initial filter
	mObj->mPass.setInputCloud (mObj->pCloud);
	mObj->mPass.setFilterFieldName ("z");
	mObj->mPass.setFilterLimits (-0.1,config.meshResolution.z);
	mObj->mPass.filter (*(mObj->pCloudFiltered));
	
	mObj->mTextured = false;
	
	/*for (int i=0; i < mObj->mConfig.endCam - mObj->mConfig.startCam +1 ; i++){
		mObj->vTextures.push_back(ofTexture());
		mObj->vTextures.back().allocate( mObj->mConfig.camSize.width,mObj->mConfig.camSize.height, GL_RGB); 
	}*/
	
	// Allocate Buffers for points VBO
	for (int i=0; i < sBufferSize * 3; i ++)
		mObj->mPointsVBO.mVertices.push_back(0.0f);
		
	mObj->mPointsVBO.compile(VBO_VERT);
	
	// Override the default numelements here

	mObj->mPointsVBO.mNumElements = 0;
	
	
}

/*
 * Set a point. This is in OpenCV co-ordinates (chessboard world co-ordinates)
 * Call on the OpenGL Thread and not the the update thread!
 */

void LeedsMesh::addPoint(double_t x, double_t y, double_t z){
	pcl::PointXYZ pos(x,y,z);
	mObj->pCloud->points.push_back(pos);
	mObj->mUpdate = true;

	mObj->mPointsVBO.mVertices[mObj->mPointsVBO.mNumElements * 3] = static_cast<GLfloat>(x);
	mObj->mPointsVBO.mVertices[mObj->mPointsVBO.mNumElements * 3 + 1] = static_cast<GLfloat>(y);
	mObj->mPointsVBO.mVertices[mObj->mPointsVBO.mNumElements * 3 + 2] = static_cast<GLfloat>(z);

	// update the buffer on the card
	mObj->mPointsVBO.bind();
	glBindBuffer(GL_ARRAY_BUFFER, mObj->mPointsVBO.mVID);
	glBufferSubData(GL_ARRAY_BUFFER, mObj->mPointsVBO.mNumElements * 3 * sizeof(GLfloat),
				3 * sizeof(GLfloat), &mObj->mPointsVBO.mVertices[mObj->mPointsVBO.mNumElements * 3]);
	
	mObj->mPointsVBO.unbind();
	
	mObj->mPointsVBO.mNumElements++;
	
	checkError(__LINE__);
	
	// at this point test to see if the size of the VBO needs to be increased - Do this when its 2/3rds full
	if (mObj->mPointsVBO.mNumElements * 3  > static_cast<GLfloat>(mObj->mPointsVBO.mVertices.size() ) * 0.6){
		
		for (int i=0; i < sBufferSize * 3; i ++)
			mObj->mPointsVBO.mVertices.push_back(0.0f);
		
		mObj->mPointsVBO.bind();
		mObj->mPointsVBO.allocateVertices();
		mObj->mPointsVBO.unbind();
	}

	
}

/*
 * Convert the winged edge into a VBO
 */

void LeedsMesh::generateMeshVBO(std::vector<boost::shared_ptr<LeedsCam> >&cameras) {	
	
	mObj->mMeshVBO.mVertices.clear();
	mObj->mMeshVBO.mNormals.clear();
	mObj->mMeshVBO.mTexCoords.clear();
	mObj->mMeshVBO.vTexIDs.clear();

			
	for (size_t i = 0; i < mObj->mWE.size(); ++i) {
	
		shared_ptr<WE_Face> sf = mObj->mWE[i];
			
		shared_ptr<WE_Vertex> v0 = sf->edge->v0;
		shared_ptr<WE_Vertex> v1 = sf->edge->next->v0;
		shared_ptr<WE_Vertex> v2 = sf->edge->next->next->v0;
			
		// Vertices
		
		mObj->mMeshVBO.mVertices.push_back(v0->x);
		mObj->mMeshVBO.mVertices.push_back(v0->y);
		mObj->mMeshVBO.mVertices.push_back(v0->z);
		
		mObj->mMeshVBO.mVertices.push_back(v1->x);
		mObj->mMeshVBO.mVertices.push_back(v1->y);
		mObj->mMeshVBO.mVertices.push_back(v1->z);
		
		mObj->mMeshVBO.mVertices.push_back(v2->x);
		mObj->mMeshVBO.mVertices.push_back(v2->y);
		mObj->mMeshVBO.mVertices.push_back(v2->z);
		
		// TexCoords
		
		vector<cv::Point3f> tOPoints;
		tOPoints.push_back(cv::Point3f(v0->x, v0->y, v0->z));
		tOPoints.push_back(cv::Point3f(v1->x, v1->y, v1->z));
		tOPoints.push_back(cv::Point3f(v2->x, v2->y, v2->z));
		vector<cv::Point2f> results;
				
		boost::shared_ptr<LeedsCam> cam = cameras[ sf->t ];
		CameraParameters in = cam->getParams();
		cv::projectPoints(tOPoints, in.R, in.T, in.M, in.D, results );
	
		// We need to mirror the co-ordinates here but only in the X plane
		
		mObj->mMeshVBO.mTexCoords.push_back(results[0].x);
		mObj->mMeshVBO.mTexCoords.push_back(results[0].y);
		
		mObj->mMeshVBO.mTexCoords.push_back(results[1].x);
		mObj->mMeshVBO.mTexCoords.push_back(results[1].y);
		
		mObj->mMeshVBO.mTexCoords.push_back(results[2].x);
		mObj->mMeshVBO.mTexCoords.push_back(results[2].y);
		
			
		// Normals and TexIDs
		///\todo could use the winged edge to interpolate the normals for smoother shading?
		
		for (size_t j =0; j < 3; j++){
			mObj->mMeshVBO.mNormals.push_back(sf->nx);
			mObj->mMeshVBO.mNormals.push_back(sf->ny);
			mObj->mMeshVBO.mNormals.push_back(sf->nz);
			mObj->mMeshVBO.vTexIDs.push_back(sf->t);
		}
	}
			
	// Finally, compile the VBO
	mObj->mMeshVBO.compile(VBO_VERT | VBO_TEXC | VBO_NORM | VBO_TEXI);
	checkError(__LINE__);
	
	// Now create the normals VBO
	
	mObj->mNormalsVBO.mColours.clear();
	mObj->mNormalsVBO.mVertices.clear();
	
	// Create the normals VBO for drawing
	for (size_t i = 0; i < mObj->mMeshVBO.mNormals.size(); i+=3){
		GLfloat x =  mObj->mMeshVBO.mVertices[i];
		GLfloat y =  mObj->mMeshVBO.mVertices[i+1];
		GLfloat z =  mObj->mMeshVBO.mVertices[i+2];
		
		GLfloat nx =  x + mObj->mMeshVBO.mNormals[i];
		GLfloat ny =  y + mObj->mMeshVBO.mNormals[i+1];
		GLfloat nz =  z + mObj->mMeshVBO.mNormals[i+2];
		
		mObj->mNormalsVBO.mVertices.push_back(x);
		mObj->mNormalsVBO.mVertices.push_back(y);
		mObj->mNormalsVBO.mVertices.push_back(z);
		mObj->mNormalsVBO.mVertices.push_back(nx);
		mObj->mNormalsVBO.mVertices.push_back(ny);
		mObj->mNormalsVBO.mVertices.push_back(nz);
		
		mObj->mNormalsVBO.mColours.push_back(1.0f);
		mObj->mNormalsVBO.mColours.push_back(0.0f);
		mObj->mNormalsVBO.mColours.push_back(0.0f);
		mObj->mNormalsVBO.mColours.push_back(0.8f);
		
		mObj->mNormalsVBO.mColours.push_back(0.0f);
		mObj->mNormalsVBO.mColours.push_back(1.0f);
		mObj->mNormalsVBO.mColours.push_back(0.0f);
		mObj->mNormalsVBO.mColours.push_back(0.8f);
		
	}		
	
	mObj->mNormalsVBO.mNumElements = mObj->mMeshVBO.mNormals.size();
	mObj->mNormalsVBO.compile(VBO_VERT | VBO_COLR);
	checkError(__LINE__);
	
		
	cout << "Leeds Mesh VBO Normals Count: " << mObj->mMeshVBO.mNormals.size() / 3 << endl;	
	cout << "Leeds Mesh VBO Triangle count: " << mObj->mMeshVBO.mVertices.size() / 9 << endl;
	cout << "Leeds Mesh VBO TexIDS count: " << mObj->mMeshVBO.vTexIDs.size() << endl;
	
}


/*
 * Generate the Winged Edge Structure with embedded data
 */

void LeedsMesh::generateWinged(std::vector<boost::shared_ptr<LeedsCam> >&cameras, bool reverse = false) {
	// Loop through computed cloud and save values

	mObj->mWE.clear();
	
	unsigned int nr_points = mObj->mTriangles.cloud.width * mObj->mTriangles.cloud.height;
	unsigned int nr_polygons = static_cast<unsigned int> (mObj->mTriangles.polygons.size ());

	// get field indices for x, y, z (as well as rgb and/or rgba)
	int idx_x = -1, idx_y = -1, idx_z = -1, idx_rgb = -1, idx_rgba = -1, idx_normal_x = -1, idx_normal_y = -1, idx_normal_z = -1;
	
	for (int d = 0; d < static_cast<int> (mObj->mTriangles.cloud.fields.size ()); ++d) {
		if (mObj->mTriangles.cloud.fields[d].name == "x") idx_x = d;
		else if (mObj->mTriangles.cloud.fields[d].name == "y") idx_y = d;
		else if (mObj->mTriangles.cloud.fields[d].name == "z") idx_z = d;
		else if (mObj->mTriangles.cloud.fields[d].name == "rgb") idx_rgb = d;
		else if (mObj->mTriangles.cloud.fields[d].name == "rgba") idx_rgba = d;
		else if (mObj->mTriangles.cloud.fields[d].name == "normal_x") idx_normal_x = d;
		else if (mObj->mTriangles.cloud.fields[d].name == "normal_y") idx_normal_y = d;
		else if (mObj->mTriangles.cloud.fields[d].name == "normal_z") idx_normal_z = d;
		else
			cout << mObj->mTriangles.cloud.fields[d].name << endl;
		
	}
	if ( ( idx_x == -1 ) || ( idx_y == -1 ) || ( idx_z == -1 ) )
		nr_points = 0;

	// Create a temporary array of vertices for now. 
	///\todo since we only ever get vertices from Poisson we could remove the step below?
	std::vector< boost::shared_ptr<WE_Vertex> > vs;

	if (nr_points > 0){					
		Eigen::Array4i xyz_offset (mObj->mTriangles.cloud.fields[idx_x].offset, mObj->mTriangles.cloud.fields[idx_y].offset, mObj->mTriangles.cloud.fields[idx_z].offset, 0);
		for (vtkIdType cp = 0; cp < static_cast<vtkIdType> (nr_points); ++cp, xyz_offset += mObj->mTriangles.cloud.point_step) {
			float_t v;
			shared_ptr<WE_Vertex> sv (new WE_Vertex);
			vs.push_back(sv);
			memcpy(&v, &mObj->mTriangles.cloud.data[xyz_offset[0]], sizeof (float_t));
			sv->x = v;
			memcpy(&v, &mObj->mTriangles.cloud.data[xyz_offset[1]], sizeof (float_t));
			sv->y = v;
			memcpy(&v, &mObj->mTriangles.cloud.data[xyz_offset[2]], sizeof (float_t));
			sv->z = v;
		}
	} else
		return;
	
	
	cerr << "Leeds Winged Edge Vertices Count: " << vs.size() << endl;

	
	// Now create proper winged edge and indices using a concatenated 32 bit index to a 64 bit key
	// In addition we create a straight list of unique triangles
	
	map< uint64_t, shared_ptr<WE_Edge> > es;
	
	if (nr_polygons > 0) {
		for (unsigned int i = 0; i < nr_polygons; i++) {
			
			unsigned int nr_points_in_polygon = static_cast<unsigned int> (mObj->mTriangles.polygons[i].vertices.size ());
			if (nr_points_in_polygon != 3){
				cerr << "Leeds - Non triangular polygon detected" << endl;
				throw;
			}


			shared_ptr<WE_Face>  f (new WE_Face());
			bool bf = false;
			uint32_t idcs[3];
			
			if (reverse){
				for (int j =2; j >= 0; j--)
					idcs[j] = mObj->mTriangles.polygons[i].vertices[j];
			}
			else{
				for (int j =0; j < 3; j++)
					idcs[j] = mObj->mTriangles.polygons[i].vertices[j];
			}
			
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
		
	}
	
	cerr << "Leeds Winged Edge edges count: " << es.size() << endl;
	cerr << "Leeds Winged Edge Face Count: " << mObj->mWE.size() << endl;
	
	// Recreate the normals using a bruteforce simple method
		
	for (size_t i = 0; i < mObj->mWE.size(); ++i) {
		
		shared_ptr<WE_Face> sf = mObj->mWE[i];
			
		shared_ptr<WE_Vertex> v0 = sf->edge->v0;
		shared_ptr<WE_Vertex> v1 = sf->edge->next->v0;
		shared_ptr<WE_Vertex> v2 = sf->edge->next->next->v0;
		
	
		// Use Eigen for conversion as its part of PCL anyway
		Eigen::Vector3d v(v0->x, v0->y, v0->z);
		Eigen::Vector3d w(v1->x, v1->y, v1->z);
		Eigen::Vector3d d(v2->x, v2->y, v2->z);
		
		Eigen::Vector3d n = (w - v).cross(w - d);
		n.normalize();
	
		// These normals are effectively face normals and not interpolated "across" the surface
		sf->nx = n.x();
		sf->ny = n.y();
		sf->nz = n.z();
	}
	
	generateTexIDs(cameras);
}


/*
 * Generate the TexIDs for the Winged Edge
 */

void LeedsMesh::generateTexIDs(std::vector<boost::shared_ptr<LeedsCam> >&cameras) {
	vector<Eigen::Vector3d> normals;
	
	for (int i =0; i < cameras.size(); i ++){
		boost::shared_ptr<LeedsCam> cam = cameras[i];
		cv::Mat n = cam->getNormal();
		Eigen::Vector3d nn (n.at<double_t>(0,0),n.at<double_t>(1,0),n.at<double_t>(2,0));
		nn.normalize();
		normals.push_back(nn);
	}
	
	for (size_t i = 0; i < mObj->mWE.size(); ++i) {
		
		shared_ptr<WE_Face> sf = mObj->mWE[i];
			
		Eigen::Vector3d p(sf->nx,sf->ny,sf->nz);
				
		float range = 3.0;
		GLuint idx = 0;
		for (GLuint j=0; j < normals.size(); j++){
			
			Eigen::Vector3d nn = normals[j];
				
			float angle =  acosf(p.dot(nn)) ;
			if (fabs(angle) < range){
				idx = j;
				range = fabs(angle);
			}
		}
		sf->t = idx;
	}
	
	// Now we have texids - check winged edge for these we need to change
	// Assume a maximum choice of 8 textures
	
	for (size_t i = 0; i < mObj->mWE.size(); ++i) {	
		shared_ptr<WE_Face> sf = mObj->mWE[i];
		
		int counts[] = {0,0,0,0,0,0,0,0};
		
		if (sf->edge->sym != boost::shared_ptr<WE_Edge>()) counts[sf->edge->sym->face->t]+=1;
		if (sf->edge->next->sym != boost::shared_ptr<WE_Edge>()) counts[sf->edge->next->sym->face->t]+=1;
		if (sf->edge->next->next->sym != boost::shared_ptr<WE_Edge>()) counts[sf->edge->next->next->sym->face->t]+=1;
		
		int tc = 0;
		for (size_t j=0; j < 8; j++){
			if (counts[j] > tc){
				sf->t = j;
				tc = counts[j];
			}
		}
	}
}


/*
 * Update mesh if needed, if any values have changed
 */

void LeedsMesh::generate(std::vector<boost::shared_ptr<LeedsCam> >&cameras) {
	
	if (mObj->pCloud->points.size() > 3){
		
		try {
		// Statistical removal
			pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
			sor.setInputCloud (mObj->pCloud);
			sor.setMeanK (mObj->mConfig.pclFilterMeanK);
			sor.setStddevMulThresh (mObj->mConfig.pclFilterThresh);
			sor.filter (*(mObj->pCloudFiltered));
			
			// now setup the points VBO for filtered points
			
			mObj->mPointsFilteredVBO.mVertices.clear();
			for (pcl::PointCloud<pcl::PointXYZ>::iterator it = mObj->pCloudFiltered->begin(); it != mObj->pCloudFiltered->end(); it++){
				mObj->mPointsFilteredVBO.mVertices.push_back( it->x);
				mObj->mPointsFilteredVBO.mVertices.push_back( it->y);
				mObj->mPointsFilteredVBO.mVertices.push_back( it->z);
			}
			mObj->mPointsFilteredVBO.compile(VBO_VERT);
			
			
			MovingLeastSquaresOMP<PointXYZ, PointXYZ> mls;
			mls.setNumberOfThreads(6);
			mls.setInputCloud (mObj->pCloudFiltered);
			mls.setSearchRadius (mObj->mConfig.pclSearchRadius);
			mls.setPolynomialFit (true);
			mls.setPolynomialOrder (mObj->mConfig.pclPolynomialOrder);
			mls.setUpsamplingMethod (MovingLeastSquares<PointXYZ, PointXYZ>::SAMPLE_LOCAL_PLANE);
			mls.setUpsamplingRadius (mObj->mConfig.pclUpsamplingRadius);
			mls.setUpsamplingStepSize (mObj->mConfig.pclUpsamplingStepSize);
			PointCloud<PointXYZ>::Ptr cloud_smoothed (new PointCloud<PointXYZ> ());
			mls.process (*cloud_smoothed);
			
			cout << "Leeds - Cloud smoothed: " << cloud_smoothed->size() << endl;
			
			NormalEstimationOMP<PointXYZ, Normal> ne;
			ne.setNumberOfThreads (8);
			ne.setInputCloud (cloud_smoothed);
			ne.setRadiusSearch (0.01);
			
			Eigen::Vector4f centroid;
			compute3DCentroid (*cloud_smoothed, centroid);
			ne.setViewPoint (centroid[0], centroid[1], centroid[2]);
			PointCloud<Normal>::Ptr cloud_normals (new PointCloud<Normal> ());
			ne.compute (*cloud_normals);
				
			
			cout << "Leeds Centroid: " << centroid << endl;
			
			cout << "Leeds Normals Smoothed: " << cloud_normals->size() << endl;
		
	/*		for (size_t i = 0; i < cloud_normals->size (); ++i) {
				cloud_normals->points[i].normal_x *= -1;
				cloud_normals->points[i].normal_y *= -1;
				cloud_normals->points[i].normal_z *= -1;
			}*/
			
			PointCloud<PointNormal>::Ptr cloud_smoothed_normals (new PointCloud<PointNormal> ());
			concatenateFields (*cloud_smoothed, *cloud_normals, *cloud_smoothed_normals);
			
			// Setup the computed normals VBO
			mObj->mComputedNormalsVBO.mVertices.clear();
			for (size_t i = 0; i < cloud_smoothed_normals->size (); ++i) {
				
				mObj->mComputedNormalsVBO.mVertices.push_back( cloud_smoothed_normals->points[i].x);
				mObj->mComputedNormalsVBO.mVertices.push_back( cloud_smoothed_normals->points[i].y);
				mObj->mComputedNormalsVBO.mVertices.push_back( cloud_smoothed_normals->points[i].z);
				
				mObj->mComputedNormalsVBO.mColours.push_back(0.0);
				mObj->mComputedNormalsVBO.mColours.push_back(1.0);
				mObj->mComputedNormalsVBO.mColours.push_back(0.0);
				mObj->mComputedNormalsVBO.mColours.push_back(1.0);
				
				mObj->mComputedNormalsVBO.mVertices.push_back( cloud_smoothed_normals->points[i].x + cloud_smoothed_normals->points[i].normal_x);
				mObj->mComputedNormalsVBO.mVertices.push_back( cloud_smoothed_normals->points[i].y + cloud_smoothed_normals->points[i].normal_y );
				mObj->mComputedNormalsVBO.mVertices.push_back( cloud_smoothed_normals->points[i].z + cloud_smoothed_normals->points[i].normal_z);
			
				mObj->mComputedNormalsVBO.mColours.push_back(0.0);
				mObj->mComputedNormalsVBO.mColours.push_back(0.0);
				mObj->mComputedNormalsVBO.mColours.push_back(1.0);
				mObj->mComputedNormalsVBO.mColours.push_back(1.0);
			
			}
			mObj->mComputedNormalsVBO.compile(VBO_VERT | VBO_COLR);

			pcl::Poisson<pcl::PointNormal> pp;
			pp.setInputCloud(cloud_smoothed_normals);
			pp.setDepth(mObj->mConfig.poissonDepth);
			//pp.setOutputPolygons(true);
			pp.setSamplesPerNode(mObj->mConfig.poissonSamples);
			pp.setScale(mObj->mConfig.poissonScale);
			pp.reconstruct (mObj->mTriangles);
	
		
		}
		catch(...) {
			std::cerr << "Leeds - Exception in  generating mesh" << std::endl;
		}
		
		generateWinged(cameras);
		generateMeshVBO(cameras);
	}
}



/*
 * Generates a mesh from the STL
 */

void LeedsMesh::loadFromSTL(std::string filename, std::vector<boost::shared_ptr<LeedsCam> >&cameras) {

	clearMesh();
	
	pcl::io::loadPolygonFileSTL ( filename, mObj->mTriangles);
			
	generateWinged(cameras, true);
	generateMeshVBO(cameras);
	
}



/*
 * Save the filtered cloud to a PCD File
 */

void LeedsMesh::saveToFile(std::string filename) {
	mObj->pCloud->width = mObj->pCloud->points.size(); 
	mObj->pCloud->height = 1;
	pcl::io::savePCDFileASCII (filename,  *(mObj->pCloud));
	std::cerr << "Saved " <<  mObj->pCloud->points.size () << " data points to " << filename << std::endl;
	
}

/*
 * Save triangulated mesh to an STL file
 */

void LeedsMesh::saveMeshToFile(std::string filename){
	pcl::io::savePolygonFileSTL (filename, mObj->mTriangles);
}

/*
 * Clear the mesh completely
 */

void LeedsMesh::clearMesh() {
	mObj->pCloud->clear();
	mObj->pCloudFiltered->clear();
	mObj->mNormals->clear();
	mObj->mCloud_with_normals->clear();	
	
	// Clear the VBOs
	
	mObj->mPointsVBO.mVertices.clear();
	mObj->mPointsVBO.mNumElements = 0;
	
	mObj->mNormalsVBO.mVertices.clear();
	mObj->mNormalsVBO.mNumElements = 0;
	
	
	mObj->mMeshVBO.mVertices.clear();
	mObj->mMeshVBO.mNormals.clear();
	mObj->mMeshVBO.mTexCoords.clear();
	mObj->mMeshVBO.mIndices.clear();
	mObj->mMeshVBO.vTexIDs.clear();
	mObj->mMeshVBO.mNumElements = 0;
	mObj->mMeshVBO.mNumIndices = 0;
	
	// Clear the Winged Edge
	mObj->mWE.clear();
}

/*
 * Loud a cloud from a pcd file, ready for meshing
 */
 
 void LeedsMesh::loadFile(std::string filename){

	if (pcl::io::loadPCDFile<pcl::PointXYZ> (filename, *(mObj->pCloud)) == -1) {
		cerr << "Leeds - Could NOT load PCD file " <<  filename << endl;
		return;
	}
	
	// Run through and add the points to the VBO
	
	mObj->mPointsVBO.mVertices.clear();
	mObj->mPointsVBO.mNumElements = 0;
	
	for ( pcl::PointCloud<pcl::PointXYZ>::iterator it = mObj->pCloud->begin(); it != mObj->pCloud->end(); it++) {
		pcl::PointXYZ pos = *it;
		
		mObj->mPointsVBO.mVertices.push_back(static_cast<GLfloat>(pos.x));
		mObj->mPointsVBO.mVertices.push_back(static_cast<GLfloat>(pos.y));
		mObj->mPointsVBO.mVertices.push_back(static_cast<GLfloat>(pos.z));
		mObj->mPointsVBO.mNumElements++;
		
	}
	
	// update the buffer on the card
	mObj->mPointsVBO.bind();
	mObj->mPointsVBO.allocateVertices();
	mObj->mPointsVBO.unbind();
	

	checkError(__LINE__);
	
	
	cerr << "Leeds - Loaded PCD file " <<  filename << " with " << mObj->mPointsVBO.mNumElements << " points." << endl;
	
	
	mObj->mUpdate = true;
 }

