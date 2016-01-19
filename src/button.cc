#include "button.hh"

namespace tui {
  Button::Button(Application *app, std::string text)
    : Widget(app), text(text) {
      this->locator = [](Rectangle s)-> Rectangle { return {}; };
    }
}
