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

    //super routes
    void s_locale();

    //generations (views?)
    void v_header();
    void v_footer();
    void v_menu();

    //accessors
    Translator& translator(){ return tr; }

  private:
    FileSet files;

    Translator tr;

    //templates
    Template t_header, t_footer, t_index, t_menu_el;

    std::map<std::string, std::string> menu;
};

#endif
