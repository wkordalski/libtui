#ifndef _LIBTUI_POSITIONING_HH_
#define _LIBTUI_POSITIONING_HH_

#include "point.hh"
#include "size.hh"

#include <functional>
#include <utility>

namespace tui {
  using Rectangle = std::pair<Point, Size>;
  using Locator = std::function<Rectangle(Size)>;
}

#endif /* end of include guard: _LIBTUI_POSITIONING_HH_ */
