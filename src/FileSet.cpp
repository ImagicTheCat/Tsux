/* 
 * FileSet and File
 * manage file and array file into a set
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */

#include "FileSet.hpp"

/* FILE */

bool File::loadFromFile(const std::string& path){
  std::ifstream file(path.c_str(), std::ios::binary);
  if(file){
    data = "";
    name = "";

    //set name
    Regex reg("^(.*/)?([[:alnum:]_.]+)$");
    if(reg.isValid() && reg.match(path))
      name = reg.get(1);

    //get content
    data.assign(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>());

    return true;
  }
  else
    return false;
}

/* FILESET */

File& FileSet::alloc(const std::string& param){
  std::map<std::string, std::vector<File> >::iterator it = entries.find(param);
  //add if doesn't exist
  if(it == entries.end())
    it = entries.insert(std::pair<std::string, std::vector<File> >(param, std::vector<File>())).first;

  std::vector<File>& files = it->second;
  files.push_back(File());
  return files[files.size()-1];
}


File& FileSet::get(const std::string& param, File& def){
  std::map<std::string, std::vector<File> >::iterator it = entries.find(param);
  if(it != entries.end()){
    std::vector<File>& files = it->second;
    if(files.size() > 0)
      return files[0];
  }

  return def;
}


std::vector<File>& FileSet::get(const std::string& param, std::vector<File>& def){
  std::map<std::string, std::vector<File> >::iterator it = entries.find(param);
  if(it != entries.end())
    return it->second;

  return def;
}


