#ifndef H_MAINMOD
#define H_MAINMOD

#include <tsux/Module.hpp>
#include <tsux/Template.hpp>

class MainMod : public Module{
  public:
    MainMod(Tsux& tsux);

    void c_index();

  private:
    std::string messages;
    std::string content;
    Template tpl;
};

#endif
