/**
* @brief Camera Manager that deals with the control of the 8 cameras
* @file camera_manager.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/05/2012
*
*/

#include "camera_manager.hpp"

using namespace std;
using namespace cv;
using namespace boost::assign; 

/*
 * Constructor for the LeedsCam - initialise transforms and similar
 */

LeedsCam::LeedsCam(UVCVideo &cam, Size size) : mCam(cam) {
	
	// Initialise Matrices
	mImage = Mat(size, CV_8UC3);
	mImageRectified = Mat(size, CV_8UC3);
	mResult = Mat(size,CV_8UC3);
	
	// Initialise texture - using GL_TEXTURE_RECTANGLE
	glGenTextures(1, &mTexID);
	glBindTexture(GL_TEXTURE_RECTANGLE, mTexID);               
	glTexParameterf(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 3, size.width, size.height, 0, GL_RGB, GL_UNSIGNED_BYTE, mCam.getBuffer());
	
	glGenTextures(1, &mTexResultID);
	glBindTexture(GL_TEXTURE_RECTANGLE, mTexResultID);               
	glTexParameterf(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 3, size.width, size.height, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *) IplImage(mResult).imageData);

	glGenTextures(1, &mRectifiedTexID);
	glBindTexture(GL_TEXTURE_RECTANGLE, mRectifiedTexID);               
	glTexParameterf(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 3, size.width, size.height, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *) IplImage(mResult).imageData);


	mVBONormal.mColours += 0.0,0.0,1.0,0.9,
					0.0,1.0,1.0,0.9,

	mVBONormal.compile(VBO_VERT | VBO_COLR);

}



/*
 * Camera update - checks the buffer and performs rectification if possible
 */

void LeedsCam::update() {
	// Update from the UVCVideo
	mImage = cv::Mat (mImage.size(), CV_8UC3, mCam.getBuffer());
		
	if (isRectified())
		undistort(mImage, mImageRectified, mP.M, mP.D);
}

/*
 * Camera update for GL Textures only. Swaps out when not needed
 */
 
 void LeedsCam::updateTexture() {

	
	
	// Update OpenGL texture - hopefully fast enough
	if (isRectified()){
		
		bindRectified();
		glTexSubImage2D(GL_TEXTURE_RECTANGLE,0,0,0,mImageRectified.size().width, 
			mImageRectified.size().height, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *) IplImage(mImageRectified).imageData );
		unbind();
	}

	bind();
	glTexSubImage2D(GL_TEXTURE_RECTANGLE,0,0,0,mImage.size().width, 
	mImage.size().height, GL_RGB, GL_UNSIGNED_BYTE, mCam.getBuffer() );		
	unbind();

}

void LeedsCam::updateResultTexture(){
	// Update any results textures - This needs to be dependent on state as well I think
	bindResult();
	glTexSubImage2D(GL_TEXTURE_RECTANGLE,0,0,0,mResult.size().width, 
			mResult.size().height, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *) IplImage(mResult).imageData );
	unbind();
}
 

/*
 * Bind the Texture for drawing
 */
 
 void LeedsCam::bind() {
	//glEnable(GL_TEXTURE_RECTANGLE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, mTexID);
 }
 
 void LeedsCam::bindRectified() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, mRectifiedTexID);
 }
 
/*
 * UnBind the Texture
 */
 
 void LeedsCam::unbind() {
	//glDisable(GL_TEXTURE_RECTANGLE);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
 }


/*
 * Bind the Texture for drawing
 */
 
 void LeedsCam::bindResult() {
	//glEnable(GL_TEXTURE_RECTANGLE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, mTexResultID);
 }

void LeedsCam::computeNormal() {
	Mat r (Size(3,3), CV_64FC1);
	Rodrigues(mP.R,r);

	r = r.inv();
	r *= -1;
	mPlaneNormal = Mat(Size(1,3), CV_64FC1);
	mPlaneNormal.at<double_t>(0,0) = 0.0;
	mPlaneNormal.at<double_t>(1,0) = 0.0;	
	mPlaneNormal.at<double_t>(2,0) = 1.0;
	mPlaneNormal = r * mPlaneNormal;
	
	
	mVBONormal.mVertices.clear();
	mVBONormal.mVertices += 0.0,0.0,0.0,
					mPlaneNormal.at<double_t>(0,0), 
					mPlaneNormal.at<double_t>(1,0), 
					mPlaneNormal.at<double_t>(2,0);
	
	mVBONormal.bind();
	mVBONormal.mNumElements = 2;
	mVBONormal.allocateVertices();
	mVBONormal.unbind();
	
	
	
}


