#ifndef _LIBTUI_POSITIONING_HH_
#define _LIBTUI_POSITIONING_HH_

#include "point.hh"
#include "size.hh"

#include <functional>
#include <utility>

namespace tui {
  class Application;

  using Rectangle = std::pair<Point, Size>;
  using Locator = std::function<Rectangle(Rectangle)>;

  Rectangle center_screen_locator(Rectangle parent, Application *app, Size target);
}

#endif /* end of include guard: _LIBTUI_POSITIONING_HH_ */
