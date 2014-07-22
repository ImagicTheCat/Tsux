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

//handle pointer and plain data
struct TemplatePart{
  enum{
    POINTER = 0,
    PLAIN = 1,
    TRANSLATION = 2
  };

  TemplatePart(const std::string& data):type(PLAIN), plain(data){}
  TemplatePart(std::string* data): type(POINTER), pointer(data){}
  TemplatePart(Translator* tr, const std::string& name): type(TRANSLATION), plain(name){}

  std::string plain;
  std::string* pointer;
  int type;
};

class Template{
  public:
    Template():translator(NULL){}
    ~Template();

    //compile the data string into a template flux
    void compile();

    //compile with a translator
    void compile(Translator& tr);

    void clear();

    //render the template : write string internally for performance
    std::string& render();

    //load file into the data string
    bool loadFromFile(const std::string& path);

    //set variables for rendering (need a compilation before use)
    void set(const std::string& name, const std::string& data);
    void set(const std::string& name, std::string* data);

    std::string data, rendered_data;

  private:
    //links to the part of the flux
    std::map<std::string, unsigned int> params;

    //parts
    std::vector<TemplatePart*> flux;

    //translator
    Translator* translator;
};

#endif
