#include "tui.hh"

#include <cassert>
#include <iostream>
#include <thread>

#include <ncurses.h>
#include <signal.h>
#include <unistd.h>

#include <sys/ioctl.h>

namespace {
  tui::Application *currapp = nullptr;
  void on_terminal_resize_handler(int sig) {
    struct winsize size;

    if (ioctl(fileno(stdout), TIOCGWINSZ, &size) == 0) {
        resize_term(size.ws_row, size.ws_col);
        //wrefresh(curscr);   /* Linux needs this */
    }
    if(currapp != nullptr) {
      currapp->terminal_resize();
    }
    signal(SIGWINCH, on_terminal_resize_handler);
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
    initscr();
    // Get screen size
    this->screen_size = this->get_screen_size();
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
    curs_set(0);
    signal(SIGWINCH, on_terminal_resize_handler);
  }

  Application::~Application() {
    endwin();
    currapp = nullptr;
  }

  void Application::run() {
    if(this->running) {
      throw std::logic_error("Can not run running tui::Application.");
    }
    this->running = true;

    std::thread keyboard_thread (std::bind(&Application::keyboard_worker, this));

    if(this->window != nullptr) {
      this->screen_size = {0, 0};
      this->on_terminal_resize();
      this->window->refresh();
    }

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
    ::clear();
    ::refresh();
    if(this->window != nullptr) {
      this->window->refresh();
    }
  }

  void Application::on_terminal_resize() {
    Size tmp = this->get_screen_size();
    if(this->screen_size != tmp) {
      this->screen_size = tmp;

      // Call window
      if(this->window != nullptr) {
        this->window->parent_resize(this->screen_size);
      }
      this->refresh();
    }
  }

  void Application::terminal_resize() {
    this->call(std::bind(&Application::on_terminal_resize, this));
  }

  void Application::on_keyboard(int ch) {
    if(ch == KEY_F(1)) {
      this->exit();
    } else {
      if(this->window) this->window->key(ch);
    }
    this->refresh();
  }

  Size Application::get_screen_size() {

    int w, h, x, y;
    getbegyx(stdscr, y, x);
    getmaxyx(stdscr, h, w);
    return {w-x, h-y};

    //return {COLS, LINES};
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
