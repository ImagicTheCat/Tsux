/* 
 * Module
 * 
 * let you manage your code into modules
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#ifndef H_MODULE
#define H_MODULE

#include <iostream>
#include "Tsux.hpp"

class Module{
  public:
    Module(Tsux& tsux, const std::string& name);

    /* Interface to tsux */
    //time in seconds
    void createCookie(const std::string& name, const std::string& data, int time){
      tsux.createCookie(name, data, time);
    }

    //generate html : 404, etc
    void generate(int code){ tsux.generate(code); }
    void generate(ParamSet set){ tsux.generate(set); }
    void generate(FileSet set){ tsux.generate(set); }

    //accessors
    const std::string& uri()const{ return tsux.uri(); }
    const std::string& location()const{ return tsux.location(); }

    //streams
    std::ostream& out;
    std::istream& in;
    std::ostream& err;
    std::stringstream& response;

    //data
    ParamSet &post, &get, &header, &route, &param, &cookie;
    FileSet &file;
    
    /* Module */
    //bind action for this module
    template<typename T> void bind(const std::string& path, void (T::*action)(void)){
      tsux.bind(path, Action((ModAction)action, this));
    }

    //accessors
    const std::string& name()const{ return _name; }

  private:
    Tsux& tsux;
    std::string _name;
};

typedef void (Module::*ModAction)(void);

#endif
