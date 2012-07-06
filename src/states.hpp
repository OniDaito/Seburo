/**
* @brief States for the Leeds Main Program
* @file states.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 07/06/2012
*
*/


#ifndef _STATES_HPP_
#define _STATES_HPP_


#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/shared_ptr.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera_manager.hpp"
#include "drawer.hpp"
#include "mesh.hpp"
#include "projector_window.hpp"


/*
 * Struct for the information each state needs
 */
 
class StateInfo{
public:
	StateInfo(CameraManager &p0, Drawer &p1, LeedsMesh &p2, ProjectorWindow &p3) : c(p0), d(p1), m(p2), p(p3) { status = "Leeds - State Enabled"; };
	
	std::string getStatus() { boost::lock_guard<boost::mutex> lock(mutex); std::string r = status; return r;};
	void updateStatus(std::string r) {   boost::lock_guard<boost::mutex> lock(mutex);  status = r; };
	
	CameraManager &c;
	Drawer &d;
	LeedsMesh &m;
	ProjectorWindow &p;
	
	int mx,my,dx,dy;	// Mouse positions
	bool ml, mr, mm;	// button down
	double dt;			// delta time
	bool sr;	// Draw Results to the screen, whatever they are? - should only be read by stack states as this is global for all current states
	
protected:
	std::string status;	// Protected as this is written and wrote to by different threads
	boost::mutex mutex;

}; 

typedef boost::shared_ptr<StateInfo> SharedInfo;


 /*
 * Small class that deals with state
 * Only one state can run and draw at once. Some states cannot be paused as they launch threads
 * http://www.boost.org/doc/libs/1_49_0/libs/ptr_container/doc/examples.html
 * Defined here as a class that exports functionality and is to be extended
 */
 

class LeedsState : boost::noncopyable {
public:
	
	LeedsState( const LeedsState& r ) { mID = r.mID; mW = r.mW; mR = r.mR; mF = r.mF; mI = r.mI; };
    void operator=( const LeedsState& );

	LeedsState(SharedInfo info) { mW = true; mR =true; mF = false; mI = info; };
	
	virtual ~LeedsState() { } // Called when destroyed
	
	LeedsState* clone() const { return do_clone(); }
	
	// Functionality exported that we may have control over
	virtual void update() = 0;
	virtual void draw() = 0;
	
	bool mW; // is this state  pausable?
	bool mR; // is this state removeable?
	bool mF; // is this state finished?
	
	std::string mID;

protected:
	SharedInfo mI;

private:
	
	virtual LeedsState* do_clone() const = 0;
	
 };
 
 
/*
 * Handy Typedefs
 */

typedef typename boost::ptr_list<LeedsState>::iterator state_iterator;
typedef typename boost::ptr_deque<LeedsState> LeedsQueue;
typedef typename boost::ptr_list<LeedsState> LeedsList;

state_iterator hasState( boost::ptr_list< LeedsState >  &l, std::string type);
 

/*
 * Initial base state for this program
 */
 
class BaseState : public LeedsState {
public:
	BaseState(SharedInfo info) : LeedsState(info) { mID = "BaseState"; mR = false; mF = false; info->p.setFlash(false);};
	virtual LeedsState* do_clone() const { return new BaseState( *this ); }
	void operator ()();
 	void update() { mI->updateStatus("Leeds - Ready"); };
	void draw();
};
 
 
/*
 * Leeds state for camera calibration
 */
 
class StateCalibrateCamera : public LeedsState {
public:

	virtual LeedsState* do_clone() const { return new StateCalibrateCamera( *this ); }

	StateCalibrateCamera(SharedInfo info) : LeedsState(info) { mID = "StateCalibrateCamera"; mW = true; mG = false; mR = false; };
	void update();
	void draw();
	
	bool mG;
};

 
/*
 * Leeds state for world calibration
 */
 
class StateCalibrateWorld : public LeedsState {
public:

	virtual LeedsState* do_clone() const { return new StateCalibrateWorld( *this ); }

	StateCalibrateWorld(SharedInfo info) : LeedsState(info) { mID = "StateCalibrateWorld"; mW = true; mG = false; mR = false; };
	void update();
	void draw();
	
	bool mG;
};



/*
 * ListState provides extra functions to the state in order to remove itself from a list
 */

template<class LeedsState> class ListState : public LeedsState {
public:	
	ListState(LeedsList  &list, SharedInfo info) : LeedsState(info), mL(list)  { };
	void operator ()() {
		if (hasState(mL, this->mID) == mL.end()){
			mL.push_back( this->clone() );
		
		}
	}
	
	bool remove() { 
		if (this->mR) {
			state_iterator it = hasState(this->mL, this->mID); 
			if (it != mL.end()){
				mL.release(it);
				return true;
					
			}
		}
		return false;
	}
	
protected:

	LeedsList  &mL;
};

/*
 * StackState provides extra functions to the state in order to remove itself from a deque
 */

template<class LeedsState> class StackState : public LeedsState {
public:

	StackState( LeedsQueue  &d, SharedInfo info) : LeedsState(info), mD(d)  {  };

	void operator ()() {
		if (mD.size() > 0){
			if (mD.back().mW){
				mD.push_back( this->clone() );
			}
		}
		else
			mD.push_back( this->clone() );
	}
	
	// Allow removal of this state if its on the top of the stack and can be removed
	bool remove() { 
		if (this->mR && this->mID == mD.back().mID){
			mD.pop_back();
			return true;
		}
		return false;
	}
	
	LeedsQueue &mD;
};



/*
 * Leeds state for displaying the cameras
 */

class StateDisplayCameras : public LeedsState  {
public:
	StateDisplayCameras(SharedInfo info) : LeedsState(info) { mID = "StateDisplayCameras"; };
	virtual LeedsState* do_clone() const { return new StateDisplayCameras( *this ); }
	
	void update();
	void draw();
};

/*
 * Leeds state for drawing the mesh
 */

class StateDrawMesh : public LeedsState {
public:
	StateDrawMesh(SharedInfo info) : LeedsState(info) {  mID = "StateDrawMesh"; info->p.setFlash(true); };
	~StateDrawMesh() { mI->p.setFlash(false);};
	virtual StateDrawMesh* do_clone() const { return new StateDrawMesh( *this ); }
	void update() {};
	void draw();
};
 


/*
 * State Scanning
 */
 
class StateScan : public LeedsState {
public:
	StateScan(SharedInfo info) : LeedsState(info) { mID = "StateScan"; mW = false; mHoriz =0; mVert =0; }
	virtual StateScan* do_clone() const { return new StateScan( *this ); };
	void update();
	void draw();
	
	glm::mat4 mViewMatrix;
	float_t mHoriz, mVert;
	
};

/*
 * State Texturing
 */
 
 
class StateTexturing : public LeedsState {
public:
	StateTexturing(SharedInfo info) : LeedsState(info) { mID = "StateTexturing"; mW = false;  }
	~StateTexturing();
	virtual StateTexturing* do_clone() const { return new StateTexturing( *this ); };
	void draw();
	void update();
		
};


/*
 * State Tool View
 */
 
 
class StateToolView : public LeedsState {
public:
	StateToolView(SharedInfo info) : LeedsState(info) { mID = "StateToolView"; mW = false;  }
	~StateToolView(){};
	virtual StateToolView* do_clone() const { return new StateToolView( *this ); };
	void draw();
	void update();
		
};




#endif
