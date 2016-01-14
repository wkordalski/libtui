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
  class Window;

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

  protected:
    void refresh();                   // redraws the screen
    void on_terminal_resize();
    void on_keyboard(int ch);
    std::pair<int, int> get_screen_size();  // x and y
    void keyboard_worker();
  protected:
    Window * window;
    bool running;
    bool has_colors;
    int width, height;

    // calling...
    std::queue<std::function<void()>> call_queue;
    std::mutex call_queue_mutex;
    std::condition_variable call_processor_cv;
  };
}

#endif /* end of include guard: _LIBTUI_TUI_HH_ */
