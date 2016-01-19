#ifndef _LIBTUI_CHECKBOX_HH_
#define _LIBTUI_CHECKBOX_HH_

#include "positioning.hh"
#include "text.hh"
#include "widget.hh"
#include "window.hh"

#include <string>

namespace tui {
  class CheckBox : public Widget {
  public:
    CheckBox(Application *app, std::string text = "", bool checked = false);

    void set_text(std::string text) {
      this->text = text;
      this->get_window()->refresh();
    }

    void set_checked(bool state) {
      this->checked = state;
      this->get_window()->refresh();
    }

    bool get_checked() {
      return this->checked;
    }

    void set_locator(Locator locator) {
      this->locator = locator;
    }

    virtual bool is_focusable() { return true; }
    virtual void focus() { this->focused = true; }
    virtual void blur() { this->focused = false; }
  protected:
    virtual void draw();
    virtual void parent_resize(Rectangle parent);

  protected:
    std::string text = "";
    std::string overflow_text = "...";
    bool checked = false;
    bool focused = false;
    Point position;
    Size size;
    Locator locator;
  };
}

#endif /* end of include guard: _LIBTUI_CHECKBOX_HH_ */
