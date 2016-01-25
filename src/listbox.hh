#ifndef _LIBTUI_LISTBOX_HH_
#define _LIBTUI_LISTBOX_HH_

#include "positioning.hh"
#include "text.hh"
#include "widget.hh"
#include "window.hh"

#include <string>

namespace tui {
  class ListBox : public Widget {
  public:
    ListBox(Application *app);

    void set_locator(Locator locator) {
      this->locator = locator;
    }

    void set_elements(std::vector<std::string>&& elements) {
      this->elements = std::move(elements);
      /*
      if(this->elements.size() > selected_index - 1) {
        selected_index = this->elements.size() - 1;
        shift = std::max(0, selected_index - size.h + 1);
      }
      */
      if(this->elements.size() > 0) {
        selected_index = 0;
        shift = 0;
      }
      this->refresh();
    }
    void set_elements(const std::vector<std::string>& elements) {
      this->elements = elements;
      /*
      if(this->elements.size() > selected_index - 1) {
        selected_index = this->elements.size() - 1;
        shift = std::max(0, selected_index - size.h + 1);
      }
      */
      if(this->elements.size() > 0) {
        selected_index = 0;
        shift = 0;
      }
      this->refresh();
    }

    int get_selected_index() {
      return this->selected_index;
    }

    void set_no_rows_text(std::string text) {
      this->no_rows_text = text;
      if(elements.size() == 0 && this->get_window()) this->get_window()->refresh();
    }

    void set_action(std::function<void()> action) {
      this->action = action;
    }
  protected:
    virtual void draw() {
      if(visible) {
        WINDOW * cwin = this->get_window()->get_curses_window();
        if(elements.size() == 0) {
          std::string rnr = fit_string_in_gap(no_rows_text, size.w-4, TextAlign::center, overflow_text);
          ::wmove(cwin, position.y, size.w/2 - rnr.size()/2);
          ::wprintw(cwin, "%s", rnr.c_str());
        } else {
          for(int i = shift; i < shift + size.h && i < elements.size(); i++) {
            if(i == selected_index) {
              ::wmove(cwin, position.y + i - shift, position.x);
              ::waddch(cwin, (this->focused?'[':'<'));
              ::wmove(cwin, position.y + i - shift, position.x+size.w-1);
              ::waddch(cwin, (this->focused?']':'>'));
            }
            std::string rnr = fit_string_in_gap(elements[i], size.w - 2, TextAlign::left, overflow_text);
            ::wmove(cwin, position.y + i - shift, position.x + 2);
            ::wprintw(cwin, "%s", rnr.c_str());
          }
        }
      }
    }
    virtual void parent_resize(Rectangle parent) {
      std::tie(position, size) = locator(parent);
    }

    virtual bool is_focusable() { return elements.empty() == false; }
    virtual void focus() { this->focused = true; }
    virtual void blur() { this->focused = false; }

    virtual void key(int ch) {
      if(ch == KEY_DOWN) {
        if(elements.size() > 0) {
          if(selected_index + 1 < elements.size()) {
            selected_index++;
            if(selected_index + 1 == elements.size()) {
              if(shift + size.h < selected_index) shift = selected_index - size.h;
            }
            if(selected_index < elements.size() && selected_index >= shift + size.h - 1)
              shift = selected_index - size.h + 2;
            this->refresh();
          }
        }
      } else if(ch == KEY_UP) {
        if(elements.size() > 0) {
          if(selected_index > 0) {
            selected_index--;
            if(selected_index == 0) shift = 0;
            if(selected_index > 0 && selected_index <= shift) shift = selected_index - 1;
          } else {
            selected_index = 0;
            shift = 0;
          }
          this->refresh();
        }
      } else if(ch == '\n') {
        if(elements.size() > 0 && selected_index >= 0) {
          action();
        }
      }
    }

  protected:
    std::vector<std::string> elements;
    int selected_index = -1;
    int shift = 0;
    bool focused = false;
    std::string no_rows_text = "-- no rows --";
    std::string overflow_text = "...";
    Point position;
    Size size;
    Locator locator;
    std::function<void()> action = [](){};
  };
}

#endif /* end of include guard: _LIBTUI_LISTBOX_HH_ */
