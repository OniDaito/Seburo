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
#include <dirent.h>
  

///\todo Windows seperator replacement in the constructor for this

///\todo relationship between directory and file and exists needs to change a little

namespace s9 {

  class SEBUROAPI Path {
  public:
    Path();
    Path(std::string path);
    std::string final_path() const { return final_path_; };
    const bool Exists() const { return Exists(final_path_); };
    static const bool Exists(const std::string &path) { struct stat file_info; return stat(path.c_str(), &file_info) == 0; }

  protected:
    std::string final_path_;
  };

  class SEBUROAPI File : public Path {
  public:
    File() : Path() {};
    File(std::string path) : Path(path) {};


    std::string Extension() const {
      std::string p = final_path();
      size_t i = p.rfind(".", p.length() );
      if (i != std::string::npos){
        return (p.substr(i+1, p.length()-i));
      }
      return("");
    }

    std::string DirectoryName() const {
      char sep = '/';
      #ifdef _SEBURO_WINDOWS
        sep ='\\';
      #endif
      std::string s = final_path();
      size_t i = s.rfind(sep,s.length());
      if (i != std::string::npos){
        return s.substr(0,i);
      }
      return ("");
    }

    std::string Filename() const {
      char sep = '/';
#ifdef _SEBURO_WINDOWS
      sep ='\\';
#endif
      std::string s = final_path();
      size_t i = s.rfind(sep,s.length());
      if (i != std::string::npos){
        return (s.substr(i+1,s.length() - i));
      }
      return ("");
    }

  };

  class SEBUROAPI Directory : public Path {
  public:
    Directory() : Path() {}
    Directory(std::string path) : Path(path) {}

    std::vector<File> ListFiles();

  };

 
}

#endif