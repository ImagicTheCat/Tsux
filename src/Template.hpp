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

//handle pointer and plain data
struct TemplatePart{
  enum{
    POINTER,
    PLAIN
  };

  TemplatePart(const std::string& data):type(PLAIN), plain(data){}
  TemplatePart(std::string* data): type(POINTER), pointer(data){}

  std::string plain;
  std::string* pointer;
  int type;
};

class Template{
  public:
    ~Template();

    //compile the data string into a template flux
    void compile();

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
};

#endif
