
#include "Tsux.hpp"

#include <fstream>

void index(Tsux &tsux, void *data){
    tsux.header.set("Content-type", "text/html");

    tsux.response << "<h1>Hello world</h1>\n";
    tsux.response << "<p>Your ip is <strong>" << tsux.param("REMOTE_ADDR") << "</strong></p>";
    tsux.response << "<p>The URL is <strong>" << tsux.url() << "</strong></p>";
    tsux.response << "<p>The location is <strong>" << tsux.location() << "</strong></p>";
    tsux.response << "<p>Nothin is <strong>" << tsux.param("nothing") << "</strong></p>";

    //display get
    std::map<std::string, std::string>::iterator it = tsux.get.getParams().begin();
    tsux.response << "<ul>";
    while(it != tsux.get.getParams().end()){
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

int main(int argc, char** argv){
  Tsux tsux;

  std::string records;

  tsux.bind("/fcgi", index);
  tsux.bind("/fcgi/record", record, &records);

  while(tsux.accept()){
    tsux.dispatch();
    tsux.end();
  }

  return 0;
}
