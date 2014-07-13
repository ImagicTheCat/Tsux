/* 
 * Tsux
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#ifndef H_TSUX
#define H_TSUX

#include <iostream>
#include <sstream>
#include <fcgio.h>

#include "ParamSet.hpp"
#include "Action.hpp"
#include "Regex.hpp"
#include "URI.hpp"


class Tsux{
  public:
    //options
    enum{
      REGEX_ROUTING = 1
    };


    Tsux();
    ~Tsux();

    //methods
    bool accept();
    void end();
    void bind(const std::string& path, TsuxAction action, void* data = NULL);
    void dispatch();

    //404, etc
    void generate(int code);
    void generate(ParamSet set);

    //options methods
    void enable(unsigned int options);
    void disable(unsigned int options);
    bool enabled(unsigned int option);

    //accessors
    const std::string& url()const{ return _url; }
    const std::string& location()const{ return _location; }

    //streams
    std::ostream out;
    std::istream in;
    std::ostream err;
    std::stringstream response;

    //data
    ParamSet post, get, header, route, param;

    //tools
    static void parseURLCouples(const std::string& url, ParamSet& pset);

  private:
    void initBufs();

    FCGX_Request request;
    fcgi_streambuf *sin, *sout, *serr;

    std::string _url, _location;

    //simple routes
    std::map<std::string, Action> routes;

    //regex routes
    std::vector<Regex*> regs;
    std::vector<Action> actions;

    unsigned int options;

    bool flushed;
};

#endif
