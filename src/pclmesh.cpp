/**
* @brief PCL Mesh Classes
* @file pclmesh.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/07/2012
*
*/

#include "pclmesh.hpp"

using namespace std;
using namespace boost;
using namespace boost::assign;
using namespace pcl;
using namespace s9;


size_t PCLMesh::sBufferSize = 100; // Number of elements in the buffer

/*
 * Create the Object
 */

void PCLMesh::make() {
	mObj.reset(new PCLMeshObj());
	
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
	mObj->mPass.filter (*(mObj->pCloudFiltered));
	
	// Allocate Buffers for points VBO
	for (int i=0; i < sBufferSize * 3; i ++)
		mObj->mPointsVBO.mVertices.push_back(0.0f);
		
	mObj->mPointsVBO.compile(VBO_VERT);

	mObj->mPointsVBO.mNumElements = 0;
	
}

void PCLMesh::setFilter(float_t min, float_t max) {mObj->mPass.setFilterLimits (min,max); };

void PCLMesh::setStatisticalFilter(float_t k, float_t thresh) { 
	mObj->mSor.setMeanK (k); 
	mObj->mSor.setStddevMulThresh (thresh); 
};

void PCLMesh::setMLS(float_t r, int p, float_t a, float_t s) {
	mObj->mMls.setSearchRadius (r);
	mObj->mMls.setPolynomialOrder (p);
	mObj->mMls.setUpsamplingRadius (a);
	mObj->mMls.setUpsamplingStepSize (s);
}
		
void PCLMesh::setPoisson(int d, float_t s, float_t c) {
	mObj->mPP.setDepth(d);
	mObj->mPP.setSamplesPerNode(s);
	mObj->mPP.setScale(c);
}
		
pcl::PolygonMesh& PCLMesh::getMesh() { return mObj->mTriangles; };

/*
 * Set a point. This is in OpenCV co-ordinates (chessboard world co-ordinates)
 * Call on the OpenGL Thread and not the the update thread!
 */

void PCLMesh::addPoint(glm::vec3 p){
	pcl::PointXYZ pos(p.x,p.y,p.z);
	mObj->pCloud->points.push_back(pos);

	mObj->mPointsVBO.mVertices[mObj->mPointsVBO.mNumElements * 3] = static_cast<GLfloat>(p.x);
	mObj->mPointsVBO.mVertices[mObj->mPointsVBO.mNumElements * 3 + 1] = static_cast<GLfloat>(p.y);
	mObj->mPointsVBO.mVertices[mObj->mPointsVBO.mNumElements * 3 + 2] = static_cast<GLfloat>(p.z);

	// update the buffer on the card
	mObj->mPointsVBO.bind();
	glBindBuffer(GL_ARRAY_BUFFER, mObj->mPointsVBO.mVID);
	glBufferSubData(GL_ARRAY_BUFFER, mObj->mPointsVBO.mNumElements * 3 * sizeof(GLfloat),
				3 * sizeof(GLfloat), &mObj->mPointsVBO.mVertices[mObj->mPointsVBO.mNumElements * 3]);
	
	mObj->mPointsVBO.unbind();
	
	mObj->mPointsVBO.mNumElements++;
	
	CXGLERROR
	
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
 * Update mesh if needed, if any values have changed
 */

void PCLMesh::generate() {
	
	if (mObj->pCloud->points.size() > 3){
		
		try {
			
			pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>);
			// Removal below ground plane
			pcl::PassThrough<pcl::PointXYZ> pass;
			pass.setInputCloud (mObj->pCloud);
			pass.setFilterFieldName ("z");
			pass.setFilterLimits (0.1, 18.0);
			//pass.setFilterLimitsNegative (true);
			pass.filter (*cloud_filtered);
			
			// Statistical removal
	
			mObj->mSor.setInputCloud (cloud_filtered);
			mObj->mSor.filter (*(mObj->pCloudFiltered));
			
			// now setup the points VBO for filtered points
			
			mObj->mPointsFilteredVBO.mVertices.clear();
			for (pcl::PointCloud<pcl::PointXYZ>::iterator it = mObj->pCloudFiltered->begin(); it != mObj->pCloudFiltered->end(); it++){
				mObj->mPointsFilteredVBO.mVertices.push_back( it->x);
				mObj->mPointsFilteredVBO.mVertices.push_back( it->y);
				mObj->mPointsFilteredVBO.mVertices.push_back( it->z);
			}
			mObj->mPointsFilteredVBO.compile(VBO_VERT);
			
			

			mObj->mMls.setNumberOfThreads(6);
			mObj->mMls.setInputCloud (mObj->pCloudFiltered);
			mObj->mMls.setPolynomialFit (true);	
			mObj->mMls.setUpsamplingMethod (pcl::MovingLeastSquares<PointXYZ, PointXYZ>::SAMPLE_LOCAL_PLANE);
			PointCloud<PointXYZ>::Ptr cloud_smoothed (new PointCloud<PointXYZ> ());
			mObj->mMls.process (*cloud_smoothed);
			
			cout << "S9Gear - Cloud smoothed: " << cloud_smoothed->size() << endl;
			
			NormalEstimationOMP<PointXYZ, Normal> ne;
			ne.setNumberOfThreads (8);
			ne.setInputCloud (cloud_smoothed);
			ne.setRadiusSearch (0.01);
			
			Eigen::Vector4f centroid;
			compute3DCentroid (*cloud_smoothed, centroid);
			ne.setViewPoint (centroid[0], centroid[1], centroid[2]);
			PointCloud<Normal>::Ptr cloud_normals (new PointCloud<Normal> ());
			ne.compute (*cloud_normals);
				
			
			cout << "S9Gear - PCL Mesh Centroid: " << centroid << endl;
			
			cout << "S9Gear - PCL Normals Smoothed: " << cloud_normals->size() << endl;
		
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

		
			mObj->mPP.setInputCloud(cloud_smoothed_normals);
			mObj->mPP.reconstruct (mObj->mTriangles);
		
		}
		catch(...) {
			std::cerr << "S9Gear - Exception in  generating mesh" << std::endl;
		}
		
	}
}



