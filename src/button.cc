#include "button.hh"

namespace tui {
  Button::Button(Widget *parent, Point position, Size size, std::string text)
    : Widget(parent), position(position), size(size), text(text) {}
}
