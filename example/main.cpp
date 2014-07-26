
#include <tsux/Tsux.hpp>
#include "mMain.hpp"
#include "mChat.hpp"

int main(int argc, char** argv){
  Tsux tsux;

  tsux.enable(Tsux::REGEX_ROUTING);
  tsux.enable(Tsux::SESSION);

  mMain main(tsux);
  mChat chat(tsux);

  while(tsux.accept()){
    //default content type
    tsux.header.set("Content-Type", "text/html");

    tsux.dispatch();
    tsux.end();
  }

  return 0;
}
