/**
* @brief GLFW Window based solution
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#include "app.hpp"


#include <boost/program_options.hpp>
#include <signal.h>

using namespace std;
using namespace boost;
using namespace boost::assign;
using namespace s9;
using namespace s9::gl;

namespace po = boost::program_options;

/*
 * Called as fast as possible. Not set FPS wise
 */
		
void BasicApp::display(){

}


/*
 * This is called by the wrapper function when an event is fired
 */

void BasicApp::fireEvent(Event e){

}
		

int main (int argc, const char * argv[]) {
  
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "S9Gear Basic Application - No Options")
    ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }
  
    BasicApp b;

  	GLFWApp a(&b);
  	a.init(4,0); 

    return EXIT_SUCCESS;

}