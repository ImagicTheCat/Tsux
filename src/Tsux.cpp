/* 
 * Tsux
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#include "Tsux.hpp"
#include "Module.hpp"

Tsux::Tsux():in(std::cin.rdbuf()),
             out(std::cout.rdbuf()),
             err(std::cerr.rdbuf()),
             sin(NULL),sout(NULL),serr(NULL),
             options(0), flushed(true),
             session_time(60),
             f_session_create(NULL),
             f_session_delete(NULL),
             check_timer_interval(60),
             route_locked(-1){
  
  srand(time(NULL));
  check_timer = time(NULL);

  FCGX_Init();
  FCGX_InitRequest(&request, 0,0);

  MIMEType::init();
  ssid_alpha = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
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
    route_locked = -1;
    initBufs();

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


    //build location and get vars
    _uri = param.get("REQUEST_URI","");
    size_t sp1 = _uri.find("?");

    _location = _uri.substr(0, sp1);
    _path = _location;

    //gets vars
    if(sp1 != std::string::npos){
      std::string gets = _uri.substr(sp1+1);
      parseURICouples(gets, get);
    }

    //search locale
    std::string lname, lvalue;
    ParamSet lopts;
    parseMIMEField("field: "+param.get("HTTP_ACCEPT_LANGUAGE",";"), lname, lvalue, lopts);
    //parse languages
    std::vector<std::string> langs;
    std::string tmp="";
    int min = 1000;
    int imin = -1;
    for(int i = 0; i < lvalue.size(); i++){
      const char& c = lvalue[i];
      bool end = (i == lvalue.size()-1);

      if(c == ',' || end){
        if(end && c != ' ')
          tmp += c;

        if(tmp.size() != 0){
          if(tmp.size() < min){
            min = tmp.size();
            imin = langs.size();
          }

          langs.push_back(tmp);
          tmp = "";
        }
      }
      else if(c != ' ')
        tmp += c;
    }

    if(imin != -1)
      _locale = langs[imin];

    //post vars
    if(param.get("CONTENT_TYPE","") == "application/x-www-form-urlencoded"){
      int length = param.get("CONTENT_LENGTH", 0);
      char data[length];

      in.read(data, length);

      parseURICouples(std::string(data, length),post);
    }
    else if(param.get("CONTENT_TYPE","").find("multipart/form-data") != std::string::npos)
      parseMIMEData();

    //cookies
    std::string cname, cvalue;
    parseMIMEField("Cookie: value; "+param.get("HTTP_COOKIE", ""), cname, cvalue, cookie);

    //session
    if(enabled(SESSION)){
      _ssid = "";

      std::map<std::string, unsigned long int>::iterator it = sessions.find(cookie.get("SSID",""));
      unsigned long int current_time = time(NULL);
      if(it != sessions.end() && it->first != ""){
        //session exist
        if(current_time > it->second + session_time){
          //session expire
          deleteSession(it);
          createSession();
        }
        else{
          //check session
          if(checkSSID(it->first)){
            //valid session
            //reupdate time
            it->second = current_time;
            //set the global ssid
            _ssid = it->first;
          }
          else{
            //invalid session
            createSession();
          }
        }
      }
      else{
        //new session
        createSession();
      }

      //check sessions by interval
      if(current_time >= check_timer+check_timer_interval){
        checkSessions();
        check_timer = current_time;
      }
    }
  }

  return ok;
}


//bind normal action
void Tsux::bind(const std::string& path, TsuxAction action, void* data){
  bind(path, Action(action, data));
}


void Tsux::bind(const std::string& path, const Action& action){
  //regex routing
  if(enabled(REGEX_ROUTING)){
    Regex *reg = new Regex(path);
    if(reg->isValid()){
      regs.push_back(reg);
      actions.push_back(action);
    }
  }
  //simple routing
  else{
    std::map<std::string, Action>::iterator it = routes.find(path);
    if(it != routes.end())
      routes.erase(it);

    routes.insert(std::pair<std::string, Action>(path, action));
  }
}

void Tsux::registerModule(const std::string& name, Module* module){
  if(module != NULL){
    std::map<std::string, Module*>::iterator it = modules.find(name);
    if(it != modules.end())
      modules.erase(it);
    modules.insert(std::pair<std::string, Module*>(name, module));
  }
}

void Tsux::unregisterModule(const std::string& name){
  std::map<std::string, Module*>::iterator it = modules.find(name);
  if(it != modules.end()){
    //find in session listener and erase
    std::vector<Module*>::iterator it_s = std::find(session_modules.begin(), session_modules.end(), it->second);
   if(it_s != session_modules.end())
    session_modules.erase(it_s); 

    //erase from modules
    modules.erase(it);
  }
}

Module* Tsux::module(const std::string& name){
  std::map<std::string, Module*>::iterator it = modules.find(name);
  if(it != modules.end())
    return it->second;
  else
    return NULL;
}


void Tsux::rewrite(const std::string& route){
  _path = route;
  dispatch();
}

void Tsux::dispatch(){
  //find route
  if(enabled(REGEX_ROUTING)){
    //regex routing
    //match regex
    int i = 0;
    bool done = false;
    while(!done && i < regs.size()){
      //check if the route is locked before (prevent infinite rewriting)
      if(i != route_locked && regs[i]->match(_path)){
        //set route params
        std::vector<std::string>& params = regs[i]->getMatchs();
        for(int j = 0; j < params.size(); j++){
          std::stringstream ss;
          ss << j+1;

          route.set(ss.str(), params[j]);
        }

        //lock the route
        route_locked = i;

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
    std::map<std::string, Action>::iterator it = routes.find(_path);
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

    //inject header stream
    out << header_stream.str();
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

    get.clear();
    route.clear();
    post.clear();
    header.clear();
    file.clear();
    cookie.clear();

    response.clear();
    response.str("");

    header_stream.clear();
    header_stream.str("");


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

void Tsux::parseMIMEData(){
  const int NONE = 0;
  const int DATA = 1;
  const int PARAM = 2;
  
  const int TEXT = 0;
  const int FILE = 1;

  int mode = NONE;
  int type = TEXT;

  File* pfile;

  std::string sep, tmp, boundary, eboundary;
  int boundary_pos;
  std::vector<std::string> params;
  ParamSet pset;

  int done = 0;
  int length = param.get("CONTENT_LENGTH",0);
  while(done < length){
    char c = in.get();

    //first line : boundary
    if(mode == NONE){
      //search for line
      if(c == '\n' || c == '\r'){
        sep += c;
        if(sep == "\r\n"){
          mode = PARAM;
          sep = "";

          //real boundary
          eboundary = "\r\n"+boundary;
        }
      }
      else
        boundary += c;
    }
    else if(mode == PARAM){
      //search for line
      if(c == '\n' || c == '\r'){
        sep += c;
        if(sep == "\r\n"){
          if(tmp.size() > 0){
            params.push_back(tmp);
            tmp = "";
          }
          else{
            //parse mime params
            for(int i = 0; i < params.size(); i++){
              std::string name, value;
              ParamSet opts;
              parseMIMEField(params[i], name, value, opts);

              //analyse field
              if(name == "Content-Disposition"){
                std::string pname = opts.get("name","");
                pset.set("name", pname);

                if(opts.has("filename")){
                  //prepare file reading
                  std::string filename = opts.get("filename","");;
                  pset.set("filename", filename);
                  type = FILE;

                  //alloc file into file Tsux FileSet
                  pfile = &(file.alloc(pname));
                }

              }
              else if(name == "Content-Type")
                pset.set("type", value);
            }

            mode = DATA;
            boundary_pos = 0;
            params.clear();
          }
          sep = "";
        }
      }
      else
        tmp += c;
    }
    else if(mode == DATA){

      if(type == FILE)
        pfile->data += c;
      else
        tmp += c;

      //waiting \r\n and boundary
      //test if the boundary will be match
      if(c == eboundary[boundary_pos]){
        //boundary possible
        if(boundary_pos == eboundary.size()-1){
          //boundary : save data
          mode = PARAM;
          if(type == FILE){
            //delete boundary from data
            if(pfile->data.size() > eboundary.size())
              pfile->data.resize(pfile->data.size()-eboundary.size());
            else
              pfile->data.clear();

            pfile->name = pset.get("filename", "");
            pfile->type = pset.get("type", "");
          }
          else if(type == TEXT){
            //delete boundary from data
            if(tmp.size() > eboundary.size())
              tmp.resize(tmp.size()-eboundary.size());
            else
              tmp.clear();
            post.set(pset.get("name",""), tmp);
          }

          tmp = "";
          type = TEXT;
          pset.clear();
          
          //skip the two next characters
          in.get();
          in.get();
        }
        
        boundary_pos++;
      }
      else //no match, reset
        boundary_pos = 0;
    }
    done++;
  }
}



std::string Tsux::deleteChars(const std::string& str, const std::string& chars){
  std::string r;
  for(int i = 0; i < str.size(); i++){
    int j = 0;
    bool done = false;
    while(!done && j < chars.size()){
      if(str[i] == chars[j])
        done = true;
      j++;
    }

    if(!done)
      r += str[i];
  }

  return r;
}

void Tsux::parseMIMEField(const std::string& field, std::string& name, std::string& value, ParamSet& options){
  const int NONE = 0;
  const int NAME = 1;
  const int VALUE = 2;
  
  //split to ":"
  size_t sp1 = field.find(":");
  if(sp1 != std::string::npos){
    name = deleteChars(field.substr(0, sp1), " \n\r");

    //get value
    size_t sp2 = field.find(";");
    if(sp2 != std::string::npos){
      value = deleteChars(field.substr(sp1+1, sp2-sp1-1)," \n\r");

      //parse options
      int mode = NONE;
      std::string oname, ovalue;
      for(int i = sp2; i < field.size(); i++){
        char c = field[i];
        bool end = (i == field.size()-1);

        //waiting
        if(mode == NONE){
          if(c != ';' && c != ' '){
            oname += c;
            mode = NAME;
          }
        }
        //parse name
        else if(mode == NAME){
          if(c == '=')
            mode = VALUE;
          else if(c != ' ' && c != '=')
            oname += c;
        }
        //parse value
        else if(mode == VALUE){
          if(c == ';' || end){
            mode = NONE;
            if(end)
              ovalue += c;
            options.set(URI::decode(oname), URI::decode(deleteChars(ovalue,"\"")));
            oname = "";
            ovalue = "";
          }
          else
            ovalue += c;
        }
      }
    }
    else
      value = field.substr(sp1+2);
  }
}


void Tsux::createCookie(const std::string& name, const std::string& data, int time){
  if(name.size() >= 0 && time >= 0 && name.size() < 100 && data.size() < 100){
    header_stream << "Set-Cookie: " << URI::encode(name) << "=" << URI::encode(data) << ";"
                  << " Path=/;"
                  << " Max-Age=" << time << "\r\n";
  }
}

std::string Tsux::generateSSID(){
  //IP | TIME AND RAND
  //4  1  4        4
  
  //convert string ip to unsigned char[4]
  unsigned char ip[4];
  int shift = 0;
  std::string sip = param.get("REMOTE_ADDR", "127.0.0.1");
  std::string tmp;
  for(int i = 0; i < sip.size(); i++){
    const char& c = sip[i];
    bool end = (i == sip.size()-1);

    if(c == '.' || end){
      if(end)
        tmp += c;

      std::stringstream ss;
      ss << tmp;
      int val;
      ss >> val;
      ip[shift] = val;
      shift++;
      shift %= 4;

      tmp.clear();
    }
    else
      tmp += c;
  }

  //encode ip
  std::string eip = Base::encode(ip, 4, ssid_alpha);

  //generate random param
  unsigned long int random = time(NULL)+rand();
  std::string erandom = Base::encode(&random, sizeof(random), ssid_alpha);

  return eip+"|"+erandom;
}

bool Tsux::checkSSID(const std::string& ssid){
  //split at '|' to get the ip part
  std::string pip;
  int i = 0;
  while(i < ssid.size() && ssid[i] != '|'){
    pip += ssid[i];
    i++;
  }

  unsigned char ip[4];

  //decode ip
  Base::decode(ip, 4, pip, ssid_alpha);

  std::string sip;
  for(i = 0; i < 4; i++){
    std::stringstream ss;
    ss << (int)ip[i];
    sip += ss.str();
    if(i != 3)
      sip += '.';
  }

  return (param.get("REMOTE_ADDR","") == sip);
}

void Tsux::createSession(){
  //new session
  unsigned long int current_time = time(NULL);

  std::string ssid = generateSSID();
  sessions.insert(std::pair<std::string, unsigned long int>(ssid, current_time));
  createCookie("SSID", ssid, session_time);
  _ssid = ssid;

  //events
  if(f_session_create != NULL)
    f_session_create(ssid);

  for(int i = 0; i < session_modules.size(); i++)
    session_modules[i]->onSessionCreate(ssid);
}


void Tsux::deleteSession(std::map<std::string, unsigned long int>::iterator it){
  //events
  if(f_session_delete != NULL)
    f_session_delete(it->first);

  for(int i = 0; i < session_modules.size(); i++)
    session_modules[i]->onSessionDelete(it->first);

  sessions.erase(it);
}

void Tsux::checkSessions(){
  //delete sessions expired
  unsigned long int current_time = time(NULL);
  std::map<std::string, unsigned long int>::iterator it = sessions.begin();
  while(it != sessions.end()){
    if(current_time > it->second+session_time)
      deleteSession(it);
    it++;
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

void Tsux::generate(FileSet set){
  std::map<std::string, std::vector<File> >::iterator it = set.map().begin();
  response << "<ul>";
  while(it != set.map().end()){
    std::vector<File>& files = it->second;

    for(int i = 0; i < files.size(); i++){
      response << "<li>" << it->first << "[" << i << "]";
      response << "<ul>";

      response << "<li>name = " << files[i].name << "</li>";
      response << "<li>type = " << files[i].type << "</li>";
      response << "<li>data-length = " << files[i].data.size() << "</li>";

      response << "</ul>";
      response << "</li>\n";
    }

    it++;
  }
  response << "</ul>";
}


