
#include "Dir.hpp"


void Dir::explode(const std::string& path, std::vector<std::string>& list, int options, bool first){
  DIR* dir = opendir(path.c_str());

  //is dir
  if(dir != NULL){
    //add the current folder
    if(options & SDIR)
      list.push_back(path);

    //fetch children
    struct dirent* child;
    while((child = readdir(dir)) != NULL && (options & RECURSIVE || first)){
      std::string name(child->d_name);
      if((name != ".." && name != ".") || (options & SPARENT))
        explode(path+(path[path.size()-1] != '/' ? "/" : "")+name, list, options, false);
    }
  }
  //is file
  else if(errno == ENOTDIR && options & SFILE)
    list.push_back(path);
}
