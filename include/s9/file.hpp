/**
* @brief Cross Platform File Sharing
* @file file.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 06/03/2013
*
*/

#ifndef S9_FILE_HPP
#define S9_FILE_HPP

#include "common.hpp"

namespace s9 {

  class SEBUROAPI File {
  public:
    File();

    File(std::string path);

    std::string path() { return _finalPath; };

  protected:
    std::string _finalPath;


  };

}

#endif