#ifndef _LIBTUI_POSITIONING_HH_
#define _LIBTUI_POSITIONING_HH_

#include "point.hh"
#include "size.hh"

#include <functional>
#include <utility>

namespace tui {
  using Locator = std::function<std::pair<Point, Size>(Size)>;
}

#endif /* end of include guard: _LIBTUI_POSITIONING_HH_ */
