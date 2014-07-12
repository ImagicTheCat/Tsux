/* 
 * POSIX extended regex encapsulation 
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */

#ifndef H_REGEX
#define H_REGEX

#include <iostream>
#include <regex.h>
#include <vector>

class Regex{
  public:
    Regex(const std::string& expr);
    ~Regex();

    bool match(const std::string& str);
    bool isValid(){ return valid; }

    //get the matched group by order
    std::string get(int index);

    std::vector<std::string>& getMatchs(){ return matchs; }

  private:
    regex_t regex;
    std::vector<std::string> matchs;
    bool valid;
};

#endif
