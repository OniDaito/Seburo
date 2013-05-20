/**
* @brief GLFW Bit
* @file visual_app.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 07/08/2012
*
*/

#include "s9/visualapp.hpp"

using namespace s9;
using namespace boost;
using namespace std;


void WindowApp::fireEvent(Event e){
  BOOST_FOREACH(boost::shared_ptr<WindowResponder> p, _listeners){
    p->processEvent(e);
  }
}

void WindowApp::fireEvent(MouseEvent e){
   BOOST_FOREACH(boost::shared_ptr<WindowResponder> p, _listeners){
    p->processEvent(e);
  }
}

void WindowApp::fireEvent(ResizeEvent e){
   BOOST_FOREACH(boost::shared_ptr<WindowResponder> p, _listeners){
    p->processEvent(e);
  }
}

void WindowApp::fireEvent(KeyboardEvent e){
   BOOST_FOREACH(boost::shared_ptr<WindowResponder> p, _listeners){
    p->processEvent(e);
  }

}
