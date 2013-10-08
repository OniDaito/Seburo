/**
* @brief Cross Platform File Handling
* @file file.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 06/03/2013
*
*/

#include "s9/file.hpp"

#include "s9/utils.hpp"

using namespace std;

using namespace s9;

File::File() {
  _finalPath = ".";
}


File::File(std::string path){
  if ( path.find("/") == 0 ){
    // Absolute path

  } 
  else if (path.find(".") == 0){
    // relative path - setup the data dir
#ifdef _SEBURO_OSX 
    _finalPath = "../Resources/" + ltrim(path);
#endif

#ifdef _SEBURO_LINUX
    _finalPath = "../../../" + path;
#endif

  }
  else {
    // error - malformed! - hard quit - compiler error
  }
}