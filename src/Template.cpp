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

  //check the begin/end brackets
  int be_count = 0;

  std::string tmp;

  for(int i = 0; i < data.size(); i++){
    char& c = data[i];
    if(c == '{')
      be_count++;
    else if(c == '}')
      be_count--;
    
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
      if(c == '}' && be_count == 1)
        mode = SUFVAR;
      else
        tmp += c;
    }
    else if(mode == SUFVAR){
      if(c == '}'){
        //new var

        //check if it's a subtemplate definition
        std::string name;
        std::string content;
        bool mode_content = false;
        for(int j = 0; j < tmp.size(); j++){
          const char& c = tmp[j];
          if(!mode_content){
            if(c == '=')
              mode_content = true;
            else
              name += c;
          }
          else
            content += c;
        }

        tmp = name;

        //check existence of this parameter
        std::map<std::string, unsigned int>::iterator it = params.find(tmp);
        if(it != params.end()){
          //build a link template
          TemplatePart* tp = new TemplatePart(this);
          tp->type = TemplatePart::LINK;
          tp->link = flux[it->second];
          flux.push_back(tp);
        }
        else{
          //subtemplate
          if(mode_content){
            TemplatePart* tp = new TemplatePart(this);
            tp->type = TemplatePart::SUBTEMPLATE;
            tp->subtpl = new Template();
            tp->subtpl->data = content;
            tp->subtpl->compile();

            flux.push_back(tp);
          }
          //check if a translation is possible
          else if(translator != NULL && translator->hasTrans(tmp))
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

  if(base == NULL){
    //herit from this in all subtemplates
    for(int i = 0; i < flux.size(); i++){
      if(flux[i]->type == TemplatePart::SUBTEMPLATE)
        flux[i]->subtpl->herit(*this);
    }
  }
  else{
    //this template is an extension of another
    //keep only the redefinitions
    std::map<std::string, TemplatePart*> parts;

    //save redefinitions
    for(int i = flux.size()-1; i >= 0; i--){
      if(flux[i]->type == TemplatePart::SUBTEMPLATE){
        std::string identifier = getIdentifier(flux[i]);
        if(base->get(identifier) != NULL){
          //herit from base template
          flux[i]->subtpl->herit(*base); 

          //save part
          parts.insert(std::pair<std::string, TemplatePart*>(identifier, flux[i]));

          //remove from list
          flux.erase(flux.begin()+i);
        }
      }
    }

    //fill flux with base flux
    clear();
    std::vector<TemplatePart*>& base_flux = base->getFlux();
    for(int i = 0; i < base_flux.size(); i++){
      TemplatePart* tp = new TemplatePart(this);
      tp->type = TemplatePart::LINK;
      tp->link = base_flux[i];
      flux.push_back(tp);
    }

    //replace with new parts
    for(std::map<std::string, TemplatePart*>::iterator it = parts.begin(); it != parts.end(); it++){
      TemplatePart *old = base->get(it->first);
      for(int i = 0; i < base_flux.size(); i++){
        if(base_flux[i] == old){
          delete flux[i];
          flux[i] = it->second;
          params.insert(std::pair<std::string, unsigned int>(it->first, i));
          parts.erase(it);
        }
      }
    }

    //clear unused parts
    for(std::map<std::string,TemplatePart*>::iterator it = parts.begin(); it != parts.end(); it++)
      delete it->second;
  }
}


void Template::herit(Template& tpl){
  for(int i = 0; i < flux.size(); i++){
    //herit from tpl to all subtemplates
    if(flux[i]->type == TemplatePart::SUBTEMPLATE)
      flux[i]->subtpl->herit(tpl);
    //link empty parts to parent's parts if they exists
    else{
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
  else if(part->type == TemplatePart::SUBTEMPLATE)
    part->subtpl->render(tsux);
}

void Template::set(const std::string& name, const std::string& data){
  std::map<std::string, unsigned int>::iterator it = params.find(name);
  if(it != params.end()){
    flux[it->second]->type = TemplatePart::PLAIN;
    flux[it->second]->plain = data;
  }
  else{
    //spread set to subtemplates
    for(it = params.begin(); it != params.end(); it++){
      if(flux[it->second]->type == TemplatePart::SUBTEMPLATE)
        flux[it->second]->subtpl->set(name, data);
    }
  }
}

void Template::set(const std::string& name, std::string* data){
  std::map<std::string, unsigned int>::iterator it = params.find(name);
  if(it != params.end()){
    flux[it->second]->type = TemplatePart::POINTER;
    flux[it->second]->pointer = data;
  }
  else{
    //spread set to subtemplates
    for(it = params.begin(); it != params.end(); it++){
      if(flux[it->second]->type == TemplatePart::SUBTEMPLATE)
        flux[it->second]->subtpl->set(name, data);
    }
  }
}

void Template::set(const std::string& name, TsuxAction act, void* data){
  std::map<std::string, unsigned int>::iterator it = params.find(name);
  if(it != params.end()){
    flux[it->second]->type = TemplatePart::ACTION;
    flux[it->second]->action = Action(act, data);
  }
  else{
    //spread set to subtemplates
    for(it = params.begin(); it != params.end(); it++){
      if(flux[it->second]->type == TemplatePart::SUBTEMPLATE)
        flux[it->second]->subtpl->set(name, act, data);
    }
  }
}

void Template::set(const std::string& name, Template& tpl){
  std::map<std::string, unsigned int>::iterator it = params.find(name);
  if(it != params.end()){
    flux[it->second]->type = TemplatePart::TEMPLATE;
    flux[it->second]->tpl = &tpl;
  }
  else{
    //spread set to subtemplates
    for(it = params.begin(); it != params.end(); it++){
      if(flux[it->second]->type == TemplatePart::SUBTEMPLATE)
        flux[it->second]->subtpl->set(name, tpl);
    }
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
  for(int i = 0; i < flux.size(); i++)
    delete flux[i];
  params.clear();
  flux.clear();
}
