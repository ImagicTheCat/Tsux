/* 
 * POSIX extended regex encapsulation 
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#include "Regex.hpp"

Regex::Regex(const std::string& expr) {
  int err = regcomp(&regex, expr.c_str(), REG_EXTENDED);

  valid = (err == 0);
}

Regex::~Regex(){
  regfree(&regex);
}

bool Regex::match(const std::string& str){
  if(valid){
    //clear
    matchs.clear();

    //matchs (with first match => all expr)
    int nmatch = regex.re_nsub+1;

    //build result table
    regmatch_t *match = new regmatch_t[nmatch];

    //exec
    int match_state = regexec(&regex, str.c_str(), nmatch, match, 0);

    //fill results : ignore first match (all expr)
    for(int i = 1; i < nmatch; i++){
      int b = match[i].rm_so;
      int e = match[i].rm_eo;

      const char* p = str.c_str();

      if(b >= 0 && b <= e && e >= 0 && e <= str.size())
        matchs.push_back(std::string(p+b, e-b));
      else
        matchs.push_back("");
    }

    return (match_state == 0);
  }
  else 
    return false;
}

std::string Regex::get(int index){
  if(index >= 0 && index < matchs.size())
    return matchs[index];
  else
    return "";
}
