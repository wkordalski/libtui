#ifndef _LIBTUI_WINDOW_HH_
#define _LIBTUI_WINDOW_HH_

#include "positioning.hh"
#include "widget.hh"

#include <functional>
#include <vector>

#include <ncurses.h>

namespace tui {
  class Window : public Widget {
  public:
    Window(Widget *parent, Point position, Size size);
    virtual ~Window();

    virtual void refresh();
    virtual void resize(Point position, Size size);

    void add(Widget *widget);

    WINDOW * get_curses_window();

    virtual const Window * get_window() const;
    virtual Window * get_window();

    virtual void draw();

    void set_locator(const Locator &locator) {
      this->window_locator = locator;
    }
  protected:
    virtual void parent_resize(Size parent_size);
    virtual void focus() {}
    virtual void blur() {}
    virtual void key(int ch) {
      if(ch == '\t') {
        if(widgets.size() > 0 && widgets[focused]->is_container()) {
          widgets[focused]->key(ch);
        } else if(widgets.size() > 1) {
          widgets[focused]->blur();
          focused++;
          focused %= widgets.size();
          widgets[focused]->focus();
        }
      } else {
        if(widgets.size() > 0) {
          widgets[focused]->key(ch);
        }
      }
    }
    virtual bool is_container() { return false; }

    virtual void set_cursor() {
      if(widgets.size() > 0) {
        widgets[focused]->set_cursor();
      }
    }

  protected:
    WINDOW *cwin = nullptr; // curses window
    Point position;
    Size size;
    Locator window_locator;
    std::vector<Widget *> widgets;
    int focused = 0;
  };
}

#endif /* end of include guard: _LIBTUI_WINDOW_HH_ */
