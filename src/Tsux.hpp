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

class Tsux{
  public:
    Tsux();
    ~Tsux();

    //methods
    bool accept();
    void end();
    void bind(const std::string& path, TsuxAction action, void* data = NULL);
    void dispatch();

    //accessors
    const std::string& url()const{ return _url; }
    const std::string& location()const{ return _location; }
    std::string param(const std::string& p);

    //streams
    std::ostream out;
    std::istream in;
    std::ostream err;
    std::stringstream response;

    //data
    ParamSet post, get, header;

    //tools
    static void parseURLCouples(const std::string& url, ParamSet& pset);

  private:
    void initBufs();

    FCGX_Request request;
    fcgi_streambuf *sin, *sout, *serr;

    std::string _url, _location;

    std::map<std::string, Action> routes;
};

#endif
