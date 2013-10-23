/**
* @brief An Application that draws to the screen at some point
* @file visualapp.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef S9_VISUALAPP_HPP
#define S9_VISUALAPP_HPP

#include "common.hpp"
#include "events.hpp"

/*
 * The main Application interface that gl/directx applications serve to
 * \todo need to pass timing information over as well I suspect
 */
 
namespace s9 {

	/*
	 * WindowResponder class responds to window events given by the
	 * windowing system. Primtitives and applications can choose to
	 * respond to these. This is a basic interface
	 */

	class SEBUROAPI WindowResponder {
	public:
		virtual void processEvent(Event e) {};
		virtual void processEvent(MouseEvent e) {};
		virtual void processEvent(ResizeEvent e) {};
		virtual void processEvent(KeyboardEvent e) {};
		
	};

	class WindowSystem;

	/*
	 * Window App can also hear events but in addition, gets display
	 * and can also link things back to the underlying event system
	 */

	class SEBUROAPI WindowApp  {
	public:
		virtual void init() {};
		virtual void display(double_t dt) {};
		virtual void update(double_t dt) {};
		virtual void shutdown() {};

		template<class E>
		void fireEvent(E e){
  		for(WindowResponder* p : listeners_){
    		p->processEvent(e);
  		}
  	}

		void addWindowListener(WindowResponder *responder) { 
			listeners_.push_back(responder);
		};

		void removeWindowListener(WindowResponder *responder) { 
			for (std::vector<WindowResponder*>::iterator it = listeners_.begin(); it != listeners_.end(); ) {
				if  (*it == responder){
					listeners_.erase(it);
				} else {
					++it;
				}
			}
		};

	protected:
		std::vector< WindowResponder* > listeners_;

	};

	/*
	 * Visual System is implemented by the underlying system
	 * in this case, GLFW. Sends events to the Windowed app
	 */


	class SEBUROAPI WindowSystem {
	public:
		WindowSystem(WindowApp& app) : _app(app) { };

	protected: 
		
		WindowApp &_app;
	};


}

#endif
