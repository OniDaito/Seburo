/**
* @brief General Context
* @file context.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 25/03/2014
*
*/

#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "common.hpp"

namespace s9 {

  /// This class is an interface that decouples OpenGL / etc from the actual Windowing Context
  class Context {
  
  public:

    Context() {};

    const int major_version() const { return obj_->major_version; }
    const int minor_version() const { return obj_->minor_version; }
    const std::string shader_version() const  { return obj_->shader_version; }

  protected:

    Context(bool b) : obj_(std::shared_ptr<SharedObject>(new SharedObject())) {};

    struct SharedObject {
        int major_version;
        int minor_version;
        std::string shader_version;
        std::string version_string;
    };

    std::shared_ptr<SharedObject> obj_ = nullptr;

  public:

    bool operator == (const Context &ref) const { return this->obj_ == ref.obj_; }
    typedef std::shared_ptr<SharedObject> Context::*unspecified_bool_type;
    operator unspecified_bool_type() const { return ( obj_.get() == 0 ) ? 0 : &Context::obj_; }
    void reset() { obj_.reset(); }


  };
  
}


#endif