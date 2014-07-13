
#include <Tsux.hpp>

void header(Tsux& tsux, const std::string& title="Title", const std::string& subtitle="subtitle"){
  tsux.response << "<!DOCTYPE html>"
                << "<html><head><title>" << title << "</title>"
                << "<link href=\"/file/css\" rel=\"stylesheet\" type=\"text/css\">"
                << "</head>"
                << "<body><div id=\"header\">"
                << "<h1>" << title << "</h1>"
                << "<p>" << subtitle << "</p>"
                << "</div><div id=\"content\">";
}

void footer(Tsux& tsux, const std::string& text){
  tsux.response << "</div><div id=\"footer\">"
                << text
                << "</div></body></html>";
}

void get_file(Tsux& tsux, void* data){
  FileSet def;
  FileSet& files = Tsux::ref(data, def);

  if(files.has(tsux.route.get("1", ""))){
    File def;
    File& file = files.get(tsux.route.get("1",""), def);
    tsux.header.set("Content-Type", file.type);
    tsux.response << file.data;
  }
  else
    tsux.generate(404);
}

void index(Tsux& tsux, void* data){
  header(tsux, "Index", "index page");
  
  tsux.response << "<p>This is an example page</p>"
                << "<a href=\"dump\">dump data</a>";

  footer(tsux, "footer");
}

void dump(Tsux& tsux, void* data){
  header(tsux, "Dump", "dump data");

  tsux.response << "<a href=\"/\">index</a>";

  tsux.response << "<h2>PARAM</h2>";
  tsux.generate(tsux.param);

  tsux.response << "<h2>GET</h2>";
  tsux.generate(tsux.get);

  tsux.response << "<h2>POST</h2>";
  tsux.generate(tsux.post);

  tsux.response << "<h2>FILE</h2>";
  tsux.generate(tsux.file);

  tsux.response << "<h2>COOKIE</h2>";
  tsux.generate(tsux.cookie);


  tsux.createCookie("mon_cookie", "blabla\"+([", 60);


  footer(tsux, "footer");
}

int main(int argc, char** argv){
  Tsux tsux;

  FileSet files;
  File& css = files.alloc("css");
  css.loadFromFile("style.css");
  css.type = "text/css";

  tsux.enable(Tsux::REGEX_ROUTING);

  tsux.bind("^/file/([[:alnum:]]+)$", get_file, &files);
  tsux.bind("^/$", index);
  tsux.bind("^/dump$", dump);
  
  while(tsux.accept()){
    tsux.dispatch();
    tsux.end();
  }

  return 0;
}
