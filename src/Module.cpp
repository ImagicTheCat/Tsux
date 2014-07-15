/* 
 * Module
 * 
 * let you manage your code into modules
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */

#include "Module.hpp"

Module::Module(Tsux& _tsux, const std::string& name):
    tsux(_tsux), _name(name),
    //build shortcuts
    out(tsux.out), in(tsux.in), err(tsux.err), response(tsux.response),
    post(tsux.post), get(tsux.get), header(tsux.header), route(tsux.route),
    param(tsux.param), cookie(tsux.cookie), file(tsux.file){
}


void Module::bind(const std::string& path, ModAction action){
  tsux.bind(path, Action(action, this));
}
