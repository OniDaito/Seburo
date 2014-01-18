/**
* @brief Cross Platform File Handling
* @file file.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 06/03/2013
*
*/

#include "s9/file.hpp"

using namespace std;

using namespace s9;

Path::Path() {
  final_path_ = ".";
}


Path::Path(std::string path){
  if ( path.find("/") == 0 ){
    // Absolute path
    final_path_ = path;
  } 
  else if (path.find(".") == 0){
    // relative path - setup the data dir
#ifdef _SEBURO_OSX 
    final_path_ = "../Resources/" + ltrim(path);
#endif

#ifdef _SEBURO_LINUX
    // This exists path is a bit naughty and only here for examples but hey
    if (Exists("../../../" + path) ) {
      final_path_ = "../../../" + path;
    } else if (Exists(path)){
      final_path_ = path;
    } else {
      cerr << "SEBURO FILE Error - Path does not exist: " << path << endl;
    }

#endif
    final_path_ = string(realpath(final_path_.c_str(),NULL)); 

  }
  else {
    // error - malformed!
    cerr << "SEBURO FILE Error - Path was malformed: " << path << endl;
    assert(false);
  }
}

/// list all the files inside this directory
std::vector<File> Directory::ListFiles() {

  std::vector<File> files;

  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir (final_path_.c_str())) != NULL) {
    while ((ent = readdir (dir)) != NULL) {

#ifndef _SEBURO_WIN32
      struct stat st;
      
      string fp = final_path_ + "/" + string(ent->d_name);
      fp = string(realpath(fp.c_str(),NULL)); 
      if (lstat(fp.c_str(), &st) == 0) {
        if(S_ISREG(st.st_mode))
          files.push_back(File(fp));

      } else {
        perror("stat");
      }
     
    }
#endif

    closedir (dir);
  } else {
    /* could not open directory */
    cerr << "SEBURO DIRECTORY ERROR - Could not open " << final_path_ << endl;
  }
  return files;
}