
#include "Translator.hpp"

/* TRANSLATOR GROUP */
TranslatorGroup::~TranslatorGroup(){
  clear();
}

void TranslatorGroup::clear(){
  std::map<std::string, TranslatorGroup*>::iterator it = els.begin();
  while(it != els.end()){
    delete it->second;
    it++;
  }
}


void TranslatorGroup::set(const std::string& key, TranslatorGroup* el){
  std::map<std::string, TranslatorGroup*>::iterator it = els.find(key);
  if(it != els.end()){
    delete it->second;
    els.erase(it);
  }

  els.insert(std::pair<std::string, TranslatorGroup*>(key, el));
}


TranslatorGroup* TranslatorGroup::get(const std::string& key){
  std::map<std::string, TranslatorGroup*>::iterator it = els.find(key);
  if(it != els.end())
    return it->second;
  else
    return NULL;
}

/* TRANSLATOR */


void Translator::buildPath(const std::string& path, std::vector<std::string>& cpath){
  //parse path
  std::string tmp;
  for(int i = 0; i < path.size(); i++){
    const char& c = path[i];
    bool end = (i == path.size()-1);

    if(c == '.'){
      cpath.push_back(tmp);
      tmp ="";
    }
    else if(end){
      tmp += c;
      cpath.push_back(tmp);
    }
    else
      tmp += c;
  }
}


bool Translator::has(const std::string& path){
  //build path
  std::vector<std::string> cpath;
  buildPath(path, cpath);

  //explode groups
  TranslatorGroup* current = &root;
  int i = 0;
  while(current != NULL && i < cpath.size()){
    current = current->get(cpath[i]);
    i++;
  }

  return (i == cpath.size() && current != NULL && current->els.size() > 0
      && current->els.begin()->second == NULL);
}

const std::string& Translator::get(const std::string& path){
  //build path
  std::vector<std::string> cpath;
  buildPath(path, cpath);

  //explode groups
  TranslatorGroup* current = &root;
  int i = 0;
  while(current != NULL && i < cpath.size()){
    current = current->get(cpath[i]);
    i++;
  }

  //search the value
  if(i == cpath.size() && current != NULL && current->els.size() > 0 
      && current->els.begin()->second == NULL)
    return current->els.begin()->first;
  else
    return path;
}


bool Translator::hasTrans(const std::string& path){
  //build path
  std::vector<std::string> cpath;
  buildPath(path, cpath);

  //check all first domain of translation
  std::map<std::string, TranslatorGroup*>::iterator it = root.els.begin();
  while(it != root.els.end()){
    //explode groups
    TranslatorGroup* current = it->second;
    int i = 0;
    while(current != NULL && i < cpath.size()){
      current = current->get(cpath[i]);
      i++;
    }

    return (i == cpath.size() && current != NULL && current->els.size() > 0
        && current->els.begin()->second == NULL);
  }

  return false;
}

std::string Translator::trans(const std::string& path){
  if(has(locale+"."+path))
    return get(locale+"."+path);
  else if(has("all."+path))
    return get("all."+path);
  else 
    return path;
}

void Translator::set(const std::string& path, const std::string& value){
  //build path
  std::vector<std::string> cpath;
  buildPath(path, cpath);

  //explode groups
  TranslatorGroup* current = &root;
  for(int i = 0; i < cpath.size(); i++){
    TranslatorGroup* next = current->get(cpath[i]);

    //create a new group or delete if last
    if(next == NULL || (i == cpath.size()-1)){
      next = new TranslatorGroup;
      current->set(cpath[i], next);
    }

    current = next;
  }

  //set the value
  current->set(value, NULL);
}


void Translator::parse(const std::string& data){
  std::string path;
  std::string tmp;
  std::string id;

  const int ID = 0;
  const int VAL = 1;

  std::string prev_id;

  int mode = ID;
  int prev_rank = 0;
  int rank = 0;
  bool ranked = false;

  std::vector<std::string> lifo;

  for(int i = 0; i < data.size(); i++){
    const char& c = data[i];
    bool end = (i == data.size()-1);

    if(mode == ID){
      //new line, end id
      if(c == '\n' || c == '='){
        //id not null
        if(tmp.size() > 0){
          //a value
          if(c == '='){
            id = tmp;
            mode = VAL;
          }


          //rank explode
           //go highter
          if(rank > prev_rank){
            path += prev_id;
            path += ".";
            lifo.push_back(prev_id);
          }
          //go lower
          else if(rank < prev_rank){
            if(lifo.size() > 0)
              prev_id = lifo[lifo.size()-1];
            path = path.substr(0, path.size()-prev_id.size()-1);
            lifo.pop_back();
          }

          if(mode == ID)
            prev_id = tmp;
          prev_rank = rank;


          ranked = false;
          rank = 0;
          tmp = "";
        }
      }
      else if(c == ' ' || c == '\t'){
        //update rank
        if(!ranked)
          rank++;
      }
      else{
        if(!ranked)
          ranked = true;
        tmp += c;
      }
    }
    else if(mode == VAL){
      //new value, go back to id mode
      if(c == '\n'){
        set(path+id, tmp);
        tmp ="";
        mode = ID;
      }
      else
        tmp += c;
    }
  }
}


void Translator::load(const std::string& data){
  parse(data);
}

bool Translator::loadFromFile(const std::string& path){
  std::ifstream file(path.c_str(), std::ios::binary);
  std::string data;
  if(file){
    //get content
    data.assign(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>());
    parse(data);
    return true;
  }
  else
    return false;
}
