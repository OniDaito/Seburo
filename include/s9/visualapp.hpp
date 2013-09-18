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

	class OPENGLCOURSEAPI WindowResponder {
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

	class OPENGLCOURSEAPI WindowApp  {
	public:
		virtual void init() {};
		virtual void display(double_t dt) {};
		virtual void update(double_t dt) {};
		virtual void shutdown() {};

		void fireEvent(Event e);
		void fireEvent(MouseEvent e);
		void fireEvent(ResizeEvent e);
		void fireEvent(KeyboardEvent e);

		void link(WindowResponder &responder) { 
			boost::shared_ptr<WindowResponder> p (&responder);
			_listeners.push_back(p);
		};

	protected:
		std::vector< boost::shared_ptr<WindowResponder> > _listeners;

	};

	/*
	 * Visual System is implemented by the underlying system
	 * in this case, GLFW. Sends events to the Windowed app
	 */

	class OPENGLCOURSEAPI WindowSystem {
	public:
		WindowSystem(WindowApp& app) : _app(app) { };

	protected: 
		
		WindowApp &_app;
	};


}

#endif
