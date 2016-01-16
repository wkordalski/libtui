#ifndef _LIBTUI_WIDGET_HH_
#define _LIBTUI_WIDGET_HH_

#include "positioning.hh"

#include <ncurses.h>

namespace tui {
  class Window;

  class Widget {
  protected:
    Widget * parent = nullptr;
  public:
    Widget() = delete;
    Widget(const Widget &) = delete;
    Widget(Widget *parent) : parent(parent) {}
    virtual ~Widget() {}

    Widget & operator= (const Widget &) = delete;

    virtual void refresh() {};

    const Widget * get_parent() const { return parent; }
    Widget * get_parent() { return parent; }
    virtual const Window * get_window() const {
      if(parent != nullptr) return parent->get_window(); else return nullptr;
    }
    virtual Window * get_window() {
      if(parent != nullptr) return parent->get_window(); else return nullptr;
    }

  protected:
    virtual void parent_resize(Size parent_size) = 0;
    virtual void draw() = 0;
    virtual void focus() {}
    virtual void blur() {}
    virtual void key(int ch) { /* ignore */ }
    virtual bool is_container() { return false; }
    virtual bool is_focusable() { return false; }
    virtual void set_cursor() {}

    friend class Window;
    friend class Application;
  };
}

#endif /* end of include guard: _LIBTUI_WIDGET_HH_ */
