/**
* @brief GLFW Bit
* @file Application.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 25/03/2014
*
*/

#include "s9/application.hpp"

using namespace s9;

Application::Application() {
  running_ = false;
}

Application::~Application(){
  Shutdown();
}


void Application::Run() {

  running_ = true;
  thread_start_= std::chrono::high_resolution_clock::now();
  main_start_= std::chrono::high_resolution_clock::now();
  update_thread_ = new std::thread(&Application::ThreadMainLoopInternal, this);

  MainLoopInternal();

}

void Application::Shutdown() {
  running_ = false;


}

void Application::ThreadMainLoopInternal(){

  while(running_){
    auto now = std::chrono::high_resolution_clock::now();
    double_t ss = std::chrono::duration_cast<std::chrono::seconds>(now - thread_start_).count();
    ThreadMainLoop(ss);
    thread_start_ = now;
  }
}

void Application::MainLoopInternal(){

  while(running_){
    auto now = std::chrono::high_resolution_clock::now();
    double_t ss = std::chrono::duration_cast<std::chrono::seconds>(now - main_start_).count();
    MainLoop(ss);
    main_start_ = now;
  }
}

