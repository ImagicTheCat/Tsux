/* 
 * Action, used by Tsux routing
 * execute a function with passed data
 * or call a ModAction of a module
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#include "Action.hpp"
#include "Tsux.hpp"
#include "Module.hpp"

Action::Action(TsuxAction _action, void* _data):
  type(MAIN),
  action(_action), data(_data){
}


Action::Action(ModAction _maction, Module* _module):
  type(MODULE),
  maction(_maction), module(_module){
}

void Action::execute(Tsux& tsux){
  if(type == MAIN)
    action(tsux, data);
  else if(type == MODULE)
    (module->*maction)();
}
