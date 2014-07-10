#ifndef H_TSUX
#define H_TSUX

#include <iostream>
#include <fcgio.h>

class Tsux{
  public:
    Tsux();
    ~Tsux();
    bool accept();
    void flush();

    std::string param(const std::string& p);

    std::ostream out;
    std::istream in;
    std::ostream err;

  private:
    void initBufs();

    FCGX_Request request;
    fcgi_streambuf *sin, *sout, *serr;
};

#endif
