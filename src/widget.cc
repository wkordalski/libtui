#include "widget.hh"

#include "window.hh"

namespace tui {
  void Widget::refresh() { if(this->get_window()) this->get_window()->refresh(); }
}