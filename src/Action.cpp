
#include "Action.hpp"
#include "Tsux.hpp"

Action::Action(TsuxAction _action, void* _data):
  action(_action), data(_data){
}

void Action::execute(Tsux& tsux){
  action(tsux, data);
}
