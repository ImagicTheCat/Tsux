#ifndef H_MAINMOD
#define H_MAINMOD

#include <tsux/Module.hpp>

class MainMod : public Module{
  public:
    MainMod(Tsux& tsux);

    void c_index();

  private:
    std::string messages;
};

#endif
