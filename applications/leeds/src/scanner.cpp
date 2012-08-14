/**
* @brief Body of the Scanner class
* @file scanner.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#include "scanner.hpp"


using namespace std;
using namespace boost;
using namespace boost::assign;
using namespace s9;
using namespace s9::gl;
using namespace s9::compvis;
using namespace s9::gl::compvis;
using namespace cv;
using namespace leeds;
using namespace pcl;

Scanner::Scanner(){
;

  _detect_proc.addBlock<BlockGreyscale>()
    .addBlock<BlockThreshold>()
    .addBlock<BlockDetectPoint>();
}


bool Scanner::detectPoint(CVVidCam cam, cv::Point2f &point, cv::Mat &result){
  result = _detect_proc.process(cam.getImageRectified());
  point = cv::Point2f(_detect_proc.getBlock(2).getValue<cv::Point2f>("point"));
  return _detect_proc.getBlock(2).getValue<bool>("found");
}


cv::Point3f Scanner::solveForAll( std::vector< std::pair< cv::Point2f, CameraParameters > > points) {
  
  int idx = 0;
  int idy = 0;
  
  vector<Point2f> results;
  
  typedef std::pair<cv::Point2f, CameraParameters > CamPoint;

  Mat m (Size(3 + points.size(), 3 * points.size()), CV_64FC1);
  Mat s (Size(1,4), CV_64FC1);
  Mat c (Size(1,3 * points.size()), CV_64FC1);
  
  // Now create our matrices and solve
  idy = 0;
  
  BOOST_FOREACH ( CamPoint p, points) { 
    CameraParameters in = p.second;
      
    vector<Point2f> tpoints;
    tpoints.push_back(p.first);
    undistortPoints(tpoints,results, in.M, in.D);
  
    Mat r (Size(3,3), CV_64FC1);
    Rodrigues(in.R,r);
    
    // now create the solving matrices
    
    m.at<double_t>(idx,0) = r.at<double_t>(0,0);
    m.at<double_t>(idx,1) = r.at<double_t>(0,1);
    m.at<double_t>(idx,2) = r.at<double_t>(0,2);
    
    m.at<double_t>(idx + 1,0) = r.at<double_t>(1,0);
    m.at<double_t>(idx + 1,1) = r.at<double_t>(1,1);
    m.at<double_t>(idx + 1,2) = r.at<double_t>(1,2);
    
    m.at<double_t>(idx + 2,0) = r.at<double_t>(2,0);
    m.at<double_t>(idx + 2,1) = r.at<double_t>(2,1);
    m.at<double_t>(idx + 2,2) = r.at<double_t>(2,2);
    
    // special values depending on idx
    
    for (int i = 0; i < points.size(); i ++) {
      
      if (i == idy) {
        m.at<double_t>(idx,3 + i) = -results[0].x;
        m.at<double_t>(idx+1,3 + i) = -results[0].y;
        m.at<double_t>(idx+2,3 + i) = -1.0;
      }
      else {
        m.at<double_t>(idx,3+ i) = 0;
        m.at<double_t>(idx+1,3+ i) = 0;
        m.at<double_t>(idx+2,3+ i) = 0;
      }
    }
    
    c.at<double_t>(idx,0) = -in.T.at<double_t>(0,0);
    c.at<double_t>(idx+1,0) = -in.T.at<double_t>(1,0);
    c.at<double_t>(idx+2,0) = -in.T.at<double_t>(2,0);
  
    idx += 3;
    idy++;

  }
  
  // Should only be one. Now solve and send back 

  solve(m,c,s,DECOMP_QR);
  
  Point3f p(s.at<double_t>(0,0),s.at<double_t>(1,0),s.at<double_t>(2,0));
  
  cerr << "Leeds - Computed depth point: " << p << endl;
  
  return p;
}



Mesher::Mesher() {
  _mean_k = 50.0f;
  _std_dev = 1.0f;
  _radius = 1.0f;
  _upsampling = 1.0f;
  _step = 0.003f;
  _normal_radius = 1.0f;
  _poisson_samples = 1.0f;
  _poisson_scale = 1.0f;
  _poisson_depth = 9;
  _order = 4;
}



/*
 * Generate a mesh for us using the PCL
 * \todo this could be made better by using geom in place perhaps
 */

