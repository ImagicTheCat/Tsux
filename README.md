![alt text](https://raw.githubusercontent.com/ImagicTheCat/Tsux/master/resources/logo/logo300.png "Tsux")

*C++ library for FCGI C/C++ web application*
 
#### Table of content
* [Introduction](#introduction-)
* [Installation](#installation-)
  * [Requirements](#requirements-)
  * [Compilation](#compilation-)
* [Configuration](#configuration-)
  * [Nginx](#nginx-)
* [Launch](#launch-)
* [Tutorials](#tutorials-)
  * [Simple main](#simple-main-)
  * [Routing](#routing-)
  * [Variables](#variables-)
  * [Module](#module-)
  * [Sessions](#sessions-)
  * [Template](#template-)
  * [Config](#config-)
  * [Translator](#translator-)
  * [Generations](#generations-)
  * [Miscellaneous](#miscellaneous-)
* [About](#about-)


## Introduction [&uarr;](#table-of-content)
Tsux is a library designed to made C++ web application (it's means the application stay alive between two requests) in a simple way.

Tsux is based on [FastCGI](http://www.fastcgi.com) (especially libfcgi and libfcgi++) and try to be simple as possible.

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
* base encoder/decoder
* a translator system
* a session system
* customization of error pages internally

Tsux don't have the goal to implement current web technologies, but to let you the capacity to build simple website in C++.

**Advantages:**
* Simple
* The FastCGI advantages

**Inconvenients:**
* Don't handle huge files or stream

## Installation [&uarr;](#table-of-content)
### Requirements [&uarr;](#table-of-content)
You need the FastCGI library. Under a linux distribution like debian you will find the `libfcgi-dev` package. And `fcgi` under Archlinux.

### Compilation [&uarr;](#table-of-content)
For the moment, Tsux don't have version or package.

Compile :
```bash
$ git clone https://github.com/ImagicTheCat/Tsux
$ cd Tsux
$ make
```

Install to /usr/local/lib :
```bash
$ sudo make install
```

## Configuration [&uarr;](#table-of-content)
The configuration is the same of all FastCGI applications, but Tsux must have some Webserver paramaters to work correctly :
* REQUEST_URI
* HTTP_COOKIE
* REMOTE_ADDR
* CONTENT_TYPE
* CONTENT_LENGTH
* HTTP_ACCEPT_LANGUAGE

### Nginx [&uarr;](#table-of-content)
An example of a simple configuration with nginx
```
server{
  listen  80;
  server_name localhost;
  
  location /{
    fastcgi_pass 127.0.0.1:8000;
    include      fastcgi_params;
  }
}
```

All the requests will be send to the application. Read the nginx manual for an advanced configuration (for example, if you want to deserve static files outside the app).

## Launch [&uarr;](#table-of-content)
You will probably need `spawn-fcgi` to launch your application :
```bash
spawn-fcgi -n myapp -p 8000
```

## Tutorials [&uarr;](#table-of-content)
### Simple main [&uarr;](#table-of-content)
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

### Routing [&uarr;](#table-of-content)

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

Routes are evaluated in the bind call order. If a two routes have matched regex, the first will be executed.

#### Route rewriting
You can rewrite route for internal redirection (ex: administration, maintenance or language specific pages).
```cpp
//in tsux action function
tsux.rewrite("/newroute/foo/bar");

//or in a module action function (shortcut)
rewrite("/newroute/foo/bar");
```
The new route will be executed. Strange behaviour can happend if you continue to use Tsux in the current action function after a rewrite call, so, follow the rewrite function by a `return;` instruction if you don't know exatly what you are doing.

### Variables [&uarr;](#table-of-content)

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
tsux.uri(); //complete uri(get couples included)

//location
tsux.location(); //the location in the client url

//path
tsux.path(); //the matched route, different of location after a route rewriting

//locale
//tsux look at the accepted languages in the request and find the first small country denomination
tsux.locale();

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

### Load and send file to client [&uarr;](#table-of-content)
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

### Module [&uarr;](#table-of-content)

Modules let you manage your routes and data. They also have tsux shortcuts.

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

Or just use a parameter into the module constructor
```cpp
  MyModule mymodule(tsux);
  MyOtherModule myothermodule(tsux, mymodule);
```

Do whatever you want.

### Sessions [&uarr;](#table-of-content)
Tsux sessions are like PHP sessions, a cookie is created to identify the users. Tsux don't store data, just an unique string id (ssid), but you can bind functions for two events :
* when a session is created
* when a session is deleted

First enable sessions with `tsux.enable(Tsux::SESSION)`.

Bind global function
```cpp
void mycreate(const std::string& ssid){
}

void mydelete(const std::string& ssid){
}

tsux.bindSessionCreate(mycreate);
tsux.bindSessionDelete(mydelete);
```

Better way with module :
```cpp
//listen sessions events
MyModule::MyModule(Tsux& tsux): 
  Module(tsux, "mymodule", SESSION){ //add SESSION to module options
}

//implement onSessionCreate and onSessionDelete functions
void MyModule::onSessionCreate(const std::string& ssid){
  //ex: add id into a std::map with a data structure
}

void MyModule::onSessionDelete(const std::string& ssid){
  //ex: delete id from the std::map
}

```

Get the ssid for a request
```cpp
//global
tsux.ssid();

//in module
ssid();
```

Set the session time with `tsux.setSessionTime(600)` (in seconds, 10 minutes in this case)


### Template [&uarr;](#table-of-content)

The template system is actually very simple. 

#### Template usage
```cpp
#include <tsux/Template.hpp> 

Template tpl;

//load from memory
tpl.data = "<html>{{content}}</html>";

//or load from file
tpl.loadFromFile("mytemplate.html.tpl");

//compile the template
tpl.compile();

//set data string
tpl.set("content", "mycontent");

//set data string pointer (linked data)
std::string mycontent = "mycontent";
tpl.set("content", &mycontent);

//set a tsux function
//prototype: void myfunc(Tsux& tsux, void* data);
tpl.set("content", myfunc, mydata);

//set a module function
tpl.set("content", &MyMod::myfunc, &mymod);

//set another template
tpl.set("header", header_tpl);

//you can set template value with any type if it can be converted into a string
tpl.set("page", 2);
tpl.set("time", 1.5585);

//render the template
tpl.render(tsux);
```

#### Template herit
```cpp
  //after compilation
  tpl.herit(another_tpl);
```

The template will herit of the possibles translations given by the parent's translator and all the identifiers. Multiple heritances possible.

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

### Config [&uarr;](#table-of-content)
Tsux have a simple parser for config files, only string based.
Each line can be a new group, or a new value. A new group is just an indented identifier, and a value an indented identifier plus a "=" followed by a value (any characters until the end of line is reached).
Identations can be done with tabulations or spaces, but keep one behaviour for prevent strange problems.

#### Config notation
```
mysql
  host=localhost
  user=root
  password=
  db=database

global
  admin
    password=mypassword
  moderator
    password=mypassword
```

#### Config usage
```cpp
Config conf;

//loading file or data don't erase previously loaded elements
conf.load("from memory");
conf.loadFromFile("config/global.conf");


//test full path
if(conf.has("mysql.host"))
  std::string host = conf.get("mysql.host");

//get any value type converted in string with default value
int pages = conf.get("pages", 10);

//set a value by full path
conf.set("mysql.host", "mydomain.com");
conf.set("mysql.threads", 5);
```


### Translator [&uarr;](#table-of-content)
#### Translator notation
The translator work with the config file notation.
The groups at the root level are the locales, except `all` which define values for all languages.

```
all 
  domain=http://mydomain.com

en
  index
    content
      msg=Welcome !
      msg2=It's bigger on the inside
    content2
      msg=ect...
  menu
    first=First
fr
  index
    content
      msg=Bienvenue !
      msg2=C'est plus grand à l'intérieur
    content2
      msg=ect...
  menu
    first=Premier
```

#### Translator usage

```cpp
Translator tr;

//loading file or data don't erase previously loaded translations
tr.load("from memory");
tr.loadFromFile("translations/fr");
tr.loadFromFile("translations/en");

//set the locale
tr.setLocale("en");

//test full path
if(tr.has("en.index.content.msg"))
  std::string translation = tr.get("en.index.content.msg");

//test relative path (with the locale, also check all group)
if(tr.hasTrans("index.content.msg"))
  std::string translation = tr.trans("index.content.msg");

//set a value by full path
tr.set("all.domain", "http://mydomain.com");
```

#### Use translator with templates
Just compile your template with the translator, when all the translations are loaded. Adding new translations after the compilation will not affect the template.
```cpp
tpl.compile(tr);
```

In the template
```html
<body>
  {{index.content.msg}}
  <br />{{index.content.msg2}}
</body>
```

### Generations [&uarr;](#table-of-content)

#### Custom error pages

You can customize any error page like routes. For now, 404 error is the only one implemented by default.

```cpp
//in a module
bindCode(404, &MyModule::my404);

//or with a global tsux function
tsux.bindCode(404, my404, mydata);
```

404 example

```cpp
void MyModule::my404(){
  header.set("Content-Type", "text/html");
  header.set("Status", "404 Not Found");

  response << "My 404 custom page.";
}
```

Generate

```cpp
void MyModule::myroute(){
  if(post.get("password", "") != "mypassword")
    generate(404); //or 403
  else 
    response << "hi admin !";
}
```

#### ParamSet and FileSet
To debug your code, you can print `ParamSet` and `FileSet` structures to html.

Example
```cpp
void MyModule::debug(){
  response << "POST\n";
  generate(post);
  response << "GET\n";
  generate(get);
  response << "HEADER\n";
  generate(header);
  response << "PARAM\n";
  generate(param);
  response << "FILE\n";
  generate(file);
  response << "ROUTE\n";
  generate(route);
}
```

### Miscellaneous [&uarr;](#table-of-content)

#### Paginator

##### Usage

```cpp
Paginator p;

//maximum visible pages in pagination
p.setVisiblePages(10);

//maximum elements per page
p.setElementsPerPage(20);

//change elements size
p.size(1000);

//begin at fist page
p.page(1);

//get pages size number
p.pages();

//get a range of current page elements
p.a(); //first index (begin at 0)
p.b(); //last index

//get elements size for the current page
p.n();

//get the main template for rendering pagination
mytpl.set("pagination", p.tpl());

//render directly
p.render(tsux);
```

##### Customization

You can customize the rendered pagination with template-like notation.
This is the default render.

```cpp
//pagination container
p.htmlMain("<ul class=\"paginator\">{{elements}}</ul>");

//first button <<
p.htmlFirst("<li><a href=\"?p={{page}}\">&laquo;</a></li>");

//prev button <
p.htmlPrev("<li><a href=\"?p={{page}}\">&lsaquo;</a></li>");

//page button 1 2 3 ...
p.htmlEl("<li class=\"{{class}}\"><a href=\"?p={{page}}\">{{page}}</a></li>");

//next button >
p.htmlNext("<li><a href=\"?p={{page}}\">&rsaquo;</a></li>");

//last button >>
p.htmlLast("<li><a href=\"?p={{page}}\">&raquo;</a></li>");
```

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

std::string title = "My article title";
std::string slug = URI::slugify(title);
```

#### HTML tools
```cpp
std::string html = "<b>bold</b>";
std::string safe_text = HTML::entities(html);
```

#### Dir tools
```cpp
#include <tsux/Dir.hpp> 

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
#include <tsux/MIMEType.hpp> 

std::string mimetype;

//get a mime type by extension, filename or path
mimetype = MIMEType::get("css");
mimetype = MIMEType::get("filename.css");
mimetype = MIMEType::get("foo/bar/filename.css");

//set a mimetype (owerwrite in this case)
MIMEType::set("css", "text/plain");
```

#### Base encoder/decoder
Tsux have a class to encode and decode data into a string with a custom alphabet.

**Warning** : Use it carefully if you want to send the encoded data accross the network and decode it on another computer (there are differences in data encoding, for example `int` can be 2 or 4 bytes).

```cpp 
#include <tsux/Base.hpp> 

//a base62 alphabet
std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

int var = 42;
int decoded = 0;

//encoding
std::string encoded = Base::encode(&var, sizeof(int), alphabet);
//decoding
Base::decode(&var, sizeof(int), encoded, alphabet);
//decoded int equal 42

//same with string
//encoding
std::string data = "mydata";
encoded = Base::encode(data.c_str(), data.size()+1, alphabet);
//decoding
char sdecoded[256];
Base::decode(sdecoded, 256, alphabet);
```

## About [&uarr;](#table-of-content)
Tsux is written by ImagicTheCat, a very lazy developer who can't understand complexity. The library is under the MIT licence and based on FastCGI. 

I hope you will enjoy it and made awesome C++ websites !
