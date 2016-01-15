#ifndef _LIBTUI_WINDOW_HH_
#define _LIBTUI_WINDOW_HH_

#include "positioning.hh"
#include "widget.hh"

#include <functional>
#include <vector>

#include <ncurses.h>

namespace tui {
  class Window : public Widget {
  protected:
    WINDOW *cwin = nullptr; // curses window
    Point position;
    Size size;
    Locator window_locator;
    std::vector<Widget *> widgets;
  public:
    Window(Widget *parent, Point position, Size size) : Widget(parent), position(position), size(size) {
      cwin = newwin(size.h, size.w, position.y, position.x);
      window_locator = [this](Size parent_size) -> std::pair<Point, Size>
      {
        return std::make_pair(this->position, this->size);
      };
    }
    virtual ~Window() {
      delwin(cwin);
    }

    virtual void refresh() {
      for(auto w : widgets) {
        w->draw();
      }
      ::wrefresh(cwin);
    }
    virtual void resize(Point position, Size size) {
      delwin(cwin);
      this->position = position;
      this->size = size;
      cwin = newwin(size.h, size.w, position.y, position.x);
    }

    void add(Widget *widget) {
      for(auto w : widgets) {
        if(widget == w) throw std::logic_error("Can not add the same widget two times.");
      }
      widgets.push_back(widget);
    }

    WINDOW * get_curses_window() { return cwin; }

    virtual const Window * get_window() const { return this; }
    virtual Window * get_window() { return this; }

    virtual void draw() {}
  protected:
    virtual void parent_resize(Size parent_size) {
      Point p;
      Size s;
      std::tie(p, s) = this->window_locator(parent_size);
      this->resize(p, s);
    }
  };
}

#endif /* end of include guard: _LIBTUI_WINDOW_HH_ */
