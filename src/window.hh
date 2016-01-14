#ifndef _LIBTUI_WINDOW_HH_
#define _LIBTUI_WINDOW_HH_

#include <ncurses.h>

namespace tui {
  class Window {
  public:
    ~Window();

    void refresh();   // redraws the window
  protected:
    virtual void parent_resize(int w, int h);
  protected:
    WINDOW *cwin; // curses window

    friend class Application;
  };
}

#endif /* end of include guard: _LIBTUI_WINDOW_HH_ */
