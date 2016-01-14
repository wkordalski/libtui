#ifndef _LIBTUI_TUI_HH_
#define _LIBTUI_TUI_HH_

#include "window.hh"

#include <functional>
#include <vector>

#include <sys/poll.h>

namespace tui {
  class Window;

  class Application {
  public:
    Application();
    ~Application();

    void run();           // event loop
    void exit();          // stops running

    // TODO: adding polling of other fds

  protected:
    void refresh();                   // redraws the screen
    void on_keyboard(short events);   // done on keyboard information
  protected:
    Window * window;
    bool running;
    bool has_colors;
    std::vector<struct pollfd> fds;
    std::vector<std::function<void(short)>> fds_actions;  // gets event flags
  };
}

#endif /* end of include guard: _LIBTUI_TUI_HH_ */
