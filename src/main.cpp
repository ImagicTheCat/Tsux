
#include "Tsux.hpp"

#include <fstream>

void index(Tsux &tsux, void *data){
    tsux.header.set("Content-type", "text/html");

    tsux.response << "<h1>Hello world</h1>\n";
    tsux.response << "<p>Your ip is <strong>" << tsux.param.get("REMOTE_ADDR","") << "</strong></p>";
    tsux.response << "<p>The URL is <strong>" << tsux.url() << "</strong></p>";
    tsux.response << "<p>The location is <strong>" << tsux.location() << "</strong></p>";
    tsux.response << "<p>Nothin is <strong>" << tsux.param.get("nothing","") << "</strong></p>";

    //display get
    std::map<std::string, std::string>::iterator it = tsux.get.map().begin();
    tsux.response << "<ul>";
    while(it != tsux.get.map().end()){
      tsux.response << "<li>" << it->first << " = " << it->second << "</li>\n";
      it++;
    }
    tsux.response << "</ul>";

    int n = tsux.get.get("n", 0);
    for(int i = 0; i < n; i++){
      tsux.response << i << " ";
    }
}

void record(Tsux& tsux, void *data){
  tsux.header.set("Content-type", "text/plain");

  std::string& records = (std::string&)(*((std::string*)data));
  records += tsux.get.get("msg", "nothing") + "\n";
  
  tsux.response << records;
}

void hello(Tsux& tsux, void* data){
  tsux.header.set("Content-type", "text/plain");

  if(tsux.route.get("1", "") == "")
    tsux.header.set("Location", "http://perdu.com");
  tsux.response << "Hello " << tsux.route.get("0", "name") << " " << tsux.route.get("1", "firstname") << " ! \n";
}

void dump(Tsux& tsux, void* data){
  tsux.header.set("Content-type", "text/html");

  tsux.response << "<h1>DUMP</h1>";
  tsux.response << "<h2>Param</h2>";
  tsux.generate(tsux.param);

  tsux.response << "<h2>Header</h2>";
  tsux.generate(tsux.header);


  tsux.response << "<h2>Route</h2>";
  tsux.generate(tsux.route);


  tsux.response << "<h2>Get</h2>";
  tsux.generate(tsux.get);

  tsux.response << "<h2>Post</h2>";
  tsux.generate(tsux.post);
}

int main(int argc, char** argv){
  Tsux tsux;

  std::string records;

  tsux.enable(Tsux::REGEX_ROUTING);

  tsux.bind("^/fcgi$", index);
  tsux.bind("^/fcgi/record$", record, &records);
  tsux.bind("^/fcgi/hello/(.+)-(.+)$", hello);
  tsux.bind("^/fcgi/dump$", dump);

  while(tsux.accept()){
    tsux.dispatch();
    tsux.end();
  }

  return 0;
}
