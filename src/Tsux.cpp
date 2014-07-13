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
    else if(param.get("CONTENT_TYPE","").find("multipart/form-data") != std::string::npos)
      parseMIMEData();
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

    get.clear();
    route.clear();
    post.clear();
    header.clear();
    file.clear();
    response.clear();
    response.str("");

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
          if(c == '\"')
            mode = VALUE;
          else if(c != ' ' && c != '=')
            oname += c;
        }
        //parse value
        else if(mode == VALUE){
          if(c == '\"'){
            mode = NONE;
            options.set(oname, ovalue);
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


