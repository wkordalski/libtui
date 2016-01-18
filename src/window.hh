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
    Window(Application *app);
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
    void set_title(const std::string &title) {
      this->title = title;
    }
    Window * get_child_window() {
      return this->child;
    }
    void set_child_window(Window *window);
  protected:
    virtual void parent_resize(Size parent_size);
    virtual void focus() {}
    virtual void blur() {}
    virtual void key(int ch);
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
    Window *child = nullptr;
    int focused = 0;
    std::string title = "";
  };
}

#endif /* end of include guard: _LIBTUI_WINDOW_HH_ */
