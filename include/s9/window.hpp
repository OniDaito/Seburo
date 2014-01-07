/**
* @brief An Application that draws to the screen at some point
* @file visualapp.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef S9_WINDOW_HPP
#define S9_WINDOW_HPP

#include "common.hpp"
#include "events.hpp"


 
namespace s9 {

	/**
	 * WindowListener class responds to window events given by the
	 * windowing system. Primtitives and applications can choose to
	 * respond to these. This is a basic interface
	 */

	
	template<class T>
	class SEBUROAPI WindowListener {
	public:
		virtual void ProcessEvent(Event e, T w) {};
		virtual void ProcessEvent(MouseEvent e, T w) {};
		virtual void ProcessEvent(ResizeEvent e, T w) {};
		virtual void ProcessEvent(KeyboardEvent e, T w) {};
		virtual void ProcessEvent(CloseWindowEvent e, T w) {};
		
	};


	template<class T>
	class WindowApp : public WindowListener<T> {
	public:
		virtual void Init() {}
		virtual void Update(double_t dt) {}
		virtual void Display(T window, double_t dt) {}
	
	};

	template<class T>
	class WindowSystem {

	public:
	
		virtual void 	Run() = 0;
		virtual T 		CreateWindow(const char * title, size_t width, size_t height) = 0;
		virtual void 	CloseWindow(T window) = 0;
		virtual void 	Shutdown() = 0;

	};

	
	/**
	 * WindowEventor - The class we generally want to inherit from
	 * Windows are generic and are controlled by WindowSystem. They launch events
	 * that applications are aware of. They have functions that the Window System calls
	 */
	
	template<class T>
	class SEBUROAPI WindowEventor {
	
	public:
		
		void AddWindowListener(WindowListener<T> *responder) { 
			listeners_.push_back(responder);
		}

		void RemoveWindowListener(WindowListener<T> *responder) { 
			// Cx the use of auto - might be cheating :D
			for (auto it = listeners_.begin(); it != listeners_.end(); ) {
				if  (*it == responder){
					listeners_.erase(it);
				} else {
					++it;
				}
			}
		}
			
		// Fire Events that are picked up by Window Listeners
		template<class E>
		void FireEvent(E e, T t){
  		for(WindowListener<T> * p : listeners_){
    		p->ProcessEvent(e, t);
  		}
  	}

  protected:
		std::vector< WindowListener<T>* > listeners_;

	};


}

#endif
