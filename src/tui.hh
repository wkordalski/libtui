#ifndef _LIBTUI_TUI_HH_
#define _LIBTUI_TUI_HH_

#include "window.hh"

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <vector>

#include <sys/poll.h>

namespace tui {
  class IWindow;

  class Application {
  public:
    Application();
    Application(const Application &) = delete;
    ~Application();

    void run();           // event loop
    // must be called from run() thread
    void exit();          // stops running

    // TODO: adding polling of other fds
    void call(std::function<void()> action);

    // used by signal handlers to notify about terminal resize
    void terminal_resize();


    void refresh();                   // redraws the screen
    Size get_screen_size();  // x and y
  protected:

    void on_terminal_resize();
    void on_keyboard(int ch);

    void keyboard_worker();
  protected:
    Widget * window;
    bool running;
    bool has_colors;
    Size screen_size;

    // calling...
    std::queue<std::function<void()>> call_queue;
    std::mutex call_queue_mutex;
    std::condition_variable call_processor_cv;


  public:
    void set_widget(Widget *w) { this->window = w; }
  };
}

#endif /* end of include guard: _LIBTUI_TUI_HH_ */
