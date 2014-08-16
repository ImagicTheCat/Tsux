/* 
 * HTML tools
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */

#ifndef H_HTML
#define H_HTML

#include <iostream>
#include <map>

class HTML{
  public:
    static void init();

    inline static void add(char c, const std::string& replacement){
      map_entities.insert(std::pair<char, std::string>(c, replacement));
    }

    static std::string entities(const std::string& str);

  private:
    static std::map<char, std::string> map_entities;

    HTML();
};

#endif
