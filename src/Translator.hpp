#ifndef H_TRANSLATOR
#define H_TRANSLATOR

#include <iostream>
#include <map>
#include <vector>
#include <fstream>

struct TranslatorGroup{
  ~TranslatorGroup();
  void clear();

  void set(const std::string& key, TranslatorGroup* el);
  TranslatorGroup* get(const std::string& key);

  std::map<std::string, TranslatorGroup*> els;
};

class Translator{
  public:
    void load(const std::string& data);
    bool loadFromFile(const std::string& path);

    bool has(const std::string& path);
    bool hasTrans(const std::string& path);

    //if not found, return the path
    const std::string& get(const std::string& path);

    //trans with locale
    std::string trans(const std::string& path);

    void set(const std::string& path, const std::string& value);
    void setLocale(const std::string& _locale){ locale = _locale; }
    
  private:
    void parse(const std::string& data);
    void buildPath(const std::string& path, std::vector<std::string>& cpath);

    TranslatorGroup root;
    std::string locale;
};

#endif