uint8_t CameraManager::sThreads = 0;

/*
 * Camera Manager Setup Function - Shared object
 */
 
 void CameraManager::setup(GlobalConfig &config){
	mObj.reset(new SharedObj(config));
	
	mObj->mResult = Mat(config.camSize,CV_8UC3);
	
	// Now create a texture for this
	glGenTextures(1, &mObj->mTexID);
	glBindTexture(GL_TEXTURE_RECTANGLE, mObj->mTexID);               
	glTexParameterf(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_RECTANGLE,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S , GL_REPEAT );
	glTexParameterf( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 3, config.camSize.width, config.camSize.height, 
		0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *) IplImage(mObj->mResult).imageData);
 
 }

/*
 * Update the individual cameras and similar
 */

void CameraManager::update() {
	for (vector< boost::shared_ptr<LeedsCam> >::iterator it = mObj->mCams.begin(); it != mObj->mCams.end(); it++){
		boost::shared_ptr<LeedsCam> l = *it;
		l->update();
	}
	
	// update result texture - this is done here because we should have all OpenGL calls on the same thread
	bind();
	glTexSubImage2D(GL_TEXTURE_RECTANGLE,0,0,0,mObj->mResult.size().width, 
		mObj->mResult.size().height, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *) IplImage(mObj->mResult).imageData );
	unbind();
}

/*
 * Update the individual cameras but just the texture. This is only needed when drawing and texturing
 */

void CameraManager::updateTextures() {
	for (vector< boost::shared_ptr<LeedsCam> >::iterator it = mObj->mCams.begin(); it != mObj->mCams.end(); it++){
		boost::shared_ptr<LeedsCam> l = *it;
		l->updateTexture();
		l->updateResultTexture();
	}
}

/*
 * Update the results for each camera if we need to
 */

void CameraManager::updateResults() {
	for (vector< boost::shared_ptr<LeedsCam> >::iterator it = mObj->mCams.begin(); it != mObj->mCams.end(); it++){
		boost::shared_ptr<LeedsCam> l = *it;
		l->updateResultTexture();
	}
}



/*
 * Create a new camera given the device name. Create UVCVideo and a wrapper around it
 */
 
boost::shared_ptr<LeedsCam> CameraManager::addCamera(std::string dev, std::string filename){
	
	boost::shared_ptr<UVCVideo> pc (new UVCVideo());
	mObj->mDevs.push_back(pc);
	pc->startCapture(dev,mObj->mConfig.camSize.width,mObj->mConfig.camSize.height,mObj->mConfig.fps);
	 
	boost::shared_ptr<LeedsCam> pv (new LeedsCam(*pc,mObj->mConfig.camSize));
	mObj->mCams.push_back(pv);
	 
	// Attempt to load parameters
	
	if (loadCameraParameters(filename, pv->getParams()) )
		pv->computeNormal();
	
	 
	return pv;
}
 
/*
 * Calibrate all the cameras - launch the thread. Takes a callback to signal finishing
 */
  
void CameraManager::calibrateCameras() {
	sThreads++; // shows we are still working
	mObj->pWorkerThread =  new boost::thread(&CameraManager::_calibrateCameras, this);
}
 
/*
 * Calibrate all the cameras, fixing their errors - Threaded actual method
 */
  
void CameraManager::_calibrateCameras() {
	for (vector< boost::shared_ptr<LeedsCam> >::iterator it = mObj->mCams.begin(); it != mObj->mCams.end(); it++){
		boost::shared_ptr<LeedsCam> l = *it;
		
		double lastCall;	
		clock_t init, final;
		init=clock();
		int numImages = 0;
		
		CalibratorCamera c(l->getParams(), mObj->mConfig.camSize, mObj->mConfig.boardSize);
		
		Mat cammat = l->getImage();
		
		while (numImages < mObj->mConfig.maxImages){
			lastCall = ((clock()- init) / (double)CLOCKS_PER_SEC);
			
			if ( lastCall > mObj->mConfig.interval) {
				if (c.addImage( cammat , mObj->mResult))
					numImages++;
					
				lastCall = 0.0;
				init = clock();
			}
		}
		
		boost::thread(c, sThreads);
	}
	sThreads--;
}
		
 
/*
 * Calibrate all the cameras with respect to the world
 */
  
