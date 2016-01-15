#include "window.hh"


namespace tui {

    Window::Window(Widget *parent, Point position, Size size) : Widget(parent), position(position), size(size) {
      cwin = newwin(size.h, size.w, position.y, position.x);
      window_locator = [this](Size parent_size) -> std::pair<Point, Size>
      {
        return std::make_pair(this->position, this->size);
      };
    }
    Window::~Window() {
      delwin(cwin);
    }

    void Window::refresh() {
      for(auto w : widgets) {
        w->draw();
      }
      ::wrefresh(cwin);
    }
    void Window::resize(Point position, Size size) {
      delwin(cwin);
      this->position = position;
      this->size = size;
      cwin = newwin(size.h, size.w, position.y, position.x);
    }

    void Window::add(Widget *widget) {
      for(auto w : widgets) {
        if(widget == w) throw std::logic_error("Can not add the same widget two times.");
      }
      widgets.push_back(widget);
    }

    WINDOW * Window::get_curses_window() { return cwin; }

    const Window * Window::get_window() const { return this; }
    Window * Window::get_window() { return this; }

    void Window::draw() {}

    void Window::parent_resize(Size parent_size) {
      Point p;
      Size s;
      std::tie(p, s) = this->window_locator(parent_size);
      this->resize(p, s);
    }
}
