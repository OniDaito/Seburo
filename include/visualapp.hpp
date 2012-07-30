/**
* @brief An Application that draws to the screen at some point
* @file visualapp.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef VISUALAPP_HPP
#define VISUALAPP_HPP

#include "s9gear.hpp"
#include "common.hpp"


/*
 * The main Application interface that gl/directx applications serve to
 * \todo need to pass timing information over as well I suspect
 */
 
namespace s9 {


	class VisualApp {
	public:
		VisualApp(){};

		virtual void init() = 0;
		virtual void display(double_t dt) = 0;
		virtual void fireEvent(Event e){};
		virtual void fireEvent(MouseEvent e){};
		virtual void fireEvent(ResizeEvent e){};
		virtual void fireEvent(KeyboardEvent e){};
	};
}

#endif