GeometryPNF Mesher::generate(GeometryPF geom){

  _cloud.reset(new pcl::PointCloud<pcl::PointXYZ>);
  _cloud_filtered.reset(new pcl::PointCloud<pcl::PointXYZ>);
  _cloud_smoothed.reset(new pcl::PointCloud<pcl::PointXYZ>);
  _normals.reset(new pcl::PointCloud<pcl::Normal>);
  _tree.reset(new pcl::search::KdTree<pcl::PointXYZ>);
  _point_normals.reset(new pcl::PointCloud<pcl::PointNormal>);


  if ( geom.size() > 3 ){

    // Remember this is buffered!
    for (int i=0; i < geom.numElements(); ++i){
      VertPF tv = geom.getVertex(i);
      pcl::PointXYZ pos(tv.mP.x,tv.mP.y,tv.mP.z);
      _cloud->points.push_back(pos);
    }

    savePCL("./data/test.pcl");

    // Statistical removal
    pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
    sor.setInputCloud (_cloud);
    sor.setMeanK (_mean_k);
    sor.setStddevMulThresh (_std_dev);
    sor.filter (*(_cloud_filtered));

    MovingLeastSquaresOMP<PointXYZ, PointXYZ> mls;
    mls.setNumberOfThreads(6);
    mls.setInputCloud (_cloud_filtered);
    mls.setSearchRadius (_radius);
    mls.setPolynomialFit (true);
    mls.setPolynomialOrder (_order);
    mls.setUpsamplingMethod (MovingLeastSquares<PointXYZ, PointXYZ>::SAMPLE_LOCAL_PLANE);
    mls.setUpsamplingRadius (_upsampling);
    mls.setUpsamplingStepSize (_step);
    mls.process (*(_cloud_smoothed));

    NormalEstimationOMP<PointXYZ, Normal> ne;
    ne.setNumberOfThreads (8);
    ne.setInputCloud (_cloud_smoothed);
    ne.setRadiusSearch (_normal_radius);

    Eigen::Vector4f centroid;
    compute3DCentroid (*_cloud_smoothed, centroid);
    ne.setViewPoint (centroid[0], centroid[1], centroid[2]);
    ne.compute (*_normals);

    /*    for (size_t i = 0; i < cloud_normals->size (); ++i) {
      cloud_normals->points[i].normal_x *= -1;
      cloud_normals->points[i].normal_y *= -1;
      cloud_normals->points[i].normal_z *= -1;
    }*/

    concatenateFields (*_cloud_smoothed, *_normals, *_point_normals);

    cout << "Beginning Poisson Creation" << endl;

    pcl::Poisson<pcl::PointNormal> pp;
    pp.setInputCloud(_point_normals);
    pp.setDepth(_poisson_depth);
    //pp.setOutputPolygons(true);
    pp.setSamplesPerNode(_poisson_samples);
    pp.setScale(_poisson_scale);
    pp.reconstruct (_mesh);

  }
}

void Mesher::savePCL(std::string filename){
  _cloud->width = _cloud->points.size(); 
  _cloud->height = 1;
  pcl::io::savePCDFileASCII (filename.c_str(),  *(_cloud));
  std::cerr << "Saved " <<  _cloud->points.size () << " data points to " << filename << std::endl;
}


void Mesher::loadPCL(std::string filename){
  if (pcl::io::loadPCDFile<pcl::PointXYZ> (filename.c_str(), *(_cloud)) == -1) {
    cerr << "Could NOT load PCD file " <<  filename << endl;
    return;
  }  
}
