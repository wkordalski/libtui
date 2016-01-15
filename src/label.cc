#include "label.hh"

namespace tui {

  Label::Label(Widget *parent, Point position, Size size, std::string text)
      : Widget(parent), text(text), position(position), size(size) {
      if(size.h != 1) {
        throw std::invalid_argument("Label height must be one row - right now");
      }
      locator = [this] (Size parent_size) -> std::pair<Point, Size> {
        return {this->position, this->size};
      };
    }
  void Label::draw() {
    Window *wnd = this->get_window();
    WINDOW *cwin = wnd->get_curses_window();
    std::string str = fit_string_in_gap(text, size.w, align);
    switch(align) {
    case TextAlign::left:
      ::wmove(cwin, position.y, position.x); break;
    case TextAlign::right:
      ::wmove(cwin, position.y, position.x + (size.w - str.size())); break;
    case TextAlign::center:
      ::wmove(cwin, position.y, position.x + (size.w - str.size()) / 2); break;
    }
    ::wprintw(cwin, "%s", str.c_str());
  }

    void Label::parent_resize(Size parent_size) {
      std::tie(position, size) = locator(parent_size);
    }
}
