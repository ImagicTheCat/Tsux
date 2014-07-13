/* 
 * Param container with conversion of values
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#include "ParamSet.hpp"

bool ParamSet::has(const std::string& param) const{
  return params.find(param) != params.end();
}

void ParamSet::clear(){
  params.clear();
}

std::string ParamSet::get(const std::string& param, const std::string& def) const{
  std::map<std::string,std::string>::const_iterator it = params.find(param);
  if(it != params.end())
    return it->second;
  else
    return def;
}


void ParamSet::set(const std::string& param, const std::string& val){
  std::map<std::string, std::string>::iterator it = params.find(param);

  if(it != params.end())
    params.erase(it);

  params.insert(std::pair<std::string, std::string>(param,val));
}
