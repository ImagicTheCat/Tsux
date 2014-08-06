
#include "Translator.hpp"


bool Translator::hasTrans(const std::string& path){
  //build path
  std::vector<std::string> cpath;
  buildPath(path, cpath);

  //check all first domain of translation
  std::map<std::string, ConfigGroup*>::iterator it = root.els.begin();
  while(it != root.els.end()){
    //explode groups
    ConfigGroup* current = it->second;
    int i = 0;
    while(current != NULL && i < cpath.size()){
      current = current->get(cpath[i]);
      i++;
    }

    if(i == cpath.size() && current != NULL && current->els.size() > 0
        && current->els.begin()->second == NULL)
      return true;

    it++;
  }

  return false;
}

std::string Translator::trans(const std::string& path){
  if(has(locale+"."+path))
    return get(locale+"."+path);
  else if(has("all."+path))
    return get("all."+path);
  else 
    return path;
}

