#ifndef _LIBTUI_TEXTBOX_HH_
#define _LIBTUI_TEXTBOX_HH_

#include "positioning.hh"
#include "widget.hh"
#include "window.hh"

namespace tui {
  class TextBox : public Widget {
  public:
    TextBox(Application *app);

    std::string get_text() {
      return this->text;
    }
    
    void set_text(std::string s) {
      this->text = s;
      if(cursor > text.size()) {
        cursor = text.size();
        shift = std::max(0, int(text.size()) - size.w);
      }
      this->refresh();
    }
    
    void set_locator(Locator locator) {
      this->locator = locator;
    }

    void set_readonly(bool is_readonly) {
      this->readonly = is_readonly;
    }

  protected:
    virtual void draw();
    virtual void parent_resize(Rectangle parent) {
      std::tie(position, size) = locator(parent);
    }
    virtual void focus() {
      this->focused = true;
      if(cursor > size.w) {
        shift = cursor - size.w + 1;
      }
      ::wmove(this->get_window()->get_curses_window(), position.y, position.x + cursor - shift);
      ::curs_set(2);
    }
    virtual void blur() {
      ::curs_set(0);
      shift = 0;
    }

    virtual bool is_focusable() { return true; }

    virtual void key(int ch) {
      if(ch == KEY_LEFT) {
        if(cursor > 0) {
          cursor--;
          if(cursor == 0) shift = 0;
          if(cursor > 0 && cursor <= shift) shift = cursor - 1;
        }
      } else if(ch == KEY_RIGHT) {
        if(cursor < text.size()) {
          cursor++;
          if(cursor == text.size()) {
            if(shift + size.w < cursor) shift = cursor - size.w;
          }
          if(cursor < text.size() && cursor >= shift + size.w - 1)
            shift = cursor - size.w + 2;
        }
      } else if(ch == KEY_HOME) {
        cursor = shift = 0;
      } else if(ch == KEY_END) {
        cursor = text.size();
        shift = std::max(0, cursor - size.w + 1);
      } else if(ch == KEY_DC) {
        if(!readonly) {
          if(cursor < text.size()) {
            text.erase(cursor, 1);
          }
        }
      } else if(ch == KEY_BACKSPACE) {
        if(!readonly) {
          if(cursor > 0) {
            cursor--;
            text.erase(cursor, 1);
            if(cursor == 0) shift = 0;
            if(cursor > 0 && cursor <= shift) shift = cursor - 1;
          }
        }
      } else if(ch >= ' ' && ch < 0xff) {
        if(!readonly) {
          // TEXT INPUT
          text.insert(cursor, 1, (char)ch);
          cursor++;
          if(cursor == text.size()) {
            if(shift + size.w <= cursor) shift = cursor - size.w + 1;
          }
          if(cursor < text.size() && cursor >= shift + size.w - 1)
            shift = cursor - size.w + 2;
        }
      }
    }

    virtual void set_cursor() {
      ::wmove(this->get_window()->get_curses_window(), position.y, position.x + cursor - shift);
    }

  protected:
    std::string text = "";
    int cursor = 0;
    int shift = 0;
    bool focused = false;
    bool readonly = false;
    Point position;
    Size size;
    Locator locator;
  };
}

#endif /* end of include guard: _LIBTUI_TEXTBOX_HH_ */
