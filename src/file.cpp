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
    if (exists("../../../" + path) ) {
      final_path_ = "../../../" + path;
    } else if (exists(path)){
      final_path_ = path;
    } else {
      cout << "does not exist" << endl;
    }

#endif

  }
  else {
    // error - malformed!
    cerr << "SEBURO FILE Error - Path was malformed: " << path << endl;
    assert(false);
  }
}

/// list all the files inside this directory
std::vector<File> Directory::list_files() {

  std::vector<File> files;

  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir (final_path_.c_str())) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      struct stat st;
      lstat(ent->d_name, &st);
      if(!S_ISDIR(st.st_mode))
        files.push_back(File(final_path_ + "/" + string(ent->d_name)));
    }
    closedir (dir);
  } else {
    /* could not open directory */
    cerr << "SEBURO DIRECTORY ERROR - Could not open " << final_path_ << endl;
  }
  return files;
}