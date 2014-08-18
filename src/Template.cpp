/* 
 * VERY simple template system
 * replace variables with the form {{my_variable}}
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#include "Template.hpp"

Template::~Template(){
  clear();
}


void Template::compile(Translator& tr){
  translator = &tr;
  compile();
}

void Template::compile(){
  //init
  clear();

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
          flux.push_back(new TemplatePart(this, tmp));
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
          tmp += '{';

        if(tmp.size() > 0)
          flux.push_back(new TemplatePart(this, tmp));
      }
    }
    else if(mode == PREVAR){
      if(c == '{')
        mode = VAR;
      else{
        mode = NONE;

        //ignore var and go back to plain data
        //don't forget the prev '{'
        tmp += '{';
        tmp += c;
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
        //check existence of this parameter
        std::map<std::string, unsigned int>::iterator it = params.find(tmp);
        if(it != params.end())
          flux.push_back(flux[it->second]); //build a link
        else{
          //check if a translation is possible
          if(translator != NULL && translator->hasTrans(tmp))
            flux.push_back(new TemplatePart(this, translator, tmp));
          else //add parameter
            flux.push_back(new TemplatePart(this));

          params.insert(std::pair<std::string, unsigned int>(tmp, flux.size()-1));
        }


        tmp.clear();
        mode = NONE;
      }
      else{
        //go back to plain data
        tmp += '}';
        tmp += c;
        mode = NONE;
      }
    }
  }
}


void Template::herit(Template& tpl){
  //link empty parts to parent's parts if they exists
  for(int i = 0; i < flux.size(); i++){
    if(flux[i]->type == TemplatePart::EMPTY){
      //resolve part name
      std::string identifier = getIdentifier(flux[i]);
      
      //search in template
      TemplatePart* part = tpl.get(identifier);
      if(part != NULL){
        //create a link
        flux[i]->type = TemplatePart::LINK;
        flux[i]->link = part;
      }
      else{
        //search for parent translation
        Translator* translator = tpl.getTranslator();
        if(translator != NULL && translator->hasTrans(identifier)){
          flux[i]->type = TemplatePart::TRANSLATION;
          flux[i]->plain = identifier;

          //set parent for translation
          flux[i]->parent = &tpl;
        }
      }
    }
  }
}

void Template::render(Tsux& tsux){
  for(int i = 0; i < flux.size(); i++)
    render(tsux, flux[i]);
}


void Template::render(Tsux& tsux, TemplatePart* part){
  if(part->type == TemplatePart::PLAIN)
    tsux.response << part->plain;
  else if(part->type == TemplatePart::POINTER)
    tsux.response << (*(part->pointer));
  else if(part->type == TemplatePart::TRANSLATION)
    tsux.response << part->parent->getTranslator()->trans(part->plain);
  else if(part->type == TemplatePart::ACTION)
    part->action.execute(tsux);
  else if(part->type == TemplatePart::TEMPLATE)
    part->tpl->render(tsux);
  else if(part->type == TemplatePart::LINK)
    part->link->parent->render(tsux, part->link);
}

void Template::set(const std::string& name, const std::string& data){
  std::map<std::string, unsigned int>::iterator it = params.find(name);
  if(it != params.end()){
    flux[it->second]->type = TemplatePart::PLAIN;
    flux[it->second]->plain = data;
  }
}

void Template::set(const std::string& name, std::string* data){
  std::map<std::string, unsigned int>::iterator it = params.find(name);
  if(it != params.end()){
    flux[it->second]->type = TemplatePart::POINTER;
    flux[it->second]->pointer = data;
  }
}

void Template::set(const std::string& name, TsuxAction act, void* data){
  std::map<std::string, unsigned int>::iterator it = params.find(name);
  if(it != params.end()){
    flux[it->second]->type = TemplatePart::ACTION;
    flux[it->second]->action = Action(act, data);
  }
}

void Template::set(const std::string& name, Template& tpl){
  std::map<std::string, unsigned int>::iterator it = params.find(name);
  if(it != params.end()){
    flux[it->second]->type = TemplatePart::TEMPLATE;
    flux[it->second]->tpl = &tpl;
  }
}


TemplatePart* Template::get(const std::string& name){
  std::map<std::string, unsigned int>::iterator it = params.find(name);
  if(it != params.end())
    return flux[params.find(name)->second];
  else
    return NULL;
}


std::string Template::getIdentifier(TemplatePart* part){
  /* resolve part name */

  //search same part
  for(int i = 0; i < flux.size(); i++){
    if(flux[i] == part){
      //search index correspondance
      std::map<std::string, unsigned int>::iterator it = params.begin();
      while(it != params.end()){
        if(it->second == i)
          return it->first;

        it++;
      }
    }
  }

  return "";
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

void Template::clear(){
  for(std::map<std::string, unsigned int>::iterator it = params.begin(); it != params.end(); it++)
    delete flux[it->second];
  params.clear();
  flux.clear();
}
