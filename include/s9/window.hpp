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
	 * 
	 */

	template < class Window >
	class SEBUROAPI WindowListener {
	public:
		virtual void ProcessEvent( const Window &w, Event e) {};
		virtual void ProcessEvent( const Window &w, MouseEvent e) {};
		virtual void ProcessEvent( const Window &w, ResizeEvent e) {};
		virtual void ProcessEvent( const Window &w, KeyboardEvent e) {};
		virtual void ProcessEvent( const Window &w, CloseWindowEvent e) {};
		
	};


	/**
	 * WindowManager 
	 * A class that creates windows of various kinds and provides events
	 * If your class inherits from WindowListener, you can create a Manager that will
	 * report back to you.
	 */

	template < class Window >
	class SEBUROAPI WindowManager {
	public:
	
		WindowManager() { }

		void AddListener(WindowListener<Window> * listener) {
			listeners_.push_back(listener);
		}

		size_t NumWindows() { return windows_.size(); }

		const Window & CreateWindow(const char * title, size_t width, size_t height) {
			windows_.push_back( std::unique_ptr<Window> (new Window(title, width, height)) );
			return *(windows_.back());
		}

		// Fire Events that are picked up by Window Listeners
		template<class E>
		void FireEvent(const Window &t, E e){
			for(WindowListener<Window> * p : listeners_){
     		p->ProcessEvent(t,e);
   		}
  	}

		void DestroyWindow(const Window &window) {
			for (auto it = windows_.begin(); it != windows_.end();) {
				if ( &(**it) == &window){
	    		it = windows_.erase(it);
				} else{
					++it;
				}
			}
		}


	protected:

		std::vector< WindowListener<Window> * > listeners_;
		std::vector< std::unique_ptr<Window> > windows_;

	};


}

#endif
