
#include "Tsux.hpp"

Tsux::Tsux():in(std::cin.rdbuf()),
             out(std::cout.rdbuf()),
             err(std::cerr.rdbuf()),
             sin(NULL),sout(NULL),serr(NULL) {
  FCGX_Init();
  FCGX_InitRequest(&request, 0,0);
}

Tsux::~Tsux(){
  flush();
}

void Tsux::initBufs(){
  if(sin != NULL)
    delete sin;
  sin = new fcgi_streambuf(request.in);
  in.rdbuf(sin);

  if(sout != NULL)
    delete sout;
  sout = new fcgi_streambuf(request.out);
  out.rdbuf(sout);

  if(serr != NULL)
    delete serr;
  serr = new fcgi_streambuf(request.err);
  err.rdbuf(serr);
}

bool Tsux::accept(){
  bool ok = FCGX_Accept_r(&request) == 0;
  initBufs();

  return ok;
}

void Tsux::flush(){
  if(sin != NULL)
    delete sin;
  if(sout != NULL)
    delete sout;
  if(serr != NULL)
    delete serr;

  sin = NULL;
  sout = NULL;
  serr = NULL;

  in.rdbuf(std::cin.rdbuf());
  out.rdbuf(std::cout.rdbuf());
  err.rdbuf(std::cerr.rdbuf());
}


std::string& Tsux::param(const std::string p){
}
