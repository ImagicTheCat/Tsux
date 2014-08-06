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
    //options
    enum{
      SESSION = 1
    };

    //the name need to be unique
    Module(Tsux& tsux, const std::string& name, unsigned int options = 0);
    ~Module();

    //accessors
    const std::string& name()const{ return _name; }

    /* Session events */
    virtual void onSessionCreate(const std::string& ssid){}
    virtual void onSessionDelete(const std::string& ssid){}
 

  protected:
    /* Interface to tsux */
    //time in seconds
    void createCookie(const std::string& name, const std::string& data, int time){
      tsux.createCookie(name, data, time);
    }

    void rewrite(const std::string& route){ tsux.rewrite(route); }

    //generate html : 404, etc
    void generate(int code){ tsux.generate(code); }
    void generate(ParamSet set){ tsux.generate(set); }
    void generate(FileSet set){ tsux.generate(set); }

    //misc
    Module* module(const std::string& name){ return tsux.module(name); }

    //accessors
    const std::string& uri()const{ return tsux.uri(); }
    const std::string& location()const{ return tsux.location(); }
    const std::string& path()const{ return tsux.path(); }

    const std::string& locale()const{ return tsux.locale(); }
    void locale(const std::string& loc){ tsux.locale(loc); }
    const std::string& ssid(){ return tsux.ssid(); }

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

    //bind codes
    template<typename T> void bindCode(int code, void (T::*action)(void)){
      tsux.bindCode(code, Action((ModAction)action, this));
    }

    void bindCode(int code, TsuxAction func, void *data = NULL){ tsux.bindCode(code, func, data); }



    Tsux& tsux;

  private:
    unsigned int options;
    std::string _name;
};

typedef void (Module::*ModAction)(void);

#endif
