/* 
 * HTML tools
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */

#include "HTML.hpp"

//statics
std::map<char, std::string> HTML::map_entities = std::map<char, std::string>();

//functions

void HTML::init(){
  add('<', "&lt;");
  add('>', "&gt;");
  add('\'', "&apos;");
  add('\"', "&quot;");
  add('&', "&amp;");
}

std::string HTML::entities(const std::string& str){
  std::string tmp;

  for(int i = 0; i < str.size(); i++){
    const char& c = str[i];

    std::map<char, std::string>::iterator it = map_entities.find(c);
    if(it != map_entities.end())
      tmp.append(it->second);
    else
      tmp += c;
  }

  return tmp;
}
