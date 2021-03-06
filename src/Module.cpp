/* 
 * Module
 * 
 * let you manage your code into modules
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */

#include "Module.hpp"

Module::Module(Tsux& _tsux, const std::string& name, unsigned int opts):
    tsux(_tsux), _name(name), options(opts),
    //build shortcuts
    out(_tsux.out), in(_tsux.in), err(_tsux.err), response(_tsux.response),
    post(_tsux.post), get(_tsux.get), header(_tsux.header), route(_tsux.route),
    param(_tsux.param), cookie(_tsux.cookie), file(_tsux.file){
      _tsux.registerModule(name, this);
      if(opts & SESSION)
        _tsux.registerSessionListener(this);
}

Module::~Module(){
  tsux.unregisterModule(_name);
}
