/* 
 * Action, used by Tsux routing
 * execute a function with passed data
 * or call a ModAction of a module
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#ifndef H_ACTION
#define H_ACTION

class Tsux;
class Module;

typedef void (Module::*ModAction)(void);
typedef void (*TsuxAction)(Tsux& tsux, void* data);

struct Action{
  enum{
    MAIN,
    MODULE
  };

  Action(TsuxAction action, void* _data);
  Action(ModAction maction, Module* module);
  Action(){}

  //Tsux
  TsuxAction action;
  void* data;

  //Module
  ModAction maction;
  Module* module;

  void execute(Tsux& tsux);

  int type;
};

#endif
