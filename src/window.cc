#include "window.hh"

#include "text.hh"
#include "tui.hh"
#include <stdio.h>

namespace tui {

  Window::Window(Application *app) : Widget(app) {
    cwin = newwin(size.h, size.w, position.y, position.x);
    window_locator = [this](Size parent_size) -> std::pair<Point, Size>
    {
      return {{0,0}, parent_size};
    };
  }
  Window::~Window() {
    delwin(cwin);
  }

  void Window::refresh() {
    if(visible) {
      this->draw();
      this->set_cursor();
      ::wrefresh(cwin);
      for(auto w : widgets) {
        w->refresh();
      }
      if(child) child->refresh();
    }
  }
  void Window::resize(Point position, Size size) {
    delwin(cwin);
    this->position = position;
    this->size = size;
    cwin = newwin(size.h, size.w, position.y, position.x);
    for(auto w : widgets) {
      w->parent_resize(size);
    }
    if(child) child->parent_resize(size);
    this->refresh();
  }

  void Window::add(Widget *widget) {
    for(auto w : widgets) {
      if(widget == w) throw std::logic_error("Can not add the same widget two times.");
    }
    widget->parent = this;
    widgets.push_back(widget);
    if(widgets[focused]->is_focusable() == false || widgets.size() == 1) {
      focused = widgets.size() - 1;
      widget->focus();
    }
  }

  WINDOW * Window::get_curses_window() { return cwin; }

  const Window * Window::get_window() const { return this; }
  Window * Window::get_window() { return this; }

  void Window::draw() {
    if(visible) {
      ::wborder(cwin, '|', '|', '=', '-', '=', '=', '+', '+');
      if(!title.empty()) {
        int wdt = size.w - 8;
        std::string ct = fit_string_in_gap(title, wdt, TextAlign::center);
        ::wmove(cwin, 0, (size.w - ct.size())/2-2);
        ::wprintw(cwin, "[ %s ]", ct.c_str());
      }
      for(auto w : widgets) {
        w->draw();
      }
    }
  }

  void Window::parent_resize(Size parent_size) {
    Point p;
    Size s;
    std::tie(p, s) = this->window_locator(parent_size);
    this->resize(p, s);
  }

  void Window::key(int ch) {
    if(child) {
      child->key(ch);
      return;
    }
    if(ch == '\t') {
      if(widgets.size() > 0 && widgets[focused]->is_container()) {
        widgets[focused]->key(ch);
      } else if(widgets.size() > 1) {
        int nf = focused;
        nf = (nf+1)%widgets.size();
        while(nf != focused && widgets[nf]->is_focusable() == false)
          nf = (nf+1)%widgets.size();
        if(nf != focused) {
          widgets[focused]->blur();
          focused = nf;
          widgets[focused]->focus();
        }
      }
    } else {
      if(widgets.size() > 0) {
        widgets[focused]->key(ch);
      }
    }
  }

  void Window::set_child_window(Window *window) {
    if(window) {
      // loose focus
      if(widgets.size() > 0) widgets[focused]->blur();
      child = window;
      child->parent = this;
      child->parent_resize(size);
      child->focus();
    } else {
      child->blur();
      child->parent = nullptr;
      child = window;
      if(widgets.size() > 0) widgets[focused]->focus();
    }
    app->refresh();
  }
}
