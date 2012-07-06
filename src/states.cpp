/**
* @brief States for the Leeds Main Class
* @file drawer.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 07/06/2012
*
*/

#include "states.hpp"

using namespace std;
using namespace boost;



//LeedsState* new_clone( const LeedsState& a ) { return a.clone(); }

/*
 * Function that returns the iterator for a given state
 */
///\todo why cant we use the typedef for the list?
state_iterator hasState( boost::ptr_list< LeedsState >  &l, std::string type) {
	for( state_iterator i = l.begin();  i != l.end(); ++i){
        if( i->mID == type )
            return i;
    }
    return l.end();
}


/*
 * Base State
 */
 
void BaseState::draw(){
//	if (mI->ml)
//		mI->d.updateCamera(mI->dx, mI->dy, mI->dt);
	//mI->p.setFlash(false);
	mI->d.drawReferenceQuad();
	mI->d.drawMeshPoints(mI->m.getPointsVBO(),0.2,0.2,0.9);
	
	// Calculate the gripper position from pInfo
	mI->d.drawGripper();
	
	BOOST_FOREACH (boost::shared_ptr<LeedsCam> cam, mI->c.getCams()) {
		mI->d.drawNormals(cam->getVBO());
	}
	

} 


///\ todo - some states cannot be removed OR need cleanup. Do cleanup as a destructor in LeedsState

/*
 * Display Cameras update method
 */

void StateDisplayCameras::update() { }

/*
 * Display Cameras Draw Method 
 */

void StateDisplayCameras::draw() { mI->c.updateTextures(); mI->d.drawCamerasFlat(mI->c.getCams());  }



/*
 * Calibrate the Cameras update method
 */
 
void StateCalibrateCamera::update() {
	// try to pop if the mManager is finished
	if (!mG){
		mG = true;
		mI->c.calibrateCameras();
	}
	
	else if (mG && !mI->c.isThreading()){
		mF = true; // We can remove this now
	}
	
	 mI->updateStatus("Leeds - Calibrating Camera"); 
}

/*
 * Calibrate the Camera draw method
 */
 
void StateCalibrateCamera::draw(){
	mI->c.bind();
	mI->d.drawCameraQuad(); 
	mI->c.unbind();
}


/*
 * Calibrate the World update method
 */
 
void StateCalibrateWorld::update() {
	// try to pop if the mManager is finished

	if (!mG){
		mG = true;
		mI->c.calibrateWorld();
	}
	
	else if (mG && !mI->c.isThreading()){
		mF = true;
	}	
	
	std::stringstream Num;
	std::string str;
	Num << mI->c.waitingOn();
	
	str = "Leeds - Calibrating World - Waiting on " + Num.str();
	mI->updateStatus(str);

}


/*
 * Calibrate the World draw method
 */
 
void StateCalibrateWorld::draw(){
	mI->c.bind();
	mI->d.drawCameraQuad(); 
	mI->c.unbind();
		
	BOOST_FOREACH (boost::shared_ptr<LeedsCam> cam, mI->c.getCams()) {	
		cam->computeNormal();
	}
	
}


/*
 * State for scanning update method
 */

void StateScan::update(){	
	//mI->p.setFlash(false);
}

/*
 * State for scanning draw method
 */
 
void StateScan::draw(){
//	if (mI->ml)
//		mI->d.updateCamera(mI->dx, mI->dy, mI->dt);
	// Called here as the mesh scan has OpenGL calls
	
	///\todo really we should put scanning and detecting in update!
	
	vector<std::pair<cv::Point2f,CameraParameters > > points;


	BOOST_FOREACH (boost::shared_ptr<LeedsCam> cam, mI->c.getCams()) {	
		cv::Point2f p;	
		cv::Mat t = cam->getImageRectified();
		if (mI->c.detectPoint(t, cam->getResult(), p)){
			points.push_back( std::pair<cv::Point2f,CameraParameters >(p,cam->getParams()) );
		}
		
	}
	
	// If we are drawing results to the screen update textures and draw
	if ( mI->sr){
		mI->c.updateResults();
		int idx = 0;
		
		BOOST_FOREACH (boost::shared_ptr<LeedsCam> cam, mI->c.getCams()) {
			cam->bindResult();
			mI->d.drawResultFlat(idx);
			cam->unbind();
			idx++;
		}
	}
	

	// Solve for all these points -  need at least 2
	if (points.size() > 1){
		cv::Point3f result = mI->c.solveForAll(points);
		mI->m.addPoint(result.x,result.y,result.z);
	}
	
	mI->d.drawReferenceQuad();
	
	// Now draw -  sending the camera view
	mI->d.drawMeshPoints(mI->m.getPointsVBO(),0.1,0.1,1.0);
	
	//mI->m.generate();
}

/*
 * Draw the mesh if we have one
 */

void StateDrawMesh::draw() {
	mI->c.updateTextures();
	mI->d.drawMesh(mI->m.getMeshVBO(), mI->c.getCams());
//	mI->d.drawNormals(mI->m.getNormalsVBO());
	
//	mI->d.drawMeshPoints(mI->m.getPointsFilteredVBO(),1.0,0.1,0.1);
//	mI->d.drawNormals(mI->m.getComputedNormalsVBO());

}

/*
 * Texturing state - flashes the window and performs texture updates
 */
 
void StateTexturing::draw() {
	mF = true; // Remove, only run once
}


void StateTexturing::update() {
		
}

StateTexturing::~StateTexturing() {
	mI->p.setFlash(false);
}


/*
 * Tool View State
 */
 
  
void StateToolView::draw() {
	mI->d.drawToolView(mI->m.getMeshVBO(),mI->c.getCams());
}


void StateToolView::update() {
	
}
