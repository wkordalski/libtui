#ifndef _LIBTUI_SIZE_HH_
#define _LIBTUI_SIZE_HH_

#include <utility>

namespace tui {
  class Size {
  public:
    int w = 0;
    int h = 0;
  public:
    Size() : w(0), h(0) {}
    Size(int w, int h) : w(w), h(h) {}
    Size(const std::pair<int, int> &p) : w(p.first), h(p.second) {}

    Size & operator= (const Size &orig) {
      w = orig.w;
      h = orig.h;
      return *this;
    }

    Size & operator += (const Size &rhs) {
      w += rhs.w;
      h += rhs.h;
      return *this;
    }

    bool operator == (const Size &rhs) {
      return w == rhs.w && h == rhs.h;
    }

    bool operator != (const Size &rhs) {
      return !(*this == rhs);
    }

    operator std::pair<int, int>() const {
      return {w, h};
    }
  };
}

#endif /* end of include guard: _LIBTUI_SIZE_HH_ */
