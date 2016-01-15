#ifndef _LIBTUI_LABEL_HH_
#define _LIBTUI_LABEL_HH_

#include "positioning.hh"
#include "text.hh"
#include "widget.hh"
#include "window.hh"

#include <string>

namespace tui {
  class Label : public Widget {
  public:
    Label(Widget *parent, Point position, Size size, std::string text = "");

    void set_text_align(TextAlign align) {
      this->align = align;
    }
  protected:
    virtual void draw();
    virtual void parent_resize(Size parent_size);

  protected:
    std::string text = "";
    TextAlign align = TextAlign::left;
    std::string overflow_text = "...";
    Point position;
    Size size;
    Locator locator;
  };
}

#endif /* end of include guard: _LIBTUI_LABEL_HH_ */
