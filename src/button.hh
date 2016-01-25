#ifndef _LIBTUI_BUTTON_HH_
#define _LIBTUI_BUTTON_HH_

#include "text.hh"
#include "widget.hh"
#include "window.hh"

namespace tui {
  class Button : public Widget {
  public:
    Button(Application *app, std::string text = "");


    void set_action(const std::function<void()>& action) {
      this->action = action;
    }

    void set_locator(Locator locator) {
      this->locator = locator;
    }

    void set_text(std::string text) {
      this->text = text;
      if(visible) this->refresh();
    }
    
    void set_visibility(bool visibility) {
      this->visible = visibility;
      this->refresh();
    }

  protected:
    virtual void draw() {
      if(visible) {
        WINDOW *cwin = this->get_window()->get_curses_window();
        auto ss = fit_string_in_gap(text, size.w-2, TextAlign::center);
        ::wmove(cwin, position.y, position.x + 1 + (size.w-2 - ss.size())/2);
        ::wprintw(cwin, "%s", ss.c_str());
        ::wmove(cwin, position.y, position.x);
        ::waddch(cwin, focused?'[':'<');
        ::wmove(cwin, position.y, position.x+size.w-1);
        ::waddch(cwin, focused?']':'>');
      }
    }
    virtual void parent_resize(Rectangle parent) {
      std::tie(position, size) = locator(parent);
    }
    virtual void key(int ch) {
      if(visible) {
        if(ch == '\n') {
          action();
        }
      }
    }
    virtual void focus() { focused = true; }
    virtual void blur() { focused = false; }
    virtual bool is_focusable() { return visible; }

  protected:
    std::string text = "";
    Point position;
    Size size;
    Locator locator;
    std::function<void()> action = [](){};
    bool focused = false;
  };
}

#endif /* end of include guard: _LIBTUI_BUTTON_HH_ */
