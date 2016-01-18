#ifndef _LIBTUI_PROGRESSBAR_HH_
#define _LIBTUI_PROGRESSBAR_HH_

#include "widget.hh"
#include "window.hh"
#include "tui.hh"

#include <chrono>
#include <mutex>
#include <thread>

namespace tui {
  class ProgressBar : public Widget {
  public:
    ProgressBar(Application *app);
    ~ProgressBar() {
      set_unknown_progress(false);
    }

    void set_locator(Locator locator) {
      this->locator = locator;
    }

  protected:
    virtual void draw() {
      WINDOW * cwin = this->get_window()->get_curses_window();
      if(unknown) {
        ::wmove(cwin, position.y, position.x);
        for(int i = 0; i < size.w; i++) {
          ::waddch(cwin, ' ');
        }
        std::lock_guard<std::mutex> lock(this->mutex);
        int sbw = size.w *mq_width/100;
        if(this->shift < 0) {
          ::wmove(cwin, position.y, position.x);
          for(int i = 0; i < sbw + shift; i++) {
            ::waddch(cwin, '#');
          }
        } else {
          if(shift + sbw > size.w) {
            ::wmove(cwin, position.y, position.x);
            for(int i = 0; i < (sbw + shift) - size.w; i++) {
              ::waddch(cwin, '#');
            }
          }
          ::wmove(cwin, position.y, position.x + shift);
          for(int i = shift; i < std::min(size.w, shift + sbw); i++) {
            ::waddch(cwin, '#');
          }
        }
      } else {
        // TODO - print progress bar
      }
    }
    virtual void parent_resize(Size parent_size) {
      std::lock_guard<std::mutex> lock(this->mutex);
      std::tie(position, size) = locator(parent_size);
      ms_per_shift = 1000*100/(size.w * velocity);
    }

  public:
    void set_unknown_progress(bool is_unknown);

    void clock_procedure() {
      using ticker = std::chrono::steady_clock;
      auto tp = ticker::now();
      while(1) {
        {
          std::lock_guard<std::mutex> lock(this->mutex);
          if(this->unknown == false) return;
          this->shift++;
          if(this->shift >= size.w) this->shift -= size.w;
          tp += std::chrono::milliseconds(ms_per_shift);
        }
        app->call([this](){if(this->get_window()) this->get_window()->refresh();});
        std::this_thread::sleep_until(tp);
      }
    }
  protected:
    std::string text = "";
    std::string overflow_text = "...";
    int shift = 0;
    int value = 0;
    int maxvalue = 100;
    int velocity = 40;  // % paska na sekundę
    int mq_width = 30;  // % paska
    int ms_per_shift;
    bool unknown = false;
    Point position;
    Size size;
    Locator locator;
    std::mutex mutex;
    std::thread clock;
  };
}

#endif /* end of include guard: _LIBTUI_PROGRESSBAR_HH_ */
