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

Tsux don't have the goal to implement current web technologies, but to let you the capacity to build simple website in C++.

## Tutorials
### Simple main
```cpp
#include <tsux/Tsux.hpp>

int main(int argc, char** argv){
  //tsux instanciation
  Tsux tsux;

  //waiting request loop
  while(tsux.accept()){
    //a request
    //set content-type to text/plain
    tsux.header.set("Content-Type", "text/plain");

    //send content
    tsux.response << "Hello world !";

    //flush response
    tsux.end();
  }

  return 0;
}
```

### Routing

#### Simple routing
```cpp
#include <tsux/Tsux.hpp>

//callback function of /my/route
void myroute(Tsux& tsux, void *data){
  tsux.header.set("Content-Type", "text/plain");
  tsux.response << "You are on /my/route";
}

int main(int argc, char** argv){
  Tsux tsux;

  //bind route to myroute function
  tsux.bind("/my/route", myroute);

  while(tsux.accept()){
    //dispatch request to the routes
    tsux.dispatch();

    tsux.end();
  }

  return 0;
}
```


You can give data to the callback
```cpp
#include <tsux/Tsux.hpp>

//callback function of /my/route
void myroute(Tsux& tsux, void *data){
  //unreference data with a default object
  //or other method
  std::string default_str;
  std::string& str = Tsux::ref(data, default_str); //return an alias to default_str 
                                                   //if data is NULL
                                                   //return alias to the data if not

  tsux.header.set("Content-Type", "text/plain");
  tsux.response << "You are on /my/route";

  //each request will change data
  str += "NEW REQUEST\n";
  tsux.response << "\n" << str;
}

int main(int argc, char** argv){
  Tsux tsux;

  std::string data;

  //bind route to myroute function
  tsux.bind("/my/route", myroute, &data);

  while(tsux.accept()){
    //dispatch request to the routes
    tsux.dispatch();

    tsux.end();
  }

  return 0;
}
```


#### Regex routing
```cpp
#include <tsux/Tsux.hpp>

//callback function of /my/route
void myroute(Tsux& tsux, void *data){
  tsux.header.set("Content-Type", "text/plain");
  tsux.response << "You are on /my/route, arg : ";

  //get the first parameter of the route
  //you need to specify a default value (can be string, int, float, double, etc)
  tsux.reponse << tsux.route.get("1", "");
}

int main(int argc, char** argv){
  Tsux tsux;

  //enable REGEX_ROUTING option
  tsux.enable(Tsux::REGEX_ROUTING);

  //bind route to myroute function
  tsux.bind("^/my/route-(.*)$", myroute);

  while(tsux.accept()){
    //dispatch request to the routes
    tsux.dispatch();

    tsux.end();
  }

  return 0;
}
```

### Variables

#### Variable sets and others
```cpp
//route
std::string article = tsux.route.get("1","");

//get
int page = tsux.get.get("page", 0);

//post
if(tsux.post.has("pseudo")){
  //my form action
}

//cookies (read only, if you want to create a cookie, look at tsux.createCookie())
std::string cookie = tsux.cookie.get("mycookie","");

//webserver params
std::string ip = tsux.param.get("REMOTE_ADDR","");

//header
tsux.header.set("Content-Type", "image/png");

//uri
tsux.uri(); //also contain get vars

//location
tsux.location(); //the matched route only

//set a var for this request
tsux.post.set("number", 5);
```


#### File set
When a file is uploaded : 
```cpp
//default file
File def;

//if multiple, return the first
File& file = tsux.file.get("myfilefield", def);

/* ALL FILES */
//default files vector
std::vector<File> defs;

std::vector<File>& files = tsux.file.get("myfilefield", defs);

//file structure (complete as possible)
tsux.header.set("Content-Type", file.type); //the mimetype
tsux.response << file.data; //data
std::cout << file.name << " sended." << std::endl; //the filename
```

### Load and send file to client
*a little file manager*

**Warning : **FCGI and Tsux aren't good to send huge files cause of memory issue (all the file is cached before sending the request

```cpp
void get_file(Tsux& tsux, void* data){
  FileSet def;
  FileSet& files = Tsux::ref(data, def);

  if(files.has(tsux.route.get("1", ""))){
    File def;
    File& file = files.get(tsux.route.get("1",""), def);
    tsux.header.set("Content-Type", file.type);
    tsux.response << file.data;
  }
  else
    tsux.generate(404);
}

int main(int argc, char** argv){
  Tsux tsux;

  FileSet files;

  std::vector<std::string> list;
  Dir::explode("files/", list, Dir::SFILE | Dir::RECURSIVE);
  for(int i = 0; i < list.size(); i++){
    File& f = files.alloc(list[i].substr(5));
    f.loadFromFile(list[i]);
  }

  tsux.enable(Tsux::REGEX_ROUTING);

  tsux.bind("^/file/(.+)$", get_file, &files);

  while(tsux.accept()){
    tsux.dispatch();
    tsux.end();
  }

  return 0;
}
```
