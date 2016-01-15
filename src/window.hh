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
  protected:
    virtual void parent_resize(Size parent_size);

  protected:
    WINDOW *cwin = nullptr; // curses window
    Point position;
    Size size;
    Locator window_locator;
    std::vector<Widget *> widgets;
  };
}

#endif /* end of include guard: _LIBTUI_WINDOW_HH_ */
