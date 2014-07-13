/* 
 * FileSet and File
 * manage file and array file into a set
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#ifndef H_FILESET
#define H_FILESET

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "Regex.hpp"

struct File{
  std::string name, type, data;

  bool loadFromFile(const std::string& path);
};

class FileSet{
  public:
    std::map<std::string, std::vector<File> >& map(){ return entries; }
    std::vector<File>& get(const std::string& param, std::vector<File>& def);

    void clear(){ entries.clear(); }

    //if multiple : return the first file
    File& get(const std::string& param, File& def);

    //allocate a new file emplacement and return a reference to it
    File& alloc(const std::string& param);

  private:
    std::map<std::string, std::vector<File> > entries;
};

#endif
