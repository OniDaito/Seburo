/**
* @brief An Application that draws to the screen at some point
* @file visualapp.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef S9_STATE_HPP
#define S9_STATE_HPP

#include "common.hpp"

namespace s9{

  ///\todo mesh this properly with the event system 
  /// ie shared objects may listen in one state and not another
  class State;

  typedef State* StateInternalPtr;
  typedef boost::shared_ptr<State> StatePtr;

  /*
   * State class. Basically a linked list with overriden
   * methods and blocking, active flags. Initially tried
   * shared_ptr but no need really
   */

  class SEBUROAPI State {

  protected:
    StateInternalPtr _next;
    StateInternalPtr _prev;
    boost::shared_ptr<void> _data;

    bool _skip,_blocking,_active;

    virtual void _update(double_t dt){}
    virtual void _draw(double_t dt){};
  
  public:
    State(boost::shared_ptr<void> a){ 
      _data = a;
      _blocking = false;
      _active = false;
      _prev = NULL;
      _next = NULL;
    };
    
    /*
     * Add a state into the chain
     */

    void add(StatePtr s){
      if (!_blocking){
        if (_next != NULL) {
          _next->_prev = s.get();
          s->_next = _next;
        }
        _next = s.get();
        s->_prev = this;
        s->_active = true;
      }
    }

    /*
     * Remove a state from the chain
     */

    void remove() {
      if (_prev != NULL){
        if (_next != NULL){
          _prev->_next = _next;
          _next->_prev = _prev;
          _next = NULL;
        }
        else {
          _prev->_next = NULL;
        }
        _prev = NULL;
      }

      
      // this is therefore the first state so fail
      if (_next != NULL){
        std::cerr << "Seburo - FATAL - Attempted to remove first state" << std::endl;
        assert(false);
      }
      _active = false;
    }

    bool isActive() {return _active; };

    void update(double_t dt) {
      if (_active) _update(dt);
      
      if (_blocking) return;
      
      if (_next != NULL )
          _next->update(dt);
    }

    void setActive(bool b) { _active = b; };
    void setBlocking(bool b) { _blocking = b; };

    void draw(double_t dt) { 
      if (_active) _draw(dt);

      if (_blocking) return;

      if (_next !=NULL )
        _next->draw(dt);
      
    }
  };



}

#endif