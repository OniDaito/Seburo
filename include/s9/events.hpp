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
#include "context.hpp"


///\todo how does this tie in to GLFW and such? We have some good defines there already!

namespace s9{

	typedef enum {
		EVENT_MOUSE,
		EVENT_KEY,
		EVENT_RESIZE,
		EVENT_SCROLL,
		EVENT_CLOSE_WINDOW
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

		friend std::ostream& operator<<(std::ostream& out, const Event& o);

		EventType type;
		double_t t;
		const Context context;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& obj) {
		 return os << "Event - Type: " << obj.type << ", Time: " << obj.t;
	}

	struct MouseEvent : public Event {
		MouseEvent(const Context c, int xp, int yp, uint16_t flagp, double_t tp=0 ) {
			flag = flagp;
			x = xp;
			y = yp;
			type = EVENT_MOUSE;
			t = tp;
			context = c;
		}

		uint16_t flag;
		int x, y;
	}; 

	struct KeyboardEvent : public Event {
		KeyboardEvent(const Context c, int keyp, int actionp, double_t tp=0){
			key = keyp;
			action = actionp;
			type = EVENT_KEY;
			t = tp;
			context = c;
		}
		int key;
		int action;
	};

	struct ResizeEvent : public Event {
		ResizeEvent(const Context c, size_t wp, size_t hp, double_t tp=0){
			w = wp;
			h = hp;
			t = tp;
			type = EVENT_RESIZE;
			context = c;
		}

		size_t w,h;
	};

	struct ScrollEvent : public Event {
		ScrollEvent (const Context c, double_t xoffset, double_t yoffset, double_t tp=0) {
			xd = xoffset;
			yd = yoffset;
			t = tp;
			type = EVENT_SCROLL;
			context = c
		}

		double_t xd,yd;
	};

	struct CloseWindowEvent : public Event {
		CloseWindowEvent(const Context c, double_t tp=0 ){
			t = tp;
			type = EVENT_CLOSE_WINDOW;
			context = c;
		}
	};
}

#endif