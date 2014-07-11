
#include "Tsux.hpp"

Tsux::Tsux():in(std::cin.rdbuf()),
             out(std::cout.rdbuf()),
             err(std::cerr.rdbuf()),
             sin(NULL),sout(NULL),serr(NULL) {
  FCGX_Init();
  FCGX_InitRequest(&request, 0,0);
}

Tsux::~Tsux(){
  end();
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

  //init vars
  post.clear();
  get.clear();
  response.clear();
  response.str("");

  //build location and get
  _url = param("REQUEST_URI");
  size_t sp1 = _url.find("?");

  _location = _url.substr(0, sp1);

  //gets vars
  if(sp1 != std::string::npos){
    std::string gets = _url.substr(sp1+1);
    parseURLCouples(gets, get);
  }

  return ok;
}


void Tsux::bind(const std::string& path, TsuxAction action, void* data){
  std::map<std::string, Action>::iterator it = routes.find(path);
  if(it != routes.end())
    routes.erase(it);

  routes.insert(std::pair<std::string, Action>(path, Action(action, data)));
}

void Tsux::dispatch(){
  //find route
  std::map<std::string, Action>::iterator it = routes.find(_location);
  if(it != routes.end())
    it->second.execute(*this);
  else
    header.set("Status", "404 Not Found");
}

void Tsux::end(){
  //inject headers
  std::map<std::string, std::string>::iterator it = header.getParams().begin();
  while(it != header.getParams().end()){
    out << it->first << ": " << it->second << "\r\n";
    it++;
  }
  out << "\r\n";

  //inject response
  out << response.str();

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

void Tsux::parseURLCouples(const std::string& url, ParamSet& pset){
  std::string tmp;
  std::string name;
  bool mode = true;

  //parse couples
  for(int i = 0; i < url.size(); i++){
    char c = url[i];
    bool end = (i == url.size()-1);

    //var name
    if(mode){
      if(c == '='){
        if(end)
          pset.set(tmp, "");

        name = tmp;
        mode = !mode;
        tmp = "";
      }
      else if(c == '&'){
        tmp = "";
        pset.set(tmp, "");
      }
      else if(end){
        tmp += c;
        pset.set(tmp, "");
      }
      else
        tmp += c;
    }
    else{
      if(c == '&' || end){
        if(end)
          tmp += c;

        pset.set(name, tmp);
        tmp = "";
        name = "";
        mode = !mode;
      }
      else
        tmp += c;
    }
  }
}

std::string Tsux::param(const std::string& p){
  char* str = FCGX_GetParam(p.c_str(), request.envp);
  if(str != NULL)
    return std::string(str);
  else
    return "";
}
