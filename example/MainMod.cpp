
#include "MainMod.hpp"

MainMod::MainMod(Tsux& tsux):Module(tsux,"main"){
  bind("^/main$", &MainMod::c_index);
}

void MainMod::c_index(){
  response << "<h1>Main</h1>";
}
