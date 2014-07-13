/* 
 * Tsux
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#include "Tsux.hpp"

Tsux::Tsux():in(std::cin.rdbuf()),
             out(std::cout.rdbuf()),
             err(std::cerr.rdbuf()),
             sin(NULL),sout(NULL),serr(NULL),
             options(0), flushed(true){
  FCGX_Init();
  FCGX_InitRequest(&request, 0,0);
}

Tsux::~Tsux(){
  end();

  //regex
  for(int i = 0; i < regs.size(); i++)
    delete regs[i];
}


void Tsux::enable(unsigned int opts){
  options |= opts;
}

void Tsux::disable(unsigned int opts){
  options &= ~opts;
}

bool Tsux::enabled(unsigned int opt){
  return (options & opt);
}


void Tsux::initBufs(){
  flushed = false;

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

  if(ok){
    initBufs();

    //init vars
    post.clear();
    get.clear();
    response.clear();
    response.str("");
    route.clear();
    header.clear();
    param.clear();

    //webserver params
    for( ; *request.envp; ++request.envp){
      std::string couple(*request.envp);
      std::string name, value;
      bool mode = true;

      for(int i = 0; i < couple.size(); i++){
        char c = couple[i];
        if(mode){
          if(c == '=')
            mode = !mode;
          else
            name += c;
        }
        else
          value += c;

        //end
        if(i == couple.size()-1)
          param.set(name, value);
      }
    }


    //build location and get
    _uri = param.get("REQUEST_URI","");
    size_t sp1 = _uri.find("?");

    _location = _uri.substr(0, sp1);

    //gets vars
    if(sp1 != std::string::npos){
      std::string gets = _uri.substr(sp1+1);
      parseURICouples(gets, get);
    }

    //post vars
    if(param.get("CONTENT_TYPE","") == "application/x-www-form-urlencoded"){
      int length = param.get("CONTENT_LENGTH", 0);
      char data[length];

      in.read(data, length);

      parseURICouples(std::string(data),post);
    }
  }

  return ok;
}


void Tsux::bind(const std::string& path, TsuxAction action, void* data){
  //regex routing
  if(enabled(REGEX_ROUTING)){
    Regex *reg = new Regex(path);
    if(reg->isValid()){
      regs.push_back(reg);
      actions.push_back(Action(action, data));
    }
  }
  //simple routing
  else{
    std::map<std::string, Action>::iterator it = routes.find(path);
    if(it != routes.end())
      routes.erase(it);

    routes.insert(std::pair<std::string, Action>(path, Action(action, data)));
  }
}

void Tsux::dispatch(){
  //find route
  if(enabled(REGEX_ROUTING)){
    //regex routing
    //match regex
    int i = 0;
    bool done = false;
    while(!done && i < regs.size()){
      if(regs[i]->match(_location)){
        //set route params
        std::vector<std::string>& params = regs[i]->getMatchs();
        for(int j = 0; j < params.size(); j++){
          std::stringstream ss;
          ss << j;

          route.set(ss.str(), params[j]);
        }

        //execute
        actions[i].execute(*this);
        done = true;
      }
      i++;
    }

    if(!done)
      generate(404);
  }
  else{
    //simple routing
    std::map<std::string, Action>::iterator it = routes.find(_location);
    if(it != routes.end())
      it->second.execute(*this);
    else
      generate(404);
  }
}

void Tsux::end(){
  //prevent useless flush
  if(!flushed){
    //inject headers
    std::map<std::string, std::string>::iterator it = header.map().begin();
    while(it != header.map().end()){
      out << it->first << ": " << it->second << "\r\n";
      it++;
    }
    out << "\r\n";

    //inject response
    out << response.str();

    //ignore rest of input
    do in.ignore(1024); while (in.gcount() == 1024);

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
    flushed = true;
  }
}

void Tsux::parseURICouples(const std::string& uri, ParamSet& pset){
  std::string tmp;
  std::string name;
  bool mode = true;

  //parse couples
  for(int i = 0; i < uri.size(); i++){
    char c = uri[i];
    bool end = (i == uri.size()-1);

    //var name
    if(mode){
      if(c == '='){
        if(end)
          pset.set(URI::decode(tmp), "");

        name = tmp;
        mode = !mode;
        tmp = "";
      }
      else if(c == '&'){
        tmp = "";
        pset.set(URI::decode(tmp), "");
      }
      else if(end){
        tmp += c;
        pset.set(URI::decode(tmp), "");
      }
      else
        tmp += c;
    }
    else{
      if(c == '&' || end){
        if(end)
          tmp += c;

        pset.set(URI::decode(name), URI::decode(tmp));
        tmp = "";
        name = "";
        mode = !mode;
      }
      else
        tmp += c;
    }
  }
}

void Tsux::generate(int code){
  switch(code){
    case 404:
      header.set("Content-type", "text/html");
      header.set("Status", "404 Not Found");

      response << "<!DOCTYPE html>"
               << "<html>"
               << "<head><title>404 Not Found</title></head>"
               << "<body>"
               << "<div style=\"margin-top: 30px; text-align: center; border-bottom: 1px solid black;\">"
               << "<h1>404 Not Found</h1>"
               << "<p>Cruel, cruel cruel cruel.</p>"
               << "</div></body></html>";
    break;
  }
}


void Tsux::generate(ParamSet set){
  std::map<std::string, std::string>::iterator it = set.map().begin();
  response << "<ul>";
  while(it != set.map().end()){
    response << "<li>" << it->first << " = " << it->second << "</li>\n";
    it++;
  }
  response << "</ul>";
}
