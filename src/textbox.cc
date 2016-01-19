#include "textbox.hh"

namespace tui {
  TextBox::TextBox(Application *app) : Widget(app) {
    this->locator = [this](Rectangle s) -> Rectangle { return {this->position, this->size}; };
  }

  void TextBox::draw() {
    auto * cwin = this->get_window()->get_curses_window();
    ::wmove(cwin, position.y, position.x);
    for(int i = 0; i < size.w; i++) {
      ::waddch(cwin, '_');
    }
    ::wmove(cwin, position.y, position.x);
    ::wprintw(cwin, "%s", text.substr(shift, size.w).c_str());
    ::wmove(cwin, position.y, position.x + cursor - shift);
  }
}
