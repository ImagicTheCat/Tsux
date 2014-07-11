#ifndef H_TSUX
#define H_TSUX

#include <iostream>
#include <sstream>
#include <fcgio.h>

#include "ParamSet.hpp"

class Tsux{
  public:
    Tsux();
    ~Tsux();
    bool accept();
    void end();

    const std::string& url()const{ return _url; }
    const std::string& location()const{ return _location; }

    void parseURLCouples(const std::string& url, ParamSet& pset);

    std::string param(const std::string& p);

    std::ostream out;
    std::istream in;
    std::ostream err;

    std::stringstream response;

    ParamSet post, get, header;

  private:
    void initBufs();

    FCGX_Request request;
    fcgi_streambuf *sin, *sout, *serr;

    std::string _url, _location;
};

#endif
