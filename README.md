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
  std::string data;
  tsux.bind("/my/route", myroute, &data);
```

and unreference it in the callback
```cpp
  std::string default_str;
  //return alias to default_str if data is NULL
  std::string& str = Tsux::ref(data, default_str); 

  //each request will change data
  str += "NEW REQUEST\n";
  tsux.response << "\n" << str;

```


#### Regex routing

You can use regex routing to catch parameters in the url
```cpp
 //enable REGEX_ROUTING option
  tsux.enable(Tsux::REGEX_ROUTING);

  //bind route to myroute function with regex
  tsux.bind("^/my/route-(.*)$", myroute);
```

And in the route callback
```cpp
  //get the first parameter of the route
  //you need to specify a default value (can be string, int, float, double, etc)
  tsux.reponse << tsux.route.get("1", "");
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

**Warning** : FCGI and Tsux aren't good to send huge files cause of memory issue (all the file is cached before sending the request). For huge files, use the webserver to send them.

```cpp
//we make our callback for the file route
void get_file(Tsux& tsux, void* data){
  FileSet def;
  FileSet& files = Tsux::ref(data, def);
  std::string path = tsux.route.get("1","");

  if(files.has(path)){
    File def;
    File& file = files.get(path, def);

    //set header and send data
    tsux.header.set("Content-Type", file.type);
    tsux.response << file.data;
  }
  else //generate a default 404 page if the file is not found
    tsux.generate(404);
}

int main(int argc, char** argv){
  Tsux tsux;

  FileSet files;

  /* Load files of the dir "files/" in memory */
  std::vector<std::string> list;

  //explode dir
  Dir::explode("files/", list, Dir::SFILE | Dir::RECURSIVE);

  for(int i = 0; i < list.size(); i++){
    //allocate new file with his name withtout the path
    File& f = files.alloc(list[i].substr(5));

    //load the file, the mimetype will be find by the extension
    f.loadFromFile(list[i]);
  }

  tsux.enable(Tsux::REGEX_ROUTING);

  //bind file route and give the FileSet
  tsux.bind("^/file/(.+)$", get_file, &files);

  while(tsux.accept()){
    tsux.dispatch();
    tsux.end();
  }

  return 0;
}
```

### Module

Modules let you manage your code and memory. They also have tsux shortcuts.

#### Module definition

MyModule.hpp
```cpp
#include <tsux/Module.hpp>

class MyModule : public Module{
  public:
    MyModule(Tsux& tsux);
    void myroute();

  private:
    std::string content;
};
```

MyModule.cpp
```cpp
#include "MyModule.hpp"

MyModule::MyModule(Tsux& tsux):Module(tsux,"mymodule"){
  bind("^/my/route$", &MyModule::myroute);
  content = "Hello world !";
}

void MyModule::myroute(){
  header.set("Content-Type", "text/plain");
  response << content;
}
```

#### Instanciation

Just instanciate the module with tsux.
```cpp
  MyModule mymodule(tsux);

  while(tsux.accept()){
    tsux.dispatch();
    tsux.end();
  }
```

#### Get a loaded module

Use tsux
```cpp
MyModule* mymodule = (MyModule*)tsux.module("mymodule");
```

Or just use a parameter to your module constructor
```cpp
  MyModule mymodule(tsux);
  MyOtherModule myothermodule(tsux, mymodule);
```

Do whatever you want.

### Template

The template system is actually very simple. 

#### Template class
```cpp
Template tpl;

//load from memory
tpl.data = "<html>{{content}}</html>";

//or load from file
tpl.loadFromFile("mytemplate.html.tpl");

//compile the template
tpl.compile();

//set data string
tpl.set("content", "mycontent");

//or set data string pointer (linked data)
std::string mycontent = "mycontent";
tpl.set("content", &mycontent);

//render the template
tsux.response << tpl.render();
```

#### Template notation

```html
<!DOCTYPE html>
<html>
  <head>
    <title>{{title}}</title>
  </head>

  <body>
    {{content}}
  </body>
</html>
```

All the characters between {{ and }} will be a part of the identifier (also space)

### Miscellaneous

#### Cookies

Create a cookie
```cpp
//life time in seconds
int time = 60;

tsux.createCookie("name", "data", time);
```

Delete a cookie
```cpp
//just set the life time to 0
tsux.createCookie("name", "", 0);
```

#### URI tools
```cpp
std::string uri = "post data or uri";
std::string encoded = URI::encode(uri);
std::string decoded = URI::decode(encoded);
```

#### Dir tools

```cpp
//list recursively files of path/ directory
std::vector<std::string> list;
int options = Dir::SFILE | Dir::RECURSIVE;
Dir::explode("path/", list, options);

//list example content
// "path/dir/file.png"
// "path/file.jpg"
```

Options
* SFILE: list files
* SDIR: list directories
* SPARENT: list parent directories (. and ..)
* RECURSIVE: do it recursively

**Warning** : do not use SPARENT and RECURSIVE, cause infinite loop

#### MIME Types
```cpp
std::string mimetype;

//get a mime type by extension, filename or path
mimetype = MIMEType::get("css");
mimetype = MIMEType::get("filename.css");
mimetype = MIMEType::get("foo/bar/filename.css");

//force a mimetype
MIMEType::set("css", "text/plain");
```

## About
Tsux is written by ImagicTheCat, a very lazy developer who can't understand complexity. The library is under the MIT licence and based on FastCGI. 

I hope you will enjoy it and made awesome C++ website !
