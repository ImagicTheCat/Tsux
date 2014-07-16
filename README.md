Tsux
====
*C++ library for FCGI C/C++ web application*

## Introduction
Tsux is a library designed to make C++ web application (it's means the application stay alive between two requests) in a simple way.

Tsux is based on FastCGI (especially libfcgi and libfcgi++) and try to be simple as possible.

**Features:**
* usage of C++ stream to write HTTP response
* easy access to usefull data
  * regular post/get data
  * post file data (multipart/form-data enctype parsing)
  * webserver params
  * cookies
  * response header
* simple routing system with POSIX extended regex
* simple module system
* simple template system
* mimetypes matching
* regex, dirent encapsulations and easy file loading

Tsux don't have the goal to implement current web standart, but to let you the capacity to build simple website in C++.

## Tutorials
### Simple main
```cpp
#include <tsux/Tsux.hpp>

int main(int argc, char** argv){
  Tsux tsux;

  while(tsux.accept()){
    tsux.header.set("Content-Type", "text/plain");

    tsux.response << "Hello world !";

    tsux.end();
  }

  return 0;
}
```
