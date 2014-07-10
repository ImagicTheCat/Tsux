
#include "Tsux.hpp"

int main(int argc, char** argv){
  Tsux tsux;

  while(tsux.accept()){
    tsux.out << "Content-type: text/html\r\n";
    tsux.out << "\r\n";
    tsux.out << "<h1>Hello world</h2>\n";

    for(int i = 0; i < 100; i++){
      tsux.out << "\n<br/>";
      for(int j = 0; j < 100; j++)
       tsux.out << "x";
    }

    tsux.flush();
  }

  return 0;
}
