#ifndef H_CONFIG
#define H_CONFIG

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

struct ConfigGroup{
  ~ConfigGroup();
  void clear();

  void set(const std::string& key, ConfigGroup* el);
  ConfigGroup* get(const std::string& key);

  std::map<std::string, ConfigGroup*> els;
};

class Config{
  public:
    void load(const std::string& data);
    bool loadFromFile(const std::string& path);

    bool has(const std::string& path);

    //if not found, return the path
    const std::string& get(const std::string& path);

    void set(const std::string& path, const std::string& value);

    //template set/get
    template<typename T> T get(const std::string& param, const T& def) {
      if(has(param)){
        T val(0);
        std::stringstream ss(get(param));
        ss >> val;

        return val;
      }
      else
        return def;
    }


    template<typename T> void set(const std::string& param, const T& val){
      std::stringstream ss;
      ss << val;
      set(param, ss.str());
    }



  protected:
    void parse(const std::string& data);
    void buildPath(const std::string& path, std::vector<std::string>& cpath);

    ConfigGroup root;
};

#endif
