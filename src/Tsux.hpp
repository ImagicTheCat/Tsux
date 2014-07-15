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

#include "ParamSet.hpp"
#include "Action.hpp"
#include "Regex.hpp"
#include "URI.hpp"
#include "FileSet.hpp"
#include "MIMEType.hpp"


class Tsux{
  public:
    //options
    enum{
      REGEX_ROUTING = 1
    };


    Tsux();
    ~Tsux();

    //methods
    bool accept();
    void end();

    //bind tsux action
    void bind(const std::string& path, TsuxAction action, void* data = NULL);
    void bind(const std::string& path, const Action& action);


    //eval possible routes
    void dispatch();

    //time in seconds
    void createCookie(const std::string& name, const std::string& data, int time);

    //generate html : 404, etc
    void generate(int code);
    void generate(ParamSet set);
    void generate(FileSet set);

    //options methods
    void enable(unsigned int options);
    void disable(unsigned int options);
    bool enabled(unsigned int option);

    //accessors
    const std::string& uri()const{ return _uri; }
    const std::string& location()const{ return _location; }

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

    std::stringstream header_stream;

    FCGX_Request request;
    fcgi_streambuf *sin, *sout, *serr;

    std::string _uri, _location;

    //simple routes
    std::map<std::string, Action> routes;

    //regex routes
    std::vector<Regex*> regs;
    std::vector<Action> actions;

    unsigned int options;

    bool flushed;
};

#endif
