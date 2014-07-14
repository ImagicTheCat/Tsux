#ifndef H_MIMETYPE
#define H_MIMETYPE

#include "Regex.hpp"
#include <map>

class MIMEType{
  public:
    static void init();

    static std::string get(const std::string& filext);
    static void set(const std::string& ext, const std::string& type);

  private:
    MIMEType();
    static std::map<std::string, std::string> exts;
    static std::string default_type;

    static bool initialized;
};

#endif
