/* 
 * MIME types manager
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#ifndef H_MIMETYPE
#define H_MIMETYPE

#include "Regex.hpp"
#include <map>

class MIMEType{
  public:
    //init types : call once
    static void init();

    //get the mimetype from extension, path, or filename
    static std::string get(const std::string& filext);

    //set mimetype for an extension
    static void set(const std::string& ext, const std::string& type);

  private:
    MIMEType();
    static std::map<std::string, std::string> exts;
    static std::string default_type;

    static bool initialized;
};

#endif
