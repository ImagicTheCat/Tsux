
#include <Tsux.hpp>

void header(Tsux& tsux){
  tsux.response << "<!DOCTYPE html>"
                << "<html><head><title>Example</title>"
                << "<link href=\"/file/css\" rel=\"stylesheet\" type=\"text/css\">"
                << "</head>"
                << "<body>";
}

void footer(Tsux& tsux){
  tsux.response << "</body></html>";
}

void get_file(Tsux& tsux, void* data){
  FileSet def;
  FileSet& files = Tsux::ref(data, def);

  if(files.has(tsux.route.get("0", ""))){
    File def;
    File& file = files.get(tsux.route.get("0",""), def);
    tsux.header.set("Content-Type", file.type);
    tsux.response << file.data;
  }
  else
    tsux.generate(404);
}

void index(Tsux& tsux, void* data){
  header(tsux);
  
  tsux.response << "<h1>Index</h1>"
                << "<p>This is an example page</p>";

  footer(tsux);
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
  
  while(tsux.accept()){
    tsux.dispatch();
    tsux.end();
  }

  return 0;
}
