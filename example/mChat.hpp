#ifndef H_MCHAT
#define H_MCHAT

#include <tsux/Module.hpp>
#include "mMain.hpp"

class mChat : public Module{
  public:
    mChat(Tsux& tsux);

    void r_index();

  private:
    mMain* main;

    Template t_index;
};

#endif
