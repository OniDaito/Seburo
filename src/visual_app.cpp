/**
* @brief GLFW Bit
* @file visual_app.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 07/08/2012
*
*/

#include "s9/visualapp.hpp"

using namespace s9;
using namespace std;


void WindowApp::fireEvent(Event e){
  for(std::shared_ptr<WindowResponder> p : _listeners){
    p->processEvent(e);
  }
}

void WindowApp::fireEvent(MouseEvent e){
   for(std::shared_ptr<WindowResponder> p : _listeners){
    p->processEvent(e);
  }
}

void WindowApp::fireEvent(ResizeEvent e){
   for(std::shared_ptr<WindowResponder> p : _listeners){
    p->processEvent(e);
  }
}

void WindowApp::fireEvent(KeyboardEvent e){
   for(std::shared_ptr<WindowResponder> p : _listeners){
    p->processEvent(e);
  }

}
