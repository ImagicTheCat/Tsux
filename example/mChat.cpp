
#include "mChat.hpp"

mChat::mChat(Tsux& tsux):
    Module(tsux, "chat"){

    //routes
    bind("^/chat$", &mChat::r_index);

    //modules
    main = (mMain*)module("main");

    //templates
    t_index.loadFromFile("chat/templates/index.html.tpl");
    t_index.compile(main->translator());

    t_index.set("header", &mMain::v_header, main);
    t_index.set("footer", &mMain::v_footer, main);
}

void mChat::r_index(){
  t_index.render(tsux);
}
