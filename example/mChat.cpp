
#include "mChat.hpp"

mChat::mChat(Tsux& tsux):
    Module(tsux, "chat", SESSION){

    //routes
    bind("^/chat$", &mChat::r_index);
    bind("^/ajax/chat/send", &mChat::r_send);
    bind("^/ajax/chat/list", &mChat::r_list);
    bind("^/chat/logout", &mChat::r_logout);

    //modules
    main = (mMain*)module("main");

    //templates
    t_index.loadFromFile("chat/templates/index.html.tpl");
    t_index.compile();

    t_index.herit(main->t_index);
    t_index.set("chat", &mChat::v_chat, this);

    t_login.loadFromFile("chat/templates/login.html.tpl");
    t_login.compile();
    t_login.herit(t_index);

    t_chat.loadFromFile("chat/templates/chat.html.tpl");
    t_chat.compile();
    t_chat.herit(t_index);
}

/* ROUTES */

void mChat::r_index(){
  //form action
  if(post.has("pseudo")){
    std::map<std::string, User>::iterator it = users.find(ssid());
    if(it != users.end() && !it->second.logged){
      it->second.pseudo = post.get("pseudo","anonymous");
      it->second.logged = true;
    }
  }

  t_index.render(tsux);
}

void mChat::r_send(){
  std::map<std::string, User>::iterator it = users.find(ssid());
  if(it != users.end() && it->second.logged){
    //user logged
    if(messages.size() >= MAX_MESSAGES)
      messages.erase(messages.begin());
    Message msg;
    msg.pseudo = it->second.pseudo;
    msg.content = post.get("message","");
    if(msg.content != "")
      messages.push_back(msg);
  }
}

void mChat::r_list(){
  header.set("Content-Type", "text/plain");
  std::map<std::string, User>::iterator it = users.find(ssid());
  if(it != users.end() && it->second.logged){
    for(int i = it->second.cursor; i < messages.size(); i++)
      response << messages[i].pseudo << " : " << messages[i].content << "\n";
    it->second.cursor = messages.size();
  }
}

void mChat::r_logout(){
  std::map<std::string, User>::iterator it = users.find(ssid());
  if(it != users.end() && it->second.logged){
    it->second.logged = false;
    it->second.cursor = 0;
  }

  header.set("Location", "/chat");
}

/* GEN */

void mChat::v_chat(){
  std::map<std::string, User>::iterator it = users.find(ssid());
  if(it != users.end() && it->second.logged){
    User& user = it->second;
    //logged 
    //set cursor to 0
    user.cursor = 0;
    //display chat
    t_chat.render(tsux);
  }
  else{
    //display login 
    t_login.render(tsux);
  }
}


/* SESSION */

void mChat::onSessionCreate(const std::string& ssid){
  User user;
  user.pseudo = "anonymous";
  user.logged = false;
  user.cursor = 0;
  users.insert(std::pair<std::string, User>(ssid, user));
}

void mChat::onSessionDelete(const std::string& ssid){
  users.erase(users.find(ssid));
}
