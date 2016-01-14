#include "tui.hh"

#include <cassert>

#include <ncurses.h>
#include <unistd.h>



namespace tui {
  Application::Application() {
    using namespace std::placeholders;
    this->running = false;
    this->fds.push_back(pollfd{STDIN_FILENO, POLLIN});
    this->fds_actions.push_back(std::bind(&Application::on_keyboard, this, _1));
    initscr();
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
  }

  void Application::run() {
    // TODO: throw exception instead of asserting
    assert(this->running == false);

    this->running = true;
    while(this->running && this->window != nullptr) {
      // no windows - no running
      switch(poll(this->fds.data(), this->fds.size(), -1)) {
        case -1:
          this->refresh();
          break;
        default:
          for(int i = 0; i < this->fds.size(); i++) {
            if(this->fds[i].revents & this->fds[i].events) {
              this->fds_actions[i](this->fds[i].revents);
            }
          }
          // There is no refresh - we don't need it until widget asks for it
      }
    }
  }

  void Application::exit() {
    this->running = false;
  }

  void Application::refresh() {
    // TODO
  }

  void Application::on_keyboard(short event) {
    // TODO
  }
}
