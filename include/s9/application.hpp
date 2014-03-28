/**
* @brief An Application class that contains an update thread and can potentially launch windows
* @file application.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 25/03/2014
*
*/


#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "common.hpp"
#include "window.hpp"

#include <thread>
#include <ctime>
#include <chrono>

#ifdef USE_ANTTWEAKBAR
#include <AntTweakBar/AntTweakBar.h>
#endif


namespace s9 {


  // Application is the class one should derive from. It provides both a threaded update loop
  // and a main, OpenGL Context Loop.

  class SEBUROAPI Application {

  public:
    Application();
    ~Application();

    virtual void ThreadMainLoop(double_t dt) {} // To be overridden.
    virtual void MainLoop(double_t dt) {} // To be overridden.

    void Run();
    void Shutdown();

  protected:

    void ThreadMainLoopInternal();
    void MainLoopInternal();

    std::thread   *update_thread_;
    bool running_;

    // Clocks
    std::chrono::time_point< std::chrono::high_resolution_clock>  thread_start_;
    std::chrono::time_point< std::chrono::high_resolution_clock>  main_start_;

  };
}

#endif