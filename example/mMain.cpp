
#include "mMain.hpp"

mMain::mMain(Tsux& tsux) : 
  Module(tsux, "main"){

  //routes
  bind("^(.*)$", &mMain::s_locale);
  bind("^/$", &mMain::r_index);
  bind("^/static/(.*)$", &mMain::r_file);
  bind("^/info$", &mMain::r_info);
  bind("^/base$", &mMain::r_base);

  //translations
  tr.loadFromFile("main/translations/all");

  //templates
  t_header.loadFromFile("main/templates/header.html.tpl");
  t_header.compile(tr);
  t_header.set("menu", &mMain::v_menu, this);

  t_footer.loadFromFile("main/templates/footer.html.tpl");
  t_footer.compile(tr);

  t_index.loadFromFile("main/templates/index.html.tpl");
  t_index.compile(tr);
  t_index.set("header", t_header);
  t_index.set("footer", t_footer);

  t_menu_el.loadFromFile("main/templates/menu_el.html.tpl");
  t_menu_el.compile(tr);

  t_404.loadFromFile("main/templates/404.html.tpl");
  t_404.compile();
  t_404.herit(t_index);

  //test
  t_base.loadFromFile("main/templates/base.html.tpl");
  t_base.compile();
  t_base.herit(t_header);

  t_subbase.loadFromFile("main/templates/subbase.html.tpl");
  t_subbase.extend(t_base);
  t_subbase.compile();

  t_subsubbase.loadFromFile("main/templates/subsubbase.html.tpl");
  t_subsubbase.extend(t_subbase);
  t_subsubbase.compile();

  //build menu
  menu.insert(std::pair<std::string, std::string>("/","menu.index"));
  menu.insert(std::pair<std::string, std::string>("/chat","menu.chat"));
  menu.insert(std::pair<std::string, std::string>("https://github.com/ImagicTheCat/Tsux","menu.github"));


  //load static files
  std::vector<std::string> list;
  Dir::explode("static/", list, Dir::SFILE | Dir::RECURSIVE);
  for(int i = 0; i < list.size(); i++){
    std::string filepath = list[i].substr(7);
    File& file = files.alloc(filepath);
    file.loadFromFile(list[i]);
  }

  //custom codes
  bindCode(404, &mMain::r_404);
}

/* ROUTES */

void mMain::r_file(){
  std::string filepath = route.get("1","");
  if(files.has(filepath)){
    header.set("Cache-Control", "max-age=15000");

    File def;
    File& file = files.get(filepath, def);
    header.set("Content-Type",file.type);
    response << file.data;
  }
  else
    generate(404);
}

void mMain::r_404(){
  header.set("Status", "404 Not Found");
  t_404.render(tsux);
}

void mMain::r_index(){
  t_index.render(tsux);
}

void mMain::r_info(){
  generate(param);
}

void mMain::r_base(){
  if(get.has("sub"))
    t_subbase.render(tsux);
  else if(get.has("subsub"))
    t_subsubbase.render(tsux);
  else
    t_base.render(tsux);
}

/* SUPER ROUTES */
void mMain::s_locale(){
  tr.setLocale(locale());

  rewrite(route.get("1","/"));
}

/* GENERATIONS */

void mMain::v_menu(){
  for(std::map<std::string, std::string>::iterator it = menu.begin(); it != menu.end(); it++){
    t_menu_el.set("route", it->first);
    t_menu_el.set("name", tr.trans(it->second));

    //current
    if(it->first == path())
      t_menu_el.set("current_class", "class=\"current\"");
    else 
      t_menu_el.set("current_class", "");

    t_menu_el.render(tsux);
  }
}
