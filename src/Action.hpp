/* 
 * Action, used by Tsux routing
 * execute a function with passed data
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#ifndef H_ACTION
#define H_ACTION

class Tsux;

typedef void (*TsuxAction)(Tsux& tsux, void* data);

struct Action{
  Action(TsuxAction _action, void* _data);

  TsuxAction action;
  void* data;

  void execute(Tsux& tsux);
};

#endif
