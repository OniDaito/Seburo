/**
* @brief PCL Mesh Classes
* @file pclmesh.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 09/07/2012
*
*/


#ifndef PCLMESH_HPP
#define PCLMESH_HPP

#include "common.hpp"
#include "primitive.hpp"
#include "vbo.hpp"

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


/*
 * Create a PCL Mesh 
 */

///\todo lots here! Need a conversion from mesh to VBO as at the moment, the VBO is the points
/// generate a set of children primitives from this one
 
namespace s9 {

	class PCLMesh : public Primitive {
	public:
		PCLMesh() {};
		void make();
		void generate();
		void addPoint(glm::vec3 p);
		void setFilter(float_t min, float_t max);
		void setStatisticalFilter(float_t k, float_t thresh);
		void setMLS(float_t r, int p, float_t a, float_t s);
		
		void setPoisson(int d, float_t s, float_t c);		
		pcl::PolygonMesh& getMesh();
		
		void saveToFile(std::string filename);
		void saveMeshToFile(std::string filename);
		void clearMesh();
		void loadFile(std::string filename);

		
	protected:
		class PCLMeshObj : public SharedObj {
		public:
			pcl::PointCloud<pcl::PointXYZ>::Ptr pCloud;
			pcl::PointCloud<pcl::PointXYZ>::Ptr pCloudFiltered;
			pcl::PassThrough<pcl::PointXYZ> mPass;
			pcl::PolygonMesh mTriangles;
			pcl::PointCloud<pcl::Normal>::Ptr mNormals;
			pcl::search::KdTree<pcl::PointXYZ>::Ptr mTree;
			pcl::PointCloud<pcl::PointNormal>::Ptr mCloud_with_normals;	
			pcl::StatisticalOutlierRemoval<pcl::PointXYZ> mSor;
			pcl::MovingLeastSquaresOMP<pcl::PointXYZ, pcl::PointXYZ> mMls;			
			pcl::Poisson<pcl::PointNormal> mPP;	
						
			// Extra VBOs for fun things
			///\todo these should be seperate primitives as children!
			VBOData mPointsVBO;
			VBOData mNormalsVBO;
			VBOData mMeshVBO;
			VBOData mComputedNormalsVBO;
			VBOData mPointsFilteredVBO;

		};
		
		
		boost::shared_ptr<PCLMeshObj> mObj;
		static size_t sBufferSize; 
	};
	
}

#endif
