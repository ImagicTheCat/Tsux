
#include "Config.hpp"

/* CONFIG GROUP */
ConfigGroup::~ConfigGroup(){
  clear();
}

void ConfigGroup::clear(){
  std::map<std::string, ConfigGroup*>::iterator it = els.begin();
  while(it != els.end()){
    delete it->second;
    it++;
  }
}


void ConfigGroup::set(const std::string& key, ConfigGroup* el){
  std::map<std::string, ConfigGroup*>::iterator it = els.find(key);
  if(it != els.end()){
    delete it->second;
    els.erase(it);
  }

  els.insert(std::pair<std::string, ConfigGroup*>(key, el));
}


ConfigGroup* ConfigGroup::get(const std::string& key){
  std::map<std::string, ConfigGroup*>::iterator it = els.find(key);
  if(it != els.end())
    return it->second;
  else
    return NULL;
}

/* CONFIG */

void Config::buildPath(const std::string& path, std::vector<std::string>& cpath){
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


bool Config::has(const std::string& path){
  //build path
  std::vector<std::string> cpath;
  buildPath(path, cpath);

  //explode groups
  ConfigGroup* current = &root;
  int i = 0;
  while(current != NULL && i < cpath.size()){
    current = current->get(cpath[i]);
    i++;
  }

  return (i == cpath.size() && current != NULL && current->els.size() > 0
      && current->els.begin()->second == NULL);
}

const std::string& Config::get(const std::string& path){
  //build path
  std::vector<std::string> cpath;
  buildPath(path, cpath);

  //explode groups
  ConfigGroup* current = &root;
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


void Config::set(const std::string& path, const std::string& value){
  //build path
  std::vector<std::string> cpath;
  buildPath(path, cpath);

  //explode groups
  ConfigGroup* current = &root;
  for(int i = 0; i < cpath.size(); i++){
    ConfigGroup* next = current->get(cpath[i]);

    //create a new group or delete if last
    if(next == NULL || (i == cpath.size()-1)){
      next = new ConfigGroup;
      current->set(cpath[i], next);
    }

    current = next;
  }

  //set the value
  current->set(value, NULL);
}


void Config::parse(const std::string& data){
  std::string path;
  std::string tmp;
  std::string id;

  const int ID = 0;
  const int VAL = 1;

  std::string prev_id;

  int mode = ID;
  int prev_rank = 0;
  int rank = 0;
  int tab = 0;
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
            int diff = prev_rank-rank;
            int n = diff/tab;
            for(int j = 0; j < n; j++){
              if(lifo.size() > 0)
                prev_id = lifo[lifo.size()-1];
              path = path.substr(0, path.size()-prev_id.size()-1);
              lifo.pop_back();
            }
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

        //define tab spaces fo this file
        if(tab == 0)
          tab = rank;

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


void Config::load(const std::string& data){
  parse(data);
}

bool Config::loadFromFile(const std::string& path){
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
