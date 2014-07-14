/* 
 * Dir research encapsulation
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#ifndef H_DIR
#define H_DIR

#include <iostream>
#include <dirent.h>
#include <errno.h>
#include <vector>

class Dir{
  public:
    enum{
      SFILE = 1,
      SDIR = 2,
      RECURSIVE = 4,
      SPARENT = 8
    };

    //return path like "mypath/dir/file"
    //can include (S)DIR, (S)FILE, (S)PARENT (.. and .) and do RECURSIVE
    //do not use RECURSIVE and (S)PARENT, cause infinite research
    static void explode(const std::string& path, std::vector<std::string>& list, int options = SDIR | SFILE, bool first = true);

  private:
   Dir(); 
};

#endif