/*
 * Save the filtered cloud to a PCD File
 */

void PCLMesh::saveToFile(std::string filename) {
	mObj->pCloud->width = mObj->pCloud->points.size(); 
	mObj->pCloud->height = 1;
	pcl::io::savePCDFileASCII (filename,  *(mObj->pCloud));
	std::cerr << "S9Gear - Saved " <<  mObj->pCloud->points.size () << " data points to " << filename << std::endl;
	
}

/*
 * Save triangulated mesh to an STL file
 */

void PCLMesh::saveMeshToFile(std::string filename){
	pcl::io::savePolygonFileSTL (filename, mObj->mTriangles);
}

/*
 * Clear the mesh completely
 */

void PCLMesh::clearMesh() {
	mObj->pCloud->clear();
	mObj->pCloudFiltered->clear();
	mObj->mNormals->clear();
	mObj->mCloud_with_normals->clear();	
	
	// Clear the VBOs
	
	mObj->mPointsVBO.mVertices.clear();
	mObj->mPointsVBO.mNumElements = 0;
	
	mObj->mNormalsVBO.mVertices.clear();
	mObj->mNormalsVBO.mNumElements = 0;
	
	
	mObj->mVBO.mVertices.clear();
	mObj->mVBO.mNormals.clear();
	mObj->mVBO.mTexCoords.clear();
	mObj->mVBO.mIndices.clear();
	
	mObj->mMeshVBO.vTexIDs.clear();
	mObj->mMeshVBO.mNumElements = 0;
	mObj->mMeshVBO.mNumIndices = 0;
}

/*
 * Loud a cloud from a pcd file, ready for meshing
 */
 
 void PCLMesh::loadFile(std::string filename){

	if (pcl::io::loadPCDFile<pcl::PointXYZ> (filename, *(mObj->pCloud)) == -1) {
		cerr << "S9Gear - Could NOT load PCD file " <<  filename << endl;
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
	

	CXGLERROR	
	
	cerr << "S9Gear - Loaded PCD file " <<  filename << " with " << mObj->mPointsVBO.mNumElements << " points." << endl;
	
 }

