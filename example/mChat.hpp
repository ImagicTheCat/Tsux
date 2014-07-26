#ifndef H_MCHAT
#define H_MCHAT

#include <tsux/Module.hpp>
#include "mMain.hpp"

#define MAX_MESSAGES 200

struct User{
  std::string pseudo;
  bool logged;
  int cursor;
};

struct Message{
  std::string pseudo;
  std::string content;
};

class mChat : public Module{
  public:
    mChat(Tsux& tsux);

    /* ROUTES */
    void r_index();
    void r_send();
    void r_list();

    /* GEN */
    void v_chat();

    //session
    void onSessionCreate(const std::string& ssid);
    void onSessionDelete(const std::string& ssid);

  private:
    mMain* main;

    //Templates
    Template t_index, t_login, t_chat;

    std::map<std::string, User> users;
    std::vector<Message> messages;
};

#endif
