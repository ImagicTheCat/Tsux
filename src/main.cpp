
#include "Tsux.hpp"

int main(int argc, char** argv){
  Tsux tsux;

  while(tsux.accept()){
    tsux.out << "Content-type: text/html\r\n";
    tsux.out << "\r\n";
    tsux.out << "<h1>Hello world</h1>\n";
    tsux.out << "<p>Your ip is <strong>" << tsux.param("REMOTE_ADDR") << "</strong></p>";
    tsux.out << "<p>The URI is <strong>" << tsux.param("REQUEST_URI") << "</strong></p>";
    tsux.out << "<p>Nothin is <strong>" << tsux.param("nothing") << "</strong></p>";

    tsux.flush();
  }

  return 0;
}
