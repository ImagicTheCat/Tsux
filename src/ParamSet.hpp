/* 
 * Param container with conversion of values
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#ifndef H_PARAMSET
#define H_PARAMSET

#include <iostream>
#include <sstream>
#include <map>


class ParamSet{
  public:
    bool has(const std::string& param) const;

    std::string get(const std::string& param, const std::string& def) const;
    void set(const std::string& param, const std::string& val);

    std::map<std::string,std::string>& map(){ return params; }

    /*
    template<typename T> T get(const std::string& param, const T& def) const;
    template<typename T> void set(const std::string& param, const T& val);
    */

    void clear();

    template<typename T> T get(const std::string& param, const T& def) const{
      std::map<std::string,std::string>::const_iterator it = params.find(param);
      if(it != params.end()){
        T val(0);
        std::stringstream ss(it->second);
        ss >> val;

        return val;
      }
      else
        return def;
    }


    template<typename T> void set(const std::string& param, const T& val){
      std::map<std::string, std::string>::iterator it = params.find(param);

      std::stringstream ss;
      ss << val;

      if(it != params.end())
        params.erase(it);

      params.insert(std::pair<std::string, std::string>(param, ss.str()));
    }



  private:
    std::map<std::string, std::string> params;
};

#endif
