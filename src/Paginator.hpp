/* 
 * Paginator
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */

#ifndef H_PAGINATOR
#define H_PAGINATOR

#include <cmath>
#include "Template.hpp"

class Paginator{
  public:
    Paginator();

    //render pagination
    void render(Tsux& tsux);

    //change elements size
    void size(int s){ _size = s; _pages = ceil(_size/(float)per_page); }
    int size() const { return _size; }

    //change visible pages
    void setVisiblePages(int n){ visible_pages = n; }
    void setElementsPerPage(int n){ per_page = n; }

    //count pages
    int pages() const { return _pages; }

    //set current page
    void page(int p){ _page = p; }
    int page() const { return _page; }

    //get elements range (a -> b)
    int a() const;
    int b() const;

    //get size of the range a -> b
    int n() const { return b()-a()+1; }

    //views
    static void v_elements(Tsux& tsux, void* data);
    Template& tpl() { return t_main; }

  private:
    Template t_main, t_prev, t_next, t_el, t_first, t_last;
    int per_page, visible_pages;
    int _size, _pages, _page;

};

#endif
