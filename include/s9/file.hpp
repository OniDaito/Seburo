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
#include "string_utils.hpp"

#include <sys/stat.h>


namespace s9 {

  class SEBUROAPI File {
  public:
    File();
    File(std::string path);

    std::string path() const { return _finalPath; };
    const bool exists() const { struct stat file_info; return stat(path().c_str(), &file_info) == 0; }

    std::string extension() const {
      std::string p = path();
      size_t i = p.rfind(".", p.length() );
      if (i != std::string::npos){
        return (p.substr(i+1, p.length()-i));
      }
      return("");
    }

    std::string filename() const {
      char sep = '/';
#ifdef _SEBURO_WINDOWS
      sep ='\\';
#endif
      std::string s = path();
      size_t i = s.rfind(sep,s.length());
      if (i != std::string::npos){
        return (s.substr(i+1,s.length() - i));
      }
      return ("");
    }


  protected:
    std::string _finalPath;


  };

}

#endif