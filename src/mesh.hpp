/**
* @brief Mesh Class that deals with PCL Libs and the final meshing
* @file mesh.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 18/05/2012
*
*/

#ifndef _LEEDSMESH_HPP_
#define _LEEDSMESH_HPP_


#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h> 
#include <pcl/filters/passthrough.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <pcl/io/vtk_io.h>
#include <pcl/io/vtk_lib_io.h>
#include <pcl/surface/grid_projection.h>
#include <pcl/surface/mls.h>
#include <pcl/surface/mls_omp.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/surface/poisson.h>
#include <pcl/features/normal_3d_omp.h>

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <opencv2/opencv.hpp>

#include <utility>
#include <map>

#include "camera_manager.hpp"
#include "config.hpp"


/*
 * Winged edge classes for faster lookups
 * Data is encapsulated within these classes
 */

class WE_Vertex;
class WE_Face;

class WE_Edge {
public:
	boost::shared_ptr<WE_Vertex> v0, v1;
	boost::shared_ptr<WE_Face> face;
	boost::shared_ptr<WE_Edge> prev, next, sym;
};

class WE_Vertex {
public:
	std::vector< boost::shared_ptr<WE_Edge> > edges;
	GLfloat x,y,z;

};

class WE_Face {
public:
	boost::shared_ptr<WE_Edge> edge;
	GLfloat nx,ny,nz;
	GLuint t;
};


/*
 * Class for the scanned-in mesh
 */


class LeedsMesh {
public:

	LeedsMesh() {};
	void setup(GlobalConfig &config);
	void addPoint(double_t x, double_t y, double_t z);
	void generate(std::vector<boost::shared_ptr<LeedsCam> >&cameras);
	void saveToFile(std::string filename);
	void clearMesh();
	void saveMeshToFile(std::string filename);
	
	void loadFromSTL(std::string filename, std::vector<boost::shared_ptr<LeedsCam> >&cameras);
	void loadFile(std::string filename);

	VBOData& getMeshVBO() { return mObj->mMeshVBO; };
	VBOData& getPointsVBO() { return mObj->mPointsVBO; };
	VBOData& getPointsFilteredVBO() { return mObj->mPointsFilteredVBO; };
	VBOData& getNormalsVBO() { return mObj->mNormalsVBO; };
	VBOData& getComputedNormalsVBO() { return mObj->mComputedNormalsVBO; };
	

protected:

	static size_t sBufferSize;
	
	void generateMeshVBO(std::vector<boost::shared_ptr<LeedsCam> >&cameras);
	void generateWinged(std::vector<boost::shared_ptr<LeedsCam> >&cameras, bool reverse);
	void generateTexIDs(std::vector<boost::shared_ptr<LeedsCam> >&cameras);
	void textureMap(std::vector<boost::shared_ptr<LeedsCam> >&cameras);

	struct SharedObj {
	
		SharedObj(GlobalConfig &config) : mConfig(config) {};

		pcl::PointCloud<pcl::PointXYZ>::Ptr pCloud;
		pcl::PointCloud<pcl::PointXYZ>::Ptr pCloudFiltered;
		pcl::PassThrough<pcl::PointXYZ> mPass;
		pcl::PolygonMesh mTriangles;
		pcl::PointCloud<pcl::Normal>::Ptr mNormals;
		pcl::search::KdTree<pcl::PointXYZ>::Ptr mTree;
		pcl::PointCloud<pcl::PointNormal>::Ptr mCloud_with_normals;
		
		bool mUpdate;
		bool mTextured;
		
		std::vector< boost::shared_ptr<WE_Face> > mWE;
				
		VBOData mMeshVBO;
		VBOData mPointsVBO;
		VBOData mNormalsVBO;
		VBOData mComputedNormalsVBO;
		VBOData mPointsFilteredVBO;

		GlobalConfig &mConfig;
	};
	
	boost::shared_ptr<SharedObj> mObj;
};



#endif