void CameraManager::calibrateWorld() {
	sThreads++; // shows we are still working
	mObj->pWorkerThread =  new boost::thread(&CameraManager::_calibrateWorld, this);
}
  
void CameraManager::_calibrateWorld() {
	
	bool go = true;
	
	vector<boost::shared_ptr<CalibratorWorld> > calibrators;
	
	while (go){
		
		calibrators.clear();
				
		
		go = false;
		int idx = 0;
		for (vector< boost::shared_ptr<LeedsCam> >::iterator it = mObj->mCams.begin(); it != mObj->mCams.end(); it++){
			
			boost::shared_ptr<LeedsCam> l = *it;
			
			
			boost::shared_ptr<CalibratorWorld> p(new CalibratorWorld(l->getParams(), mObj->mConfig.camSize, mObj->mConfig.boardSize));
		
			calibrators.push_back(p );
			
			
			Mat cammat;
				
			if (l->isRectified())
				cammat = l->getImageRectified();
			else
				cammat = l->getImage();
			
			if (!p->addImage(cammat, mObj->mResult)){
				go = true;
				mObj->mWaitingOn = idx;
				break;
			}
			idx++;
			
		}
	}
	
	for (vector<boost::shared_ptr<CalibratorWorld> >::iterator it = calibrators.begin(); it != calibrators.end(); it++){
		CalibratorWorld  c = *(*it);	
		//boost::thread(c, sThreads);
		// go non threaded as its a fast enough operation and we need to know when we've finished
		c(sThreads);
	}
	calibrators.clear();
	
	sThreads--;
 }
 
 /*
  * Set a control for all the cameras
  */
  
 void CameraManager::setControl(CameraControl c, unsigned int v){
	for (vector< boost::shared_ptr<UVCVideo> >::iterator i = mObj->mDevs.begin(); i != mObj->mDevs.end(); i ++){
		(*i)->set_control((unsigned int)c,v);
	}
 }
 
 /*
  * Bind the manager texture
  */
  
void CameraManager::bind() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE, mObj->mTexID);
}


/*
 * UnBind the manager Texture
 */
 
 void CameraManager::unbind() {
	//glDisable(GL_TEXTURE_RECTANGLE);
	glBindTexture(GL_TEXTURE_RECTANGLE, 0);
 }


/*
 * Detect a point with a black and white image
 */

bool CameraManager::detectPoint(cv::Mat &data, cv::Mat &result, cv::Point2f &point){

	Mat grey = Mat(data.size(), CV_8UC1);
	Mat thresh = Mat(data.size(), CV_8UC1);
	cvtColor( data, grey, CV_RGB2GRAY );
	result = Mat::zeros(data.rows, data.cols, CV_8UC3);
	vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    bool found = false;
	//adaptiveThreshold(grey,thresh,220,ADAPTIVE_THRESH_MEAN_C,ADAPTIVE_THRESH_GAUSSIAN_C,7,1.0);
	threshold(grey, thresh, 10, 255, THRESH_BINARY);
	
	cvtColor( thresh, result, CV_GRAY2RGB );
	
	for (int i=0; i < thresh.rows; i++){
	
		for (int j=0; j < thresh.cols; j++){
			if (thresh.ptr<uint8_t>(i)[j] >= 220){
				float score = thresh.ptr<uint8_t>(i)[j];
				// We have a local area so search it  
				for (int k =-3; k <4; k ++){
					for (int l = -3; l < 4; l++){
						score += thresh.ptr<uint8_t>(i + k)[j + l];
					}
				}
				
				if ( score / 49.0 > 50) { 
					found = true;
					point = Point2f(j,i);
					circle(result, point, 20, Scalar(255,0,0),2);
					return found;
				}			
			}
		}
	}

	return found;
}

/*
 * Given a series of points and extrinsics do some maths and recreate the depth point
 */
 
cv::Point3f CameraManager::solveForAll( std::vector< std::pair< cv::Point2f, CameraParameters > > points) {
	
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


/*
 * Save settings to disk given the filenames
 */
 
void CameraManager::saveSettings(std::vector<std::string> filenames) {
	for (int i=0; i < mObj->mCams.size(); i++){
		saveCameraParameters(filenames[i], mObj->mCams[i]->getParams());
	}
}

/*
 * Shutdown all cameras
 */
 
void CameraManager::shutdown() {
	for (vector< boost::shared_ptr<UVCVideo> >::iterator it = mObj->mDevs.begin(); it != mObj->mDevs.end(); it ++){
		(*it)->stop();
	}
}
