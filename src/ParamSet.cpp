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


/*
template<typename T> T ParamSet::get(const std::string& param, const T& def) const{
  std::map<std::string,std::string>::const_iterator it = params.find(param);
  if(it != params.end()){
    T val;
    std::stringstream ss(it->second);
    ss >> val;

    return val;
  }
  else
    return def;
}


template<typename T> void ParamSet::set(const std::string& param, const T& val){
  std::map<std::string, std::string>::iterator it = params.find(param);

  std::stringstream ss;
  ss << val;

  if(it != params.end())
    params.erase(it);

  params.insert(std::pair<std::string, std::string>(param, ss.str()));
}

*/

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

/*
//template definitions
//string
template std::string ParamSet::get(const std::string&, const std::string&)const;
template void ParamSet::set(const std::string&, const std::string&);

//float
template float ParamSet::get(const std::string&, const float&)const;
template void ParamSet::set(const std::string&, const float&);

//int
template int ParamSet::get(const std::string&, const int&)const;
template void ParamSet::set(const std::string&, const int&);

//unsigned int
template unsigned int ParamSet::get(const std::string&, const unsigned int&)const;
template void ParamSet::set(const std::string&, const unsigned int&);

//double
template double ParamSet::get(const std::string&, const double&)const;
template void ParamSet::set(const std::string&, const double&);

//char
template char ParamSet::get(const std::string&, const char&)const;
template void ParamSet::set(const std::string&, const char&);
*/

/* scheme
template T ParamSet::get(const std::string&, const T&)const;
template void ParamSet::set(const std::string&, const T&);
*/
