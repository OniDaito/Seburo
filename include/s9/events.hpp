/**
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "common.hpp"


namespace s9{

	typedef enum {
		EVENT_MOUSE,
		EVENT_KEY,
		EVENT_RESIZE,
	}EventType;

	typedef enum{
		MOUSE_LEFT_DOWN = 0x01,
		MOUSE_RIGHT_DOWN = 0x02,
		MOUSE_MIDDLE_DOWN = 0x04,
		MOUSE_WHEEL_UP = 0x08,
		MOUSE_WHEEL_DOWN = 0x10,
		MOUSE_LEFT_UP = 0x20,
		MOUSE_RIGHT_UP = 0x40,
		MOUSE_MIDDLE_UP = 0x80
	}MouseAction;

	struct Event {
		EventType mType;
		double_t mT;
	};

	struct MouseEvent : public Event {
		MouseEvent(int x, int y, uint16_t flag, double_t t=0 ) {
			mFlag = flag;
			mX = x;
			mY = y;
			mType = EVENT_MOUSE;
			mT = t;
		}

		uint16_t mFlag;
	
		int mX, mY;
	}; 

	struct KeyboardEvent : public Event {
		KeyboardEvent(int key, int action, double_t t=0){
			mKey = key;
			mAction = action;
			mType = EVENT_KEY;
			mT = t;
		}
		int mKey;
		int mAction;
	};

	struct ResizeEvent : public Event {
		ResizeEvent(size_t w, size_t h, double_t t=0){
			mW = w;
			mH = h;
			mT = t;
			mType = EVENT_RESIZE;
		}

		size_t mW,mH;
	};
}

#endif