/*
* @brief Scanner to recreate our mesh from lasers
* @file scanner.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef LEEDS_SCANNER_HPP
#define LEEDS_SCANNER_HPP

#include "s9/common.hpp"

#include "s9/cvprocess.hpp"
#include "s9/gl/glasset.hpp"
#include "s9/geometry.hpp"
#include "s9/gl/video.hpp"

// PCL Includes

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

 
namespace leeds {
    
    /*
     * Detect Laser Light and Scan for us
     */

    class Scanner {
    public:
      Scanner();
      bool detectPoint(s9::gl::compvis::CVVidCam cam, cv::Point2f &point, cv::Mat &result);
      cv::Point3f solveForAll( std::vector< std::pair< cv::Point2f, s9::gl::compvis::CameraParameters > > points);

    protected:
      s9::compvis::Process _detect_proc;
    };


    /*
     * The mesher links in PCL and creates a mesh for us
     */

    class Mesher {
    public:
      Mesher();

      s9::GeometryPNF generate(s9::GeometryPF geom);
      void savePCL(std::string filename);
      void loadPCL(std::string filename);

      void setMeanK(float_t val) {_mean_k = val; }
      void setStdDev(float_t val) {_std_dev = val; }
      void setRadius(float_t val) {_radius = val; }
      void setOrder(int val) {_order = val; }
      void setUpsampling(float_t val) {_upsampling = val;}
      void setStep(float_t val){_step = val;}
      void setNormalRadius(float_t val){_normal_radius = val;}
      void setPoissonDepth(int val){_poisson_depth = val;}
      void setPoissonSamples(float_t val){_poisson_samples = val;}
      void setPoissonScale(float_t val){_poisson_scale = val;}

    protected:
      pcl::PointCloud<pcl::PointXYZ>::Ptr _cloud;
      pcl::PointCloud<pcl::PointXYZ>::Ptr _cloud_filtered;
      pcl::PointCloud<pcl::PointXYZ>::Ptr _cloud_smoothed;
      pcl::PolygonMesh _mesh;
      pcl::PointCloud<pcl::Normal>::Ptr _normals;
      pcl::search::KdTree<pcl::PointXYZ>::Ptr _tree;
      pcl::PointCloud<pcl::PointNormal>::Ptr _point_normals;

      float_t _mean_k, _std_dev, _radius, _upsampling, _step,
        _normal_radius, _poisson_samples, _poisson_scale;
      
      int _poisson_depth, _order;

    };

}


#endif