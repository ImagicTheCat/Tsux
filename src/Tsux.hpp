/* 
 * Tsux
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#ifndef H_TSUX
#define H_TSUX

#include <iostream>
#include <sstream>
#include <fcgio.h>
#include <ctime>
#include <cstdlib>

#include "ParamSet.hpp"
#include "Action.hpp"
#include "Regex.hpp"
#include "URI.hpp"
#include "FileSet.hpp"
#include "MIMEType.hpp"
#include "HTML.hpp"
#include "Base.hpp"


class Tsux{
  public:
    //options
    enum{
      REGEX_ROUTING = 1,
      SESSION = 2
    };


    Tsux();
    ~Tsux();

    //methods
    bool accept();
    void end();

    //bind tsux action
    void bind(const std::string& path, TsuxAction action, void* data = NULL);
    void bind(const std::string& path, const Action& action);

    /* modules managment */
    void registerModule(const std::string& name, Module* module);
    void unregisterModule(const std::string& name);
    //get module by name, if not found return NULL
    Module* module(const std::string& name);

    /* session management */
    void registerSessionListener(Module* module){ session_modules.push_back(module); }
    void bindSessionCreate(void (*f)(const std::string&)){ f_session_create = f; }
    void bindSessionDelete(void (*f)(const std::string&)){ f_session_delete = f; }

    void setSessionTime(unsigned int time){ session_time = time; }
    unsigned int getSessionTime(){ return session_time; }

    const std::string& ssid(){ return _ssid; }


    //rewrite route
    void rewrite(const std::string& route);

    //eval possible routes
    void dispatch();

    //time in seconds
    void createCookie(const std::string& name, const std::string& data, int time);

    //generate html : 404, etc
    void generate(int code);
    void generate(ParamSet set);
    void generate(FileSet set);

    void bindCode(int code, TsuxAction func, void *data = NULL);
    void bindCode(int code, const Action& action);

    //defaults codes
    static void default_404(Tsux& tsux, void *data);

    //options methods
    void enable(unsigned int options);
    void disable(unsigned int options);
    bool enabled(unsigned int option);

    //accessors
    const std::string& uri()const{ return _uri; }
    const std::string& location()const{ return _location; }
    const std::string& path()const{ return _path; }

    const std::string& locale()const{ return _locale; }
    void locale(const std::string& loc){ _locale = loc; }

    //streams
    std::ostream out;
    std::istream in;
    std::ostream err;
    std::stringstream response;

    //data
    ParamSet post, get, header, route, param, cookie;
    FileSet file;

    //tools
    static void parseURICouples(const std::string& uri, ParamSet& pset);
    static std::string deleteChars(const std::string& str, const std::string& chars);

    //parse field like [name: value; opt1=val1; opt2="val2"]
    static void parseMIMEField(const std::string& field, std::string& name, std::string& value, ParamSet& options);

    //unreference data void pointer
    template<typename T> static T& ref(void* data, T& def){
      if(data != NULL)
        return (T&)(*((T*)data));
      else
        return def;
    }


  private:
    void initBufs();
    void parseMIMEData();

    //session
    std::string generateSSID();
    bool checkSSID(const std::string& ssid);
    void createSession();
    void deleteSession(std::map<std::string, unsigned long int>::iterator it);
    void checkSessions();
    unsigned long int check_timer;
    //time between two sessions check
    int check_timer_interval;

    std::stringstream header_stream;

    FCGX_Request request;
    fcgi_streambuf *sin, *sout, *serr;

    std::string _uri, _location, _locale, _path;

    //session
    std::string _ssid, ssid_alpha;


    //modules
    std::map<std::string, Module*> modules;

    //sessions
    //ssid and start time
    std::map<std::string, unsigned long int> sessions;
    //listener
    std::vector<Module*> session_modules;
    //global 
    void (*f_session_create)(const std::string&);
    void (*f_session_delete)(const std::string&);

    //simple routes
    std::map<std::string, Action> routes;

    //regex routes
    std::vector<Regex*> regs;
    std::vector<Action> actions;
    std::vector<bool> locks;

    //code generations
    std::map<int, Action> codes;

    unsigned int session_time;

    unsigned int options;

    bool flushed;
};

#endif
