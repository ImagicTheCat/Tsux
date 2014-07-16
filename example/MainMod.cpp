
#include "MainMod.hpp"

MainMod::MainMod(Tsux& tsux):Module(tsux,"main"){
  bind("^/main$", &MainMod::c_index);

  tpl.loadFromFile("index.html.tpl");
  tpl.compile();

  tpl.set("title", "Main");
  tpl.set("content", &content);
}

void MainMod::c_index(){
  header.set("Content-Type", "text/html");

  content.append("<br/>coucou");
  response << tpl.render();
}
