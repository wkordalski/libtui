#include "checkbox.hh"

namespace tui {

  CheckBox::CheckBox(Application *app, std::string text, bool checked)
      : Widget(app), text(text), checked(checked) {
        locator = [this] (Rectangle parent) -> Rectangle {
        return {this->position, this->size};
      };
    }
  void CheckBox::draw() {
    Window *wnd = this->get_window();
    WINDOW *cwin = wnd->get_curses_window();
    std::string str = fit_string_in_gap(std::string() + (this->focused?"<":"[")+(this->checked?"X":" ")+(this->focused?">":"]")+" "+text, size.w, TextAlign::left);
    ::wmove(cwin, position.y, position.x);
    ::wprintw(cwin, "%s", str.c_str());
  }

    void CheckBox::parent_resize(Rectangle parent) {
      std::tie(position, size) = locator(parent);
    }
}
