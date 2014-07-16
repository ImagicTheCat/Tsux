/* 
 * VERY simple template system
 * replace variables with the form {{my_variable}}
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#include "Template.hpp"

void Template::compile(){
  //init
  params.clear();
  flux.clear();

  const int NONE = 0;
  const int PREVAR = 1;
  const int VAR = 2;
  const int SUFVAR = 3;

  int mode = NONE;

  std::string tmp;

  for(int i = 0; i < data.size(); i++){
    char& c = data[i];
    
    if(mode == NONE){
      if(c == '{'){
        if(tmp.size() > 0){
          //add new plain data to the flux
          flux.push_back(TemplatePart(tmp));
          tmp.clear();
        }

        mode = PREVAR;
      }
      else
        tmp += c;

      //reach end
      if(i == data.size()-1){
        //don't forget the '{' of a final declaration begin
        if(mode == PREVAR)
          tmp += "{";

        if(tmp.size() > 0)
          flux.push_back(TemplatePart(tmp));
      }
    }
    else if(mode == PREVAR){
      if(c == '{')
        mode = VAR;
      else{
        mode = NONE;

        //ignore var and go back to plain data
        //don't forget the prev '{'
        tmp += "{"+c;
      }
    }
    else if(mode == VAR){
      if(c == '}')
        mode = SUFVAR;
      else
        tmp += c;
    }
    else if(mode == SUFVAR){
      if(c == '}'){
        //add var
        flux.push_back(TemplatePart(""));
        params.insert(std::pair<std::string, unsigned int>(tmp, flux.size()-1));
        tmp.clear();
        mode = NONE;
      }
      else{
        //go back to plain data
        tmp += "}"+c;
        mode = NONE;
      }
    }
  }
}

std::string& Template::render(){
  rendered_data.clear();
  for(int i = 0; i < flux.size(); i++){
    if(flux[i].type == TemplatePart::PLAIN)
      rendered_data.append(flux[i].plain);
    else if(flux[i].type == TemplatePart::POINTER)
      rendered_data.append(*(flux[i].pointer));
  }

  return rendered_data;
}

void Template::set(const std::string& name, const std::string& data){
  std::map<std::string, unsigned int>::iterator it = params.find(name);
  if(it != params.end()){
    flux[it->second].type = TemplatePart::PLAIN;
    flux[it->second].plain = data;
  }
}

void Template::set(const std::string& name, std::string* data){
  std::map<std::string, unsigned int>::iterator it = params.find(name);
  if(it != params.end()){
    flux[it->second].type = TemplatePart::POINTER;
    flux[it->second].pointer = data;
  }
}

bool Template::loadFromFile(const std::string& path){
  std::ifstream file(path.c_str(), std::ios::binary);
  if(file){
    data.clear();

    //get content
    data.assign(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>());

    return true;
  }
  else
    return false;
}

