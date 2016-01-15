#ifndef _LIBTUI_POINT_HH_
#define _LIBTUI_POINT_HH_

#include <utility>

namespace tui {
  class Point {
  public:
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    Point(const std::pair<int, int> &p) : x(p.first), y(p.second) {}

    Point & operator= (const Point &orig) {
      x = orig.x;
      y = orig.y;
      return *this;
    }

    Point & operator += (const Point &rhs) {
      x += rhs.x;
      y += rhs.y;
      return *this;
    }

    bool operator == (const Point &rhs) {
      return x == rhs.x && y == rhs.y;
    }

    bool operator != (const Point &rhs) {
      return !(*this == rhs);
    }

    operator std::pair<int, int>() const {
      return {x, y};
    }

  public:
    int x, y;
  };
}

#endif /* end of include guard: _LIBTUI_POINT_HH_ */
