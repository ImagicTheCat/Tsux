#ifndef H_TRANSLATOR
#define H_TRANSLATOR

#include "Config.hpp"

class Translator : public Config{
  public:
    bool hasTrans(const std::string& path);

    //trans with locale
    std::string trans(const std::string& path);

    void setLocale(const std::string& _locale){ locale = _locale; }
  private:
    std::string locale;
};

#endif
