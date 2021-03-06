/* 
 * VERY simple template system
 * replace variables with the form {{my_variable}}
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#ifndef H_TEMPLATE
#define H_TEMPLATE

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "Translator.hpp"
#include "Tsux.hpp"

class Template;

//handle pointer and plain data
struct TemplatePart{
  enum{
    EMPTY = 0,
    POINTER = 1,
    PLAIN = 2,
    TRANSLATION = 3,
    ACTION = 4,
    TEMPLATE = 5,
    LINK = 6,
    SUBTEMPLATE = 7
  };

  TemplatePart(Template* p): subtpl(NULL), parent(p), type(EMPTY){}

  //string and string pointer
  TemplatePart(Template* p, const std::string& data): subtpl(NULL),parent(p), type(PLAIN), plain(data){}
  TemplatePart(Template* p, std::string* data): subtpl(NULL),parent(p), type(POINTER), pointer(data){}

  //translator
  TemplatePart(Template* p,Translator* tr, const std::string& name): subtpl(NULL),parent(p), type(TRANSLATION), plain(name){}

  ~TemplatePart(){
    if(subtpl != NULL)
      delete subtpl;
  }

  Action action;
  std::string plain;
  std::string* pointer;
  Template* tpl;
  Template* parent;
  TemplatePart* link;
  Template* subtpl;

  int type;
};

class Template{
  public:
    Template():translator(NULL), base(NULL){}
    ~Template();

    //extend from a base template
    //change the flux behaviour of the template
    void extend(Template& tpl){ base = &tpl; }

    //compile the data string into a template flux
    void compile();

    //compile with a translator
    void compile(Translator& tr);

    //herit identifiers from another template
    void herit(Template& tpl);

    void clear();

    //render the template : write string internally for performance
    void render(Tsux& tsux);

    //render a part
    void render(Tsux& tsux, TemplatePart* part);

    //load file into the data string
    bool loadFromFile(const std::string& path);

    Translator* getTranslator(){ return translator; }

    //set variables for rendering (need a compilation before using)
    void set(const std::string& name, const std::string& data);
    void set(const std::string& name, std::string* data);

    //tsux action
    void set(const std::string& name, TsuxAction act, void* data = NULL);

    //module action
    template<typename T> void set(const std::string& name, void (T::*act)(void), Module* module){
      std::map<std::string, unsigned int>::iterator it = params.find(name);
      if(it != params.end()){
        flux[it->second]->type = TemplatePart::ACTION;
        flux[it->second]->action = Action((ModAction)act, module);
      }
      else{
        //spread set to subtemplates
        for(it = params.begin(); it != params.end(); it++){
          if(flux[it->second]->type == TemplatePart::SUBTEMPLATE)
            flux[it->second]->subtpl->set(name, act, module);
        }
      }
    }

    //template
    void set(const std::string& name, Template& tpl);

    //set any type converted to string
    template<typename T> void set(const std::string& param, const T& val){
      std::stringstream ss;
      ss << val;
      set(param, ss.str());
    }



    //get part by name
    TemplatePart* get(const std::string& name);

    //resolve part name by part pointer
    std::string getIdentifier(TemplatePart* part);

    std::vector<TemplatePart*>& getFlux(){ return flux; }

    std::string data;

  private:
    //links to the part of the flux
    std::map<std::string, unsigned int> params;

    //parts
    std::vector<TemplatePart*> flux;

    //translator
    Translator* translator;

    //base template
    Template* base;
};

#endif
