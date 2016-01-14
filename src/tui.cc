#include "tui.hh"

#include <cassert>
#include <thread>

#include <ncurses.h>
#include <signal.h>
#include <unistd.h>

namespace {
  tui::Application *currapp = nullptr;
  void on_terminal_resize_handler(int) {
    if(currapp != nullptr) {
      currapp->terminal_resize();
      signal(SIGWINCH, on_terminal_resize_handler);
    }
  }
}

namespace tui {
  Application::Application() {
    using namespace std::placeholders;
    if(currapp != nullptr) {
      throw std::logic_error("Can not create more than one instance of tui::Application");
    }

    currapp = this;

    this->running = false;
    signal(SIGWINCH, on_terminal_resize_handler);
    initscr();
    // Get screen size
    std::tie(this->width, this->height) = this->get_screen_size();
    if(::has_colors()) {
      this->has_colors = true;
      start_color();
    } else {
      this->has_colors = false;
    }
    cbreak();
    // or raw() - it do not redirect Ctrl+C and Ctrl+Z to signal handlers
    noecho();
    keypad(stdscr, TRUE);
  }

  Application::~Application() {
    endwin();
    currapp = nullptr;
  }

  void Application::run() {
    if(this->running) {
      throw std::logic_error("Can not run running tui::Application.");
    }

    std::thread keyboard_thread (std::bind(&Application::keyboard_worker, this));

    this->running = true;
    while(this->running) {
      std::unique_lock<std::mutex> lock(this->call_queue_mutex);
      this->call_processor_cv.wait(lock,
        [this] ()->bool { return !this->call_queue.empty(); } );

      std::queue<std::function<void()>> local_call_queue;
      this->call_queue.swap(local_call_queue);

      lock.unlock();

      while(!local_call_queue.empty() && this->running) {
        auto e = local_call_queue.front();
        local_call_queue.pop();
        e();
      }
    }

    keyboard_thread.join();
  }

  void Application::exit() {
    this->running = false;
  }

  void Application::call(std::function<void()> action) {
    // Add element to the queue
    {
      std::lock_guard<std::mutex> lock(this->call_queue_mutex);
      this->call_queue.push(move(action));
    }
    // Call processor
    this->call_processor_cv.notify_one();
  }

  void Application::refresh() {
    // check if we need to send on_resize()...
  }

  void Application::on_terminal_resize() {
    int w, h;
    std::tie(w, h) = this->get_screen_size();
    if(this->width != w || this->height != h) {
      this->width = w;
      this->height = h;

      // Call parent
      if(this->window != nullptr) {
        this->window->parent_resize(this->width, this->height);
      }
    }
  }

  void Application::terminal_resize() {
    this->call(std::bind(&Application::on_terminal_resize, this));
  }

  void Application::on_keyboard(int ch) {
    if(ch == KEY_F(1)) {
      this->exit();
    }
  }

  std::pair<int, int> Application::get_screen_size() {
    int w, h, x, y;
    getbegyx(stdscr, y, x);
    getmaxyx(stdscr, h, w);
    return {w-x, h-y};
  }

  void Application::keyboard_worker() {
    struct pollfd ufds;
    ufds.fd = STDIN_FILENO;
    ufds.events = POLLIN;
    while(this->running) {
      switch(poll(&ufds, 1, 200)) {
      case -1:
        break;
      default:
        if(ufds.revents & POLLIN) {
          int ch = getch();
          this->call(std::bind(&Application::on_keyboard, this, ch));
        }
      }
    }
  }
}
