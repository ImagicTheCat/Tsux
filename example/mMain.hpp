#ifndef H_MMAIN
#define H_MMAIN

#include <tsux/Module.hpp>
#include <tsux/Dir.hpp>
#include <tsux/Template.hpp>

class mMain : public Module{
  public:
    mMain(Tsux& tsux);

    //routes
    void r_file();
    void r_index();
    void r_info();
    void r_404();

    //super routes
    void s_locale();

    //generations (views?)
    void v_menu();

    //accessors
    Translator& translator(){ return tr; }

    Template t_index;

  private:
    FileSet files;

    Translator tr;

    std::map<std::string, std::string> menu;

    //templates
    Template t_header, t_footer, t_menu_el, t_404;
};

#endif
