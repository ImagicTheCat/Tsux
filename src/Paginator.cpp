/* 
 * Paginator
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */

#include "Paginator.hpp"

Paginator::Paginator():
  per_page(10),
  visible_pages(3),
  _size(0), _pages(0), _page(1)
{
  //initialize templates
  htmlMain("<ul class=\"paginator\">{{elements}}</ul>");
  htmlNext("<li><a href=\"?p={{page}}\">&rsaquo;</a></li>");
  htmlPrev("<li><a href=\"?p={{page}}\">&lsaquo;</a></li>");
  htmlLast("<li><a href=\"?p={{page}}\">&raquo;</a></li>");
  htmlFirst("<li><a href=\"?p={{page}}\">&laquo;</a></li>");
  htmlEl("<li class=\"{{class}}\"><a href=\"?p={{page}}\">{{page}}</a></li>");
}

int Paginator::a() const {
  return (_page-1)*per_page;
}

int Paginator::b() const {
  int val = _page*per_page-1;
  if(val >= size())
    val = size()-1;

  return val;
}

void Paginator::render(Tsux& tsux){
  t_main.render(tsux);
}


void Paginator::v_elements(Tsux& tsux, void* data){
  Paginator def_paginator;
  Paginator& paginator = Tsux::ref(data, def_paginator);

  /* center page number to the current page and add prev/next */

  //middle delta
  int m1 = paginator.visible_pages/2;
  int m2 = paginator.visible_pages-m1;

  //range
  int a = paginator.page()-m1;
  int b = paginator.page()+m2-1;

  //minimize/maximize range
  if(a < 1 && b > paginator.pages()){
    a = 1;
    b = paginator.pages();
  }
  else if(a < 1){
    b += 1-a;
    a = 1;
    if(b > paginator.pages())
      b = paginator.pages();
  }
  else if(b > paginator.pages()){
    a -= b-paginator.pages();
    b = paginator.pages();
    if(a < 1)
      a = 1;
  }

  //first
  if(a > 1){
    paginator.t_first.set("page", "1");
    paginator.t_first.render(tsux);

  }

  if(paginator.page() > 1){
    std::stringstream ss;
    ss << paginator.page()-1;
    paginator.t_prev.set("page", ss.str());
    paginator.t_prev.render(tsux);
  }


  for(int i = a; i <= b; i++){
    if(i == paginator.page())
      paginator.t_el.set("class", "current");
    else
      paginator.t_el.set("class", "");

    std::stringstream ss;
    ss << i;
    paginator.t_el.set("page", ss.str());

    paginator.t_el.render(tsux);
  }

  //next
  if(paginator.page() < paginator.pages()){
    std::stringstream ss;
    ss << paginator.page()+1;
    paginator.t_next.set("page", ss.str());
    paginator.t_next.render(tsux);
  }

  //last
  if(b < paginator.pages()){
    std::stringstream ss;
    ss << paginator.pages();
    paginator.t_last.set("page", ss.str());
    paginator.t_last.render(tsux);
  }
}

/* CUSTOMIZATION */

void Paginator::htmlFirst(const std::string& str){
  t_first.data = str;
  t_first.compile();
}


void Paginator::htmlPrev(const std::string& str){
  t_prev.data = str;
  t_prev.compile();
}


void Paginator::htmlNext(const std::string& str){
  t_next.data = str;
  t_next.compile();
}


void Paginator::htmlLast(const std::string& str){
  t_last.data = str;
  t_last.compile();
}


void Paginator::htmlEl(const std::string& str){
  t_el.data = str;
  t_el.compile();
}


void Paginator::htmlMain(const std::string& str){
  t_main.data = str;
  t_main.compile();
  t_main.set("elements", Paginator::v_elements, (void*)this);
}
